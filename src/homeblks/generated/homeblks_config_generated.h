// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_HOMEBLKSCONFIG_HOMEBLKSCFG_H_
#define FLATBUFFERS_GENERATED_HOMEBLKSCONFIG_HOMEBLKSCFG_H_

#include "flatbuffers/flatbuffers.h"

#include "utility/non_null_ptr.hpp"

namespace homeblkscfg {

struct Volume;
struct VolumeT;

struct GeneralConfig;
struct GeneralConfigT;

struct HomeBlksSettings;
struct HomeBlksSettingsT;

inline const flatbuffers::TypeTable *VolumeTypeTable();

inline const flatbuffers::TypeTable *GeneralConfigTypeTable();

inline const flatbuffers::TypeTable *HomeBlksSettingsTypeTable();

struct VolumeT : public flatbuffers::NativeTable {
  typedef Volume TableType;
  static FLATBUFFERS_CONSTEXPR const char *GetFullyQualifiedName() {
    return "homeblkscfg.VolumeT";
  }
  uint32_t estimated_pending_blk_reads;
  uint64_t blks_scan_query_batch_size;
  VolumeT()
      : estimated_pending_blk_reads(128),
        blks_scan_query_batch_size(10000) {
  }
};

struct Volume FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef VolumeT NativeTableType;
  static const flatbuffers::TypeTable *MiniReflectTypeTable() {
    return VolumeTypeTable();
  }
  static FLATBUFFERS_CONSTEXPR const char *GetFullyQualifiedName() {
    return "homeblkscfg.Volume";
  }
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_ESTIMATED_PENDING_BLK_READS = 4,
    VT_BLKS_SCAN_QUERY_BATCH_SIZE = 6
  };
  uint32_t estimated_pending_blk_reads() const {
    return GetField<uint32_t>(VT_ESTIMATED_PENDING_BLK_READS, 128);
  }
  bool mutate_estimated_pending_blk_reads(uint32_t _estimated_pending_blk_reads) {
    return SetField<uint32_t>(VT_ESTIMATED_PENDING_BLK_READS, _estimated_pending_blk_reads, 128);
  }
  uint64_t blks_scan_query_batch_size() const {
    return GetField<uint64_t>(VT_BLKS_SCAN_QUERY_BATCH_SIZE, 10000);
  }
  bool mutate_blks_scan_query_batch_size(uint64_t _blks_scan_query_batch_size) {
    return SetField<uint64_t>(VT_BLKS_SCAN_QUERY_BATCH_SIZE, _blks_scan_query_batch_size, 10000);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint32_t>(verifier, VT_ESTIMATED_PENDING_BLK_READS) &&
           VerifyField<uint64_t>(verifier, VT_BLKS_SCAN_QUERY_BATCH_SIZE) &&
           verifier.EndTable();
  }
  VolumeT *UnPack(const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  void UnPackTo(VolumeT *_o, const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  static flatbuffers::Offset<Volume> Pack(flatbuffers::FlatBufferBuilder &_fbb, const VolumeT* _o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);
};

struct VolumeBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_estimated_pending_blk_reads(uint32_t estimated_pending_blk_reads) {
    fbb_.AddElement<uint32_t>(Volume::VT_ESTIMATED_PENDING_BLK_READS, estimated_pending_blk_reads, 128);
  }
  void add_blks_scan_query_batch_size(uint64_t blks_scan_query_batch_size) {
    fbb_.AddElement<uint64_t>(Volume::VT_BLKS_SCAN_QUERY_BATCH_SIZE, blks_scan_query_batch_size, 10000);
  }
  explicit VolumeBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  VolumeBuilder &operator=(const VolumeBuilder &);
  flatbuffers::Offset<Volume> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Volume>(end);
    return o;
  }
};

inline flatbuffers::Offset<Volume> CreateVolume(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t estimated_pending_blk_reads = 128,
    uint64_t blks_scan_query_batch_size = 10000) {
  VolumeBuilder builder_(_fbb);
  builder_.add_blks_scan_query_batch_size(blks_scan_query_batch_size);
  builder_.add_estimated_pending_blk_reads(estimated_pending_blk_reads);
  return builder_.Finish();
}

