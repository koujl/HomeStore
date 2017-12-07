//
// Created by Kadayam, Hari on 15/11/17.
//

#ifndef OMSTORE_BLKSTORE_HPP
#define OMSTORE_BLKSTORE_HPP

#include "cache/cache.h"
#include "device/device_selector.hpp"
#include "device/device.h"
#include "device/blkbuffer.hpp"
#include "main/omstore.hpp"
#include "main/store_limits.h"
#include <boost/optional.hpp>
#include <omds/memory/mempiece.hpp>

namespace omstore {
enum BlkStoreCacheType {
    PASS_THRU       = 0,
    WRITEBACK_CACHE = 1,
    WRITETHRU_CACHE = 2
};

/* Threshold of size upto when there is overlap in the cache entry, that it will discard instead of copying. Say
 * there is a buffer of size 64K, out of which first N bytes are freed, then remaining bytes 64K - N bytes could
 * be either be discarded or copied into new buffer. This threshold dictates whats the value of (64K - N) upto which
 * it will copy. In other words ((64K - N) <= CACHE_DISCARD_THRESHOLD_SIZE) ? copy : discard
 */
#define CACHE_DISCARD_THRESHOLD_SIZE  16384

class BlkStoreConfig
{
public:
    /* Total size of BlkStore inital, it could grow based on demand. */
    uint64_t m_initial_size;

    /* Type of cache to use. */
    BlkStoreCacheType m_cache_type;

    /* Mirrored copy to maintain within this block store. */
    uint32_t m_nmirrors;
};

template <typename BAllocator>
class BlkStore {
public:
    BlkStore(Cache< BlkId > *cache, BlkStoreConfig &cfg) :
            m_cfg(cfg),
            m_cache(cache),
            m_vdev(cfg.m_initial_size, cfg.m_nmirrors, true,
                   BLKSTORE_BLK_SIZE, DeviceManagerInstance.get_dev_list()) {
    }

    /* Allocate a new block of the size based on the hints provided */
    void alloc_blk(uint8_t nblks, blk_alloc_hints &hints, BlkId *out_blkid) {
        m_vdev.alloc_blk(nblks, hints, out_blkid);
    }

    /* Free the block previously allocated. Blkoffset refers to number of blks to skip in the BlkId and
     * nblks refer to the total blks from offset to free. This method returns a optional array of new
     * BlkIds - max of 2 in case complete BlkIds are not free. If it is single blk, it returns no value */
    boost::optional<std::array<BlkId, 2>> free_blk(const BlkId &bid, boost::optional<uint8_t> blkoffset,
                                                   boost::optional<uint8_t> nblks) {
        boost::intrusive_ptr< BlkBuffer > erased_buf;
        boost::optional<std::array< BlkId, 2>> ret_arr;

        // Check if its a full element freed. In that case remove the element in the cache and free it up
        if ((blkoffset.get_value_or(0) == 0) && ((nblks == boost::none) || (nblks == bid.get_nblks()))) {
            m_cache->erase(bid, &erased_buf);
            m_vdev.free_blk(bid);
            return boost::none;
        }

        // Not the entire block is freed. Remove the entire entry from cache and split into possibly 2 entries
        // and insert it.
        if (m_cache->erase(bid, &erased_buf)) {
            // If number of blks we are freeing is more than 80% of the total buffer in cache, it does not make sense
            // to collect other buffers, creating a copy etc.. Just consider the entire entry is out of cache
            if (nblks < (bid.get_nblks() * 0.8)) {
                uint8_t from_blk = blkoffset.get_value_or(0);
                uint8_t to_blk = from_blk + nblks.get_value_or(bid.get_nblks());
                std::array< boost::intrusive_ptr< BlkBuffer >, 2> bbufs = free_partial_cache(erased_buf, from_blk, to_blk);

                // Add the split entries to the cache.
                for (auto i = 0U; i < bbufs.size(); i++) {
                    ret_arr->at(i) = bbufs[i]->get_key();
                    boost::intrusive_ptr< BlkBuffer > out_buf;
                    bool inserted = m_cache->insert(ret_arr->at(i), bbufs[i], &out_buf);
                    assert(inserted);
                }
            }
        }

        BlkId tmp_bid(bid.get_id() + blkoffset, bid.get_nblks(), bid.get_chunk_num());
        m_vdev.free_blk(tmp_bid);

        return ret_arr;
    }

