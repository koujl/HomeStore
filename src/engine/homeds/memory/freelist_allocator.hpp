//
// Created by Kadayam, Hari on 30/10/17.
//
#ifndef OMSTORE_CACHELISTALLOCATOR_HPP
#define OMSTORE_CACHELISTALLOCATOR_HPP

#include <utility>
#include <iostream>
#include <algorithm>
#include <folly/ThreadLocal.h>
#include <fds/utils.hpp>
#include <metrics/metrics.hpp>

namespace homeds {

struct free_list_header {
    free_list_header* next;
};

class FreeListAllocatorMetrics : public sisl::MetricsGroupWrapper {
public:
    explicit FreeListAllocatorMetrics() : sisl::MetricsGroupWrapper("FreeListAllocator", "Singleton") {
        REGISTER_COUNTER(freelist_alloc_hit, "freelist: Number of allocs from cache");
        REGISTER_COUNTER(freelist_alloc_miss, "freelist: Number of allocs from system");
        REGISTER_COUNTER(freelist_dealloc_passthru, "freelist: Number of dealloc not cached because of size mismatch");
        REGISTER_COUNTER(freelist_dealloc, "freelist: Number of deallocs to system");
        REGISTER_COUNTER(freelist_alloc_size, "freelist: size of alloc", sisl::_publish_as::publish_as_gauge);
        REGISTER_COUNTER(freelist_cache_size, "freelist: cache size", sisl::_publish_as::publish_as_gauge);

        register_me_to_farm();
    }

    static FreeListAllocatorMetrics& instance() {
        static FreeListAllocatorMetrics inst;
        return inst;
    }
};

template < uint16_t MaxListCount, std::size_t Size >
class FreeListAllocatorImpl {
private:
    free_list_header* m_head;
    int64_t m_list_count;

public:
    FreeListAllocatorImpl() : m_head(nullptr), m_list_count(0) {}

    ~FreeListAllocatorImpl() {
        free_list_header* hdr = m_head;
        while (hdr) {
            free_list_header* next = hdr->next;
            free((uint8_t*)hdr);
            hdr = next;
        }
    }

    uint8_t* allocate(uint32_t size_needed) {
        uint8_t* ptr;
        auto& metrics = FreeListAllocatorMetrics::instance();

        if (m_head == nullptr) {
            ptr = (uint8_t*)malloc(size_needed);
            COUNTER_INCREMENT(metrics, freelist_alloc_miss, 1);
        } else {
            ptr = (uint8_t*)m_head;
            COUNTER_INCREMENT(metrics, freelist_alloc_hit, 1);
            m_head = m_head->next;
            COUNTER_DECREMENT(metrics, freelist_cache_size, size_needed);
            m_list_count--;
        }

        COUNTER_INCREMENT(metrics, freelist_alloc_size, size_needed);
        return ptr;
    }

    bool deallocate(uint8_t* mem, uint32_t size_alloced) {
        auto& metrics = FreeListAllocatorMetrics::instance();

        COUNTER_DECREMENT(metrics, freelist_alloc_size, size_alloced);
        if (
#ifndef NDEBUG
            1 ||
#endif
            (size_alloced != Size) || (m_list_count == MaxListCount)) {
            if (size_alloced != Size) { COUNTER_INCREMENT(metrics, freelist_dealloc_passthru, 1); }
            free(mem);
            COUNTER_INCREMENT(metrics, freelist_dealloc, 1);
            return true;
        }
        auto* hdr = (free_list_header*)mem;
        COUNTER_INCREMENT(metrics, freelist_cache_size, size_alloced);
        hdr->next = m_head;
        m_head = hdr;
        m_list_count++;
        return true;
    }
};

template < uint16_t MaxListCount, std::size_t Size >
class FreeListAllocator {
private:
    folly::ThreadLocalPtr< FreeListAllocatorImpl< MaxListCount, Size > > m_impl;

public:
    static_assert((Size >= sizeof(uint8_t*)), "Size requested should be atleast a pointer size");

    FreeListAllocator() { m_impl.reset(nullptr); }

    ~FreeListAllocator() { m_impl.reset(nullptr); }

    uint8_t* allocate(uint32_t size_needed) {
        if (sisl_unlikely(m_impl.get() == nullptr)) { m_impl.reset(new FreeListAllocatorImpl< MaxListCount, Size >()); }

        return (m_impl->allocate(size_needed));
    }

    bool deallocate(uint8_t* mem, uint32_t size_alloced) {
        if (sisl_unlikely(m_impl.get() == nullptr)) { m_impl.reset(new FreeListAllocatorImpl< MaxListCount, Size >()); }

        return m_impl->deallocate(mem, size_alloced);
    }

    bool owns(uint8_t* mem) const { return true; }

    bool is_thread_safe_allocator() const { return true; }
};
} // namespace homeds

#endif // OMSTORE_CACHELISTALLOCATOR_HPP