flatbuffers::Offset<Volume> CreateVolume(flatbuffers::FlatBufferBuilder &_fbb, const VolumeT *_o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);

struct GeneralConfigT : public flatbuffers::NativeTable {
  typedef GeneralConfig TableType;
  static FLATBUFFERS_CONSTEXPR const char *GetFullyQualifiedName() {
    return "homeblkscfg.GeneralConfigT";
  }
  uint32_t shutdown_timeout_secs;
  uint64_t shutdown_status_check_freq_ms;
  GeneralConfigT()
      : shutdown_timeout_secs(300),
        shutdown_status_check_freq_ms(2000) {
  }
};

struct GeneralConfig FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef GeneralConfigT NativeTableType;
  static const flatbuffers::TypeTable *MiniReflectTypeTable() {
    return GeneralConfigTypeTable();
  }
  static FLATBUFFERS_CONSTEXPR const char *GetFullyQualifiedName() {
    return "homeblkscfg.GeneralConfig";
  }
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_SHUTDOWN_TIMEOUT_SECS = 4,
    VT_SHUTDOWN_STATUS_CHECK_FREQ_MS = 6
  };
  uint32_t shutdown_timeout_secs() const {
    return GetField<uint32_t>(VT_SHUTDOWN_TIMEOUT_SECS, 300);
  }
  bool mutate_shutdown_timeout_secs(uint32_t _shutdown_timeout_secs) {
    return SetField<uint32_t>(VT_SHUTDOWN_TIMEOUT_SECS, _shutdown_timeout_secs, 300);
  }
  uint64_t shutdown_status_check_freq_ms() const {
    return GetField<uint64_t>(VT_SHUTDOWN_STATUS_CHECK_FREQ_MS, 2000);
  }
  bool mutate_shutdown_status_check_freq_ms(uint64_t _shutdown_status_check_freq_ms) {
    return SetField<uint64_t>(VT_SHUTDOWN_STATUS_CHECK_FREQ_MS, _shutdown_status_check_freq_ms, 2000);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint32_t>(verifier, VT_SHUTDOWN_TIMEOUT_SECS) &&
           VerifyField<uint64_t>(verifier, VT_SHUTDOWN_STATUS_CHECK_FREQ_MS) &&
           verifier.EndTable();
  }
  GeneralConfigT *UnPack(const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  void UnPackTo(GeneralConfigT *_o, const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  static flatbuffers::Offset<GeneralConfig> Pack(flatbuffers::FlatBufferBuilder &_fbb, const GeneralConfigT* _o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);
};

struct GeneralConfigBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_shutdown_timeout_secs(uint32_t shutdown_timeout_secs) {
    fbb_.AddElement<uint32_t>(GeneralConfig::VT_SHUTDOWN_TIMEOUT_SECS, shutdown_timeout_secs, 300);
  }
  void add_shutdown_status_check_freq_ms(uint64_t shutdown_status_check_freq_ms) {
    fbb_.AddElement<uint64_t>(GeneralConfig::VT_SHUTDOWN_STATUS_CHECK_FREQ_MS, shutdown_status_check_freq_ms, 2000);
  }
  explicit GeneralConfigBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  GeneralConfigBuilder &operator=(const GeneralConfigBuilder &);
  flatbuffers::Offset<GeneralConfig> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<GeneralConfig>(end);
    return o;
  }
};

inline flatbuffers::Offset<GeneralConfig> CreateGeneralConfig(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t shutdown_timeout_secs = 300,
    uint64_t shutdown_status_check_freq_ms = 2000) {
  GeneralConfigBuilder builder_(_fbb);
  builder_.add_shutdown_status_check_freq_ms(shutdown_status_check_freq_ms);
  builder_.add_shutdown_timeout_secs(shutdown_timeout_secs);
  return builder_.Finish();
}

flatbuffers::Offset<GeneralConfig> CreateGeneralConfig(flatbuffers::FlatBufferBuilder &_fbb, const GeneralConfigT *_o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);