    /* Allocate a new block and write the contents to the allocated block and return the blkbuffer */
    boost::intrusive_ptr< BlkBuffer > alloc_and_write(omds::blob &blob, blk_alloc_hints &hints) {
        // First allocate the blk id based on the hints
        BlkId bid;
        m_vdev.alloc_blk(round_off(blob.size, BLKSTORE_BLK_SIZE), hints, &bid);

        // Insert the entry into the cache and then write to the device.
        return write(bid, blob);
    }

    /* Write the buffer. The BlkStore write does not support write in place and so it does not also support
     * writing to an offset.
     *
     * NOTE: While one could argue that even when it is not doing write in place it could still
     * create a new blkid and then write it on an offset from the blkid. So far there is no use case for that. To
     * avoid any confusion to the interface, the value_offset parameter is not provided for this write type. If
     * needed can be added later */
    boost::intrusive_ptr< BlkBuffer > write(BlkId &bid, omds::blob &blob) {
        // First try to create/insert a record for this blk id in the cache. If it already exists, it will simply
        // upvote the item.
        boost::intrusive_ptr< BlkBuffer > bbuf;
        bool inserted = m_cache->insert(bid, blob, 0 /* value_offset */, &bbuf);

        // TODO: Raise an exception if we are not able to insert - instead of assert
        assert(inserted);

        // Now write the data to the device
        m_vdev.write(bid, bbuf->get_memvec());
        return bbuf;
    }

    /* If the user already has created a blkbuffer, then use this method to use it to write the block */
    void write(BlkId &bid, boost::intrusive_ptr< BlkBuffer > in_buf) {
        // First try to create/insert a record for this blk id in the cache. If it already exists, it will simply
        // upvote the item.
        bool inserted = m_cache->insert(bid, in_buf, &in_buf);

        // TODO: Raise an exception if we are not able to insert - instead of assert
        assert(inserted);
        m_vdev.write(bid, in_buf->get_memvec());
    }