struct HomeBlksSettingsT : public flatbuffers::NativeTable {
  typedef HomeBlksSettings TableType;
  static FLATBUFFERS_CONSTEXPR const char *GetFullyQualifiedName() {
    return "homeblkscfg.HomeBlksSettingsT";
  }
  uint32_t version;
  sisl::embedded_t<VolumeT> volume;
  sisl::embedded_t<GeneralConfigT> general_config;
  HomeBlksSettingsT()
      : version(0) {
  }
};

struct HomeBlksSettings FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef HomeBlksSettingsT NativeTableType;
  static const flatbuffers::TypeTable *MiniReflectTypeTable() {
    return HomeBlksSettingsTypeTable();
  }
  static FLATBUFFERS_CONSTEXPR const char *GetFullyQualifiedName() {
    return "homeblkscfg.HomeBlksSettings";
  }
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_VERSION = 4,
    VT_VOLUME = 6,
    VT_GENERAL_CONFIG = 8
  };
  uint32_t version() const {
    return GetField<uint32_t>(VT_VERSION, 0);
  }
  bool mutate_version(uint32_t _version) {
    return SetField<uint32_t>(VT_VERSION, _version, 0);
  }
  const Volume *volume() const {
    return GetPointer<const Volume *>(VT_VOLUME);
  }
  Volume *mutable_volume() {
    return GetPointer<Volume *>(VT_VOLUME);
  }
  const GeneralConfig *general_config() const {
    return GetPointer<const GeneralConfig *>(VT_GENERAL_CONFIG);
  }
  GeneralConfig *mutable_general_config() {
    return GetPointer<GeneralConfig *>(VT_GENERAL_CONFIG);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint32_t>(verifier, VT_VERSION) &&
           VerifyOffset(verifier, VT_VOLUME) &&
           verifier.VerifyTable(volume()) &&
           VerifyOffset(verifier, VT_GENERAL_CONFIG) &&
           verifier.VerifyTable(general_config()) &&
           verifier.EndTable();
  }
  HomeBlksSettingsT *UnPack(const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  void UnPackTo(HomeBlksSettingsT *_o, const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  static flatbuffers::Offset<HomeBlksSettings> Pack(flatbuffers::FlatBufferBuilder &_fbb, const HomeBlksSettingsT* _o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);
};

struct HomeBlksSettingsBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_version(uint32_t version) {
    fbb_.AddElement<uint32_t>(HomeBlksSettings::VT_VERSION, version, 0);
  }
  void add_volume(flatbuffers::Offset<Volume> volume) {
    fbb_.AddOffset(HomeBlksSettings::VT_VOLUME, volume);
  }
  void add_general_config(flatbuffers::Offset<GeneralConfig> general_config) {
    fbb_.AddOffset(HomeBlksSettings::VT_GENERAL_CONFIG, general_config);
  }
  explicit HomeBlksSettingsBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  HomeBlksSettingsBuilder &operator=(const HomeBlksSettingsBuilder &);
  flatbuffers::Offset<HomeBlksSettings> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<HomeBlksSettings>(end);
    return o;
  }
};

inline flatbuffers::Offset<HomeBlksSettings> CreateHomeBlksSettings(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t version = 0,
    flatbuffers::Offset<Volume> volume = 0,
    flatbuffers::Offset<GeneralConfig> general_config = 0) {
  HomeBlksSettingsBuilder builder_(_fbb);
  builder_.add_general_config(general_config);
  builder_.add_volume(volume);
  builder_.add_version(version);
  return builder_.Finish();
}

flatbuffers::Offset<HomeBlksSettings> CreateHomeBlksSettings(flatbuffers::FlatBufferBuilder &_fbb, const HomeBlksSettingsT *_o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);

inline VolumeT *Volume::UnPack(const flatbuffers::resolver_function_t *_resolver) const {
  auto _o = new VolumeT();
  UnPackTo(_o, _resolver);
  return _o;
}

inline void Volume::UnPackTo(VolumeT *_o, const flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
  { auto _e = estimated_pending_blk_reads(); _o->estimated_pending_blk_reads = _e; };
  { auto _e = blks_scan_query_batch_size(); _o->blks_scan_query_batch_size = _e; };
}

inline flatbuffers::Offset<Volume> Volume::Pack(flatbuffers::FlatBufferBuilder &_fbb, const VolumeT* _o, const flatbuffers::rehasher_function_t *_rehasher) {
  return CreateVolume(_fbb, _o, _rehasher);
}

inline flatbuffers::Offset<Volume> CreateVolume(flatbuffers::FlatBufferBuilder &_fbb, const VolumeT *_o, const flatbuffers::rehasher_function_t *_rehasher) {
  (void)_rehasher;
  (void)_o;
  struct _VectorArgs { flatbuffers::FlatBufferBuilder *__fbb; const VolumeT* __o; const flatbuffers::rehasher_function_t *__rehasher; } _va = { &_fbb, _o, _rehasher}; (void)_va;
  auto _estimated_pending_blk_reads = _o->estimated_pending_blk_reads;
  auto _blks_scan_query_batch_size = _o->blks_scan_query_batch_size;
  return homeblkscfg::CreateVolume(
      _fbb,
      _estimated_pending_blk_reads,
      _blks_scan_query_batch_size);
}

inline GeneralConfigT *GeneralConfig::UnPack(const flatbuffers::resolver_function_t *_resolver) const {
  auto _o = new GeneralConfigT();
  UnPackTo(_o, _resolver);
  return _o;
}

inline void GeneralConfig::UnPackTo(GeneralConfigT *_o, const flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
  { auto _e = shutdown_timeout_secs(); _o->shutdown_timeout_secs = _e; };
  { auto _e = shutdown_status_check_freq_ms(); _o->shutdown_status_check_freq_ms = _e; };
}

inline flatbuffers::Offset<GeneralConfig> GeneralConfig::Pack(flatbuffers::FlatBufferBuilder &_fbb, const GeneralConfigT* _o, const flatbuffers::rehasher_function_t *_rehasher) {
  return CreateGeneralConfig(_fbb, _o, _rehasher);
}

inline flatbuffers::Offset<GeneralConfig> CreateGeneralConfig(flatbuffers::FlatBufferBuilder &_fbb, const GeneralConfigT *_o, const flatbuffers::rehasher_function_t *_rehasher) {
  (void)_rehasher;
  (void)_o;
  struct _VectorArgs { flatbuffers::FlatBufferBuilder *__fbb; const GeneralConfigT* __o; const flatbuffers::rehasher_function_t *__rehasher; } _va = { &_fbb, _o, _rehasher}; (void)_va;
  auto _shutdown_timeout_secs = _o->shutdown_timeout_secs;
  auto _shutdown_status_check_freq_ms = _o->shutdown_status_check_freq_ms;
  return homeblkscfg::CreateGeneralConfig(
      _fbb,
      _shutdown_timeout_secs,
      _shutdown_status_check_freq_ms);
}

inline HomeBlksSettingsT *HomeBlksSettings::UnPack(const flatbuffers::resolver_function_t *_resolver) const {
  auto _o = new HomeBlksSettingsT();
  UnPackTo(_o, _resolver);
  return _o;
}

inline void HomeBlksSettings::UnPackTo(HomeBlksSettingsT *_o, const flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
  { auto _e = version(); _o->version = _e; };
  { auto _e = volume(); if (_e) _o->volume = sisl::embedded_t<VolumeT>(_e->UnPack(_resolver)); };
  { auto _e = general_config(); if (_e) _o->general_config = sisl::embedded_t<GeneralConfigT>(_e->UnPack(_resolver)); };
}

inline flatbuffers::Offset<HomeBlksSettings> HomeBlksSettings::Pack(flatbuffers::FlatBufferBuilder &_fbb, const HomeBlksSettingsT* _o, const flatbuffers::rehasher_function_t *_rehasher) {
  return CreateHomeBlksSettings(_fbb, _o, _rehasher);
}