    /* Read the data for given blk id and size. This method allocates the required memory if not present in the cache
     * and returns an smart ptr to the BlkBuffer */
    boost::intrusive_ptr< BlkBuffer > read(BlkId &bid, uint32_t offset, uint32_t size) {
        // TODO: Convert this assert to exceptions
        assert((offset + size) <= 256 * BLKSTORE_BLK_SIZE);
        assert(offset < 256 * BLKSTORE_BLK_SIZE);
        assert((offset % BLKSTORE_BLK_SIZE) == 0);
        assert((size % BLKSTORE_BLK_SIZE) == 0);

        int cur_ind = 0;
        uint32_t cur_offset = offset;

        // Check if the entry exists in the cache.
        boost::intrusive_ptr< BlkBuffer > bbuf;
        bool cache_found = m_cache->get(bid, &bbuf);
        if (!cache_found) {
            // Not found in cache, create a new block buf and prepare it for insert to dev and cache.
            bbuf = omds::ObjectAllocator< BlkBuffer >::make_object();
            bbuf->set_key(bid);
        }

        omds::MemVector<BLKSTORE_BLK_SIZE>::cursor_t c;
        do {
            boost::optional< omds::MemPiece<BLKSTORE_BLK_SIZE> &> missing_mp =
                    bbuf->get_memvec_mutable().fill_next_missing_piece(c, cur_offset, size);
            if (!missing_mp) {
                // We don't have any missing pieces, so we are done reading the contents
                break;
            }
            cur_offset = missing_mp->end_offset();

            // Create a new block of memory for the missing piece
            uint8_t *ptr;
            int ret = posix_memalign((void **) &ptr, 4096, missing_mp->size()); // TODO: Align based on hw needs instead of 4k
            if (ret != 0) {
                throw std::bad_alloc();
            }
            missing_mp->set_ptr(ptr);

            // Read the missing piece from the device
            BlkId tmp_bid(bid.get_id() + missing_mp->offset()/BLKSTORE_BLK_SIZE,
                          missing_mp->size()/BLKSTORE_BLK_SIZE, bid.get_chunk_num());
            m_vdev.read(tmp_bid, missing_mp.get());
        } while (true);

        if (!cache_found) {
            boost::intrusive_ptr< BlkBuffer > new_bbuf;
            bool inserted = m_cache->insert(bbuf->get_key(), bbuf, &new_bbuf);
            if (!inserted) {
                // Between get and insert, other thread tried the same thing and inserted into the cache. Lets use
                // that entry in cache and free up the memory
                bbuf = new_bbuf;
            }
        }

        return bbuf;
    }

private:
    std::array< boost::intrusive_ptr< BlkBuffer >, 2> free_partial_cache(const boost::intrusive_ptr< BlkBuffer > inbuf,
                                                                         uint8_t from_nblk, uint8_t to_nblk) {
        std::array< boost::intrusive_ptr< BlkBuffer >, 2 > bbufs;
        int left_ind = 0, right_ind; // index within the vector the about to free blks cover
        uint32_t from_offset = from_nblk * BLKSTORE_BLK_SIZE;
        uint32_t to_offset = to_nblk * BLKSTORE_BLK_SIZE;

        auto &mvec = inbuf->get_memvec();
        const BlkId orig_b = inbuf->get_key();

        //////////////////// Do left hand side processing //////////////////////
        // Check if the from_blk in the cache is overlapping with previous blk for same BlkId range
        omds::MemVector< BLKSTORE_BLK_SIZE > left_mvec;
        if (from_offset) {
            bool is_left_overlap = mvec.find_index(from_offset, boost::none, &left_ind);
            for (auto i = 0; i < left_ind - 1; i++) { // Update upto the previous one.
                auto mp = mvec.get_nth_piece((uint32_t) i);
                left_mvec.push_back(mp);
            }

            if (is_left_overlap) {
                // Seems like we may be overlapping, create a new memory piece for remaining portion and set it
                auto left_mp = mvec.get_nth_piece((uint32_t) left_ind);
                auto sz = from_offset - left_mp.offset();
                if (sz) {
                    left_mp.set_size(sz);
                    left_mvec.push_back(left_mp);
                }
            }
        }

        //////////////////// Do right hand side processing //////////////////////
        // If the freed blks overlap and has some excess to the right of it, we will have to either copy the
        // remaining buffer into new buffer (so that it will be freed correctly) or simply discard them from cache
        omds::MemVector< BLKSTORE_BLK_SIZE > right_mvec;
        mvec.find_index(to_offset, boost::none, &right_ind);
        if (left_ind == right_ind) {
            auto right_mp = mvec.get_nth_piece((uint32_t)right_ind);
            uint32_t sz = (right_mp.offset()+right_mp.size()) - to_offset;
            if (sz && (sz <= CACHE_DISCARD_THRESHOLD_SIZE)) {
                uint8_t *ptr;
                int ret = posix_memalign((void **) &ptr, 4096, sz); // TODO: Align based on hw needs instead of 4k
                if (ret != 0) {
                    throw std::bad_alloc();
                }
                right_mp.set_ptr(ptr);
                right_mp.set_size(sz);
                right_mp.set_offset(to_offset);
                right_mvec.push_back(right_mp);
                right_ind++;
            } // Else case will simply discard that buffer from adding to new bbuf
        }

        for (auto i = right_ind; i < mvec.npieces(); i++) { // Update upto the tailing ones.
            auto mp = mvec.get_nth_piece((uint32_t)i);
            right_mvec.push_back(mp);
        }

        // Finally form the new BlkBuffer with new blkid and left mvec pieces
        uint32_t b = 0;
        if (from_nblk) {
            BlkId lb(orig_b.get_id(), from_nblk, orig_b.get_chunk_num());
            bbufs[b] = inbuf; // Use the same buffer as in buf
            bbufs[b]->set_key(lb);
            bbufs[b]->set_memvec(left_mvec);
            ++b;
        }

        // Similar to that to the right mvec pieces
        if (orig_b.get_nblks() - to_nblk) {
            BlkId rb(orig_b.get_id() + to_nblk, orig_b.get_nblks() - to_nblk, orig_b.get_chunk_num());
            bbufs[b] = omds::ObjectAllocator< BlkBuffer >::make_object();
            bbufs[b]->set_key(rb);
            bbufs[b]->set_memvec(right_mvec);
        }

        return bbufs;
    }

#if 0
    /* From the given blk buffer, free up portion of the cache (provided by from_blk to end_blk. It returns 2 new
     * buffer, the one before the from_blk and the one after end_blk. It is possible that the from_blk to end_blk backing
     * underlying buffer overlaps with one left to it or right to it or both, It does the following in that case
     *
     * a) If overlaps with only left: If non-overlapping buffer size is >30% of original buffer size or >16K
     * (whichever is greater)  then it retains the original buffer as is, adjust the left buffer's (mempiece) size. If
     * not, then it creates a new buffer with reduced size and copies the buffer.
     *
     * b) If overlaps with only right: Its similar to above one a)
     *
     * c) If overlaps with both left & right: If both sides satisfy condition of non-overalapping buffer size is
     * >30% of original buffer size or >16K, then it picks the one which needs least amount of copying and leaving the
     * other side to just adjust the size and not copy.
     */
    std::array<boost::intrusive_ptr< BlkBuffer >, 2> free_partial_cache(boost::intrusive_ptr< BlkBuffer > buf,
                                                                        uint8_t from_blk, uint8_t end_blk) {
        std::array< boost::intrusive_ptr< BlkBuffer >, 2 > bbufs;
        int left_ind, right_ind; // index within the vector the about to free blks cover

        auto &mvec = buf->get_memvec();
        const BlkId &orig_b = buf->get_key();

        //////////////////// Do left hand side processing //////////////////////
        // Generate a new BlkId for the left side portion
        BlkId lb(orig_b.get_id(), from_blk, orig_b.get_chunk_num());
        bbufs[0]->set_key(lb);

        // Check if the from_blk in the cache is overlapping with previous blk for same BlkId range
        bool is_left_overlap = mvec.bsearch(from_blk * BLKSTORE_BLK_SIZE, &left_ind);
        for (auto i = 0; i < left_ind-1; i++) { // Update upto the previous one.
            auto &mp = mvec.get_nth_piece((uint32_t)i);
            bbufs[0]->get_memvec_mutable().push_back(mp);
        }

        if (is_left_overlap) {
            auto &left_mp = mvec.get_nth_piece_mutable((uint32_t)left_ind);
            uint32_t left_overlap_sz = left_mp.offset() - from_blk*BLKSTORE_BLK_SIZE;
            uint32_t non_overlap_sz = left_mp.size() - left_overlap_sz;
            if (non_overlap_sz < 16384) {
                uint8_t *ptr;
                int ret = posix_memalign((void **) &ptr, 4096,
                                         non_overlap_sz); // TODO: Align based on hw needs instead of 4k
                if (ret != 0) {
                    throw std::bad_alloc();
                }
                left_mp.set_ptr(ptr);
            }
            left_mp.set_size(non_overlap_sz);
            bbufs[0]->get_memvec_mutable().push_back(left_mp);
        }

        //////////////////// Do right hand side processing //////////////////////
        BlkId rb(orig_b.get_id() + end_blk, orig_b.get_nblks() - end_blk, orig_b.get_chunk_num());
        bbufs[1]->set_key(rb);

        bool is_right_overlap = mvec.bsearch(end_blk * BLKSTORE_BLK_SIZE, &right_ind);
        if (is_right_overlap) {
            auto &right_mp = mvec.get_nth_piece_mutable((uint32_t)right_ind);
            uint32_t right_overlap_sz = (right_mp.offset()+right_mp.size()) - end_blk*BLKSTORE_BLK_SIZE;
            uint32_t non_overlap_sz = right_mp.size() - right_overlap_sz;
            if (non_overlap_sz < 16384) {
                uint8_t *ptr;
                int ret = posix_memalign((void **) &ptr, 4096,
                                         non_overlap_sz); // TODO: Align based on hw needs instead of 4k
                if (ret != 0) {
                    throw std::bad_alloc();
                }
                right_mp.set_ptr(ptr);
            } else {
                right_mp.set_ptr(ptr + )
            }
            right_mp.set_size(non_overlap_sz);
        }

        omds::MemPiece left_mp; omds::MemPiece right_mp;
        if (left_overlap > right_overlap) {
            if (make_sense_to_retain(mp.size(), left_overlap)) {
                left_mp.set_size(mp.size() - left_overlap);
            } else {

            }
        }




        // Prepare the left side of the buffer

        bbufs[0]->get_memvec_mutable().push_back()

    }
#endif

    bool make_sense_to_retain(uint32_t total_sz, uint32_t overlap_sz) {
        uint32_t non_overlap_sz = total_sz - overlap_sz;

        return ((non_overlap_sz >= 16384) || (non_overlap_sz) >
    }

    BlkId gen_offset_blkid(const BlkId &bid, uint8_t upto_blk) {

    }
private:
    BlkStoreConfig m_cfg;
    Cache< BlkId > *m_cache;
    VirtualDev<BAllocator, RoundRobinDeviceSelector> m_vdev;
};

}
#endif //OMSTORE_BLKSTORE_HPP