inline flatbuffers::Offset<HomeBlksSettings> CreateHomeBlksSettings(flatbuffers::FlatBufferBuilder &_fbb, const HomeBlksSettingsT *_o, const flatbuffers::rehasher_function_t *_rehasher) {
  (void)_rehasher;
  (void)_o;
  struct _VectorArgs { flatbuffers::FlatBufferBuilder *__fbb; const HomeBlksSettingsT* __o; const flatbuffers::rehasher_function_t *__rehasher; } _va = { &_fbb, _o, _rehasher}; (void)_va;
  auto _version = _o->version;
  auto _volume = _o->volume ? CreateVolume(_fbb, _o->volume.get(), _rehasher) : 0;
  auto _general_config = _o->general_config ? CreateGeneralConfig(_fbb, _o->general_config.get(), _rehasher) : 0;
  return homeblkscfg::CreateHomeBlksSettings(
      _fbb,
      _version,
      _volume,
      _general_config);
}

inline const flatbuffers::TypeTable *VolumeTypeTable() {
  static const flatbuffers::TypeCode type_codes[] = {
    { flatbuffers::ET_UINT, 0, -1 },
    { flatbuffers::ET_ULONG, 0, -1 }
  };
  static const char * const names[] = {
    "estimated_pending_blk_reads",
    "blks_scan_query_batch_size"
  };
  static const flatbuffers::TypeTable tt = {
    flatbuffers::ST_TABLE, 2, type_codes, nullptr, nullptr, names
  };
  return &tt;
}

inline const flatbuffers::TypeTable *GeneralConfigTypeTable() {
  static const flatbuffers::TypeCode type_codes[] = {
    { flatbuffers::ET_UINT, 0, -1 },
    { flatbuffers::ET_ULONG, 0, -1 }
  };
  static const char * const names[] = {
    "shutdown_timeout_secs",
    "shutdown_status_check_freq_ms"
  };
  static const flatbuffers::TypeTable tt = {
    flatbuffers::ST_TABLE, 2, type_codes, nullptr, nullptr, names
  };
  return &tt;
}

inline const flatbuffers::TypeTable *HomeBlksSettingsTypeTable() {
  static const flatbuffers::TypeCode type_codes[] = {
    { flatbuffers::ET_UINT, 0, -1 },
    { flatbuffers::ET_SEQUENCE, 0, 0 },
    { flatbuffers::ET_SEQUENCE, 0, 1 }
  };
  static const flatbuffers::TypeFunction type_refs[] = {
    VolumeTypeTable,
    GeneralConfigTypeTable
  };
  static const char * const names[] = {
    "version",
    "volume",
    "general_config"
  };
  static const flatbuffers::TypeTable tt = {
    flatbuffers::ST_TABLE, 3, type_codes, type_refs, nullptr, names
  };
  return &tt;
}

inline const homeblkscfg::HomeBlksSettings *GetHomeBlksSettings(const void *buf) {
  return flatbuffers::GetRoot<homeblkscfg::HomeBlksSettings>(buf);
}

inline const homeblkscfg::HomeBlksSettings *GetSizePrefixedHomeBlksSettings(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<homeblkscfg::HomeBlksSettings>(buf);
}

inline HomeBlksSettings *GetMutableHomeBlksSettings(void *buf) {
  return flatbuffers::GetMutableRoot<HomeBlksSettings>(buf);
}

inline bool VerifyHomeBlksSettingsBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<homeblkscfg::HomeBlksSettings>(nullptr);
}

inline bool VerifySizePrefixedHomeBlksSettingsBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<homeblkscfg::HomeBlksSettings>(nullptr);
}

inline void FinishHomeBlksSettingsBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<homeblkscfg::HomeBlksSettings> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedHomeBlksSettingsBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<homeblkscfg::HomeBlksSettings> root) {
  fbb.FinishSizePrefixed(root);
}

inline sisl::embedded_t<HomeBlksSettingsT> UnPackHomeBlksSettings(
    const void *buf,
    const flatbuffers::resolver_function_t *res = nullptr) {
  return sisl::embedded_t<HomeBlksSettingsT>(GetHomeBlksSettings(buf)->UnPack(res));
}

}  // namespace homeblkscfg

#endif  // FLATBUFFERS_GENERATED_HOMEBLKSCONFIG_HOMEBLKSCFG_H_
