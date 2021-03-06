// Generated by the protocol buffer compiler (edited to eliminate cpplint warnings)
// source: Entry.proto

#ifndef PROTOBUF_Entry_2eproto__INCLUDED
#define PROTOBUF_Entry_2eproto__INCLUDED



#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2006000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>

#include <string>
#include "Kinetic.pb.hpp"
// @@protoc_insertion_point(includes)

namespace kaos {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_Entry_2eproto();
void protobuf_AssignDesc_Entry_2eproto();
void protobuf_ShutdownFile_Entry_2eproto();

class Entry;

// ===================================================================

class Entry : public ::google::protobuf::Message {
public:
    Entry();
    virtual ~Entry();

    Entry(const Entry& from);

    inline Entry& operator=(const Entry& from) {
        CopyFrom(from);
        return *this;
    }

    inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
        return _unknown_fields_;
    }

    inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
        return &_unknown_fields_;
    }

    static const ::google::protobuf::Descriptor* descriptor();
    static const Entry& default_instance();

    void Swap(Entry* other);

    // implements Message ----------------------------------------------

    Entry* New() const;
    void CopyFrom(const ::google::protobuf::Message& from);
    void MergeFrom(const ::google::protobuf::Message& from);
    void CopyFrom(const Entry& from);
    void MergeFrom(const Entry& from);
    void Clear();
    bool IsInitialized() const;

    int ByteSize() const;
    bool MergePartialFromCodedStream(
        ::google::protobuf::io::CodedInputStream* input);
    void SerializeWithCachedSizes(
        ::google::protobuf::io::CodedOutputStream* output) const;
    ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
    int GetCachedSize() const { return _cached_size_; }
private:
    void SharedCtor();
    void SharedDtor();
    void SetCachedSize(int size) const;
public:
    ::google::protobuf::Metadata GetMetadata() const;

    // nested types ----------------------------------------------------

    // accessors -------------------------------------------------------

    // required bytes key = 1;
    inline bool has_key() const;
    inline void clear_key();
    static const int kKeyFieldNumber = 1;
    inline const ::std::string& key() const;
    inline void set_key(const ::std::string& value);
    inline void set_key(const char* value);
    inline void set_key(const void* value, size_t size);
    inline ::std::string* mutable_key();
    inline ::std::string* release_key();
    inline void set_allocated_key(::std::string* key);

    // required bytes value = 2;
    inline bool has_value() const;
    inline void clear_value();
    static const int kValueFieldNumber = 2;
    inline const ::std::string& value() const;
    inline void set_value(const ::std::string& value);
    inline void set_value(const char* value);
    inline void set_value(const void* value, size_t size);
    inline ::std::string* mutable_value();
    inline ::std::string* release_value();
    inline void set_allocated_value(::std::string* value);

    // required bytes version = 3;
    inline bool has_version() const;
    inline void clear_version();
    static const int kVersionFieldNumber = 3;
    inline const ::std::string& version() const;
    inline void set_version(const ::std::string& value);
    inline void set_version(const char* value);
    inline void set_version(const void* value, size_t size);
    inline ::std::string* mutable_version();
    inline ::std::string* release_version();
    inline void set_allocated_version(::std::string* version);

    // required bytes tag = 4;
    inline bool has_tag() const;
    inline void clear_tag();
    static const int kTagFieldNumber = 4;
    inline const ::std::string& tag() const;
    inline void set_tag(const ::std::string& value);
    inline void set_tag(const char* value);
    inline void set_tag(const void* value, size_t size);
    inline ::std::string* mutable_tag();
    inline ::std::string* release_tag();
    inline void set_allocated_tag(::std::string* tag);

    // optional .com.seagate.kinetic.proto.Command.Algorithm algorithm = 5;
    inline bool has_algorithm() const;
    inline void clear_algorithm();
    static const int kAlgorithmFieldNumber = 5;
    inline ::com::seagate::kinetic::proto::Command_Algorithm algorithm() const;
    inline void set_algorithm(::com::seagate::kinetic::proto::Command_Algorithm value);

    // @@protoc_insertion_point(class_scope:kaos.Entry)
private:
    inline void set_has_key();
    inline void clear_has_key();
    inline void set_has_value();
    inline void clear_has_value();
    inline void set_has_version();
    inline void clear_has_version();
    inline void set_has_tag();
    inline void clear_has_tag();
    inline void set_has_algorithm();
    inline void clear_has_algorithm();

    ::google::protobuf::UnknownFieldSet _unknown_fields_;

    ::google::protobuf::uint32 _has_bits_[1];
    mutable int _cached_size_;
    ::std::string* key_;
    ::std::string* value_;
    ::std::string* version_;
    ::std::string* tag_;
    int algorithm_;
    friend void  protobuf_AddDesc_Entry_2eproto();
    friend void protobuf_AssignDesc_Entry_2eproto();
    friend void protobuf_ShutdownFile_Entry_2eproto();

    void InitAsDefaultInstance();
    static Entry* default_instance_;
};
// ===================================================================


// ===================================================================

// Entry

// required bytes key = 1;
inline bool Entry::has_key() const {
    return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Entry::set_has_key() {
    _has_bits_[0] |= 0x00000001u;
}
inline void Entry::clear_has_key() {
    _has_bits_[0] &= ~0x00000001u;
}
inline void Entry::clear_key() {
    if (key_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        key_->clear();
    }
    clear_has_key();
}
inline const ::std::string& Entry::key() const {
    // @@protoc_insertion_point(field_get:kaos.Entry.key)
    return *key_;
}
inline void Entry::set_key(const ::std::string& value) {
    set_has_key();
    if (key_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        key_ = new ::std::string;
    }
    key_->assign(value);
    // @@protoc_insertion_point(field_set:kaos.Entry.key)
}
inline void Entry::set_key(const char* value) {
    set_has_key();
    if (key_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        key_ = new ::std::string;
    }
    key_->assign(value);
    // @@protoc_insertion_point(field_set_char:kaos.Entry.key)
}
inline void Entry::set_key(const void* value, size_t size) {
    set_has_key();
    if (key_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        key_ = new ::std::string;
    }
    key_->assign(reinterpret_cast<const char*>(value), size);
    // @@protoc_insertion_point(field_set_pointer:kaos.Entry.key)
}
inline ::std::string* Entry::mutable_key() {
    set_has_key();
    if (key_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        key_ = new ::std::string;
    }
    // @@protoc_insertion_point(field_mutable:kaos.Entry.key)
    return key_;
}
inline ::std::string* Entry::release_key() {
    clear_has_key();
    if (key_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        return NULL;
    }
    else {
        ::std::string* temp = key_;
        key_ = const_cast<::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
        return temp;
    }
}
inline void Entry::set_allocated_key(::std::string* key) {
    if (key_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        delete key_;
    }
    if (key) {
        set_has_key();
        key_ = key;
    }
    else {
        clear_has_key();
        key_ = const_cast<::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    }
    // @@protoc_insertion_point(field_set_allocated:kaos.Entry.key)
}

// required bytes value = 2;
inline bool Entry::has_value() const {
    return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Entry::set_has_value() {
    _has_bits_[0] |= 0x00000002u;
}
inline void Entry::clear_has_value() {
    _has_bits_[0] &= ~0x00000002u;
}
inline void Entry::clear_value() {
    if (value_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        value_->clear();
    }
    clear_has_value();
}
inline const ::std::string& Entry::value() const {
    // @@protoc_insertion_point(field_get:kaos.Entry.value)
    return *value_;
}
inline void Entry::set_value(const ::std::string& value) {
    set_has_value();
    if (value_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        value_ = new ::std::string;
    }
    value_->assign(value);
    // @@protoc_insertion_point(field_set:kaos.Entry.value)
}
inline void Entry::set_value(const char* value) {
    set_has_value();
    if (value_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        value_ = new ::std::string;
    }
    value_->assign(value);
    // @@protoc_insertion_point(field_set_char:kaos.Entry.value)
}
inline void Entry::set_value(const void* value, size_t size) {
    set_has_value();
    if (value_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        value_ = new ::std::string;
    }
    value_->assign(reinterpret_cast<const char*>(value), size);
    // @@protoc_insertion_point(field_set_pointer:kaos.Entry.value)
}
inline ::std::string* Entry::mutable_value() {
    set_has_value();
    if (value_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        value_ = new ::std::string;
    }
    // @@protoc_insertion_point(field_mutable:kaos.Entry.value)
    return value_;
}
inline ::std::string* Entry::release_value() {
    clear_has_value();
    if (value_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        return NULL;
    }
    else {
        ::std::string* temp = value_;
        value_ = const_cast<::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
        return temp;
    }
}
inline void Entry::set_allocated_value(::std::string* value) {
    if (value_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        delete value_;
    }
    if (value) {
        set_has_value();
        value_ = value;
    }
    else {
        clear_has_value();
        value_ = const_cast<::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    }
    // @@protoc_insertion_point(field_set_allocated:kaos.Entry.value)
}

// required bytes version = 3;
inline bool Entry::has_version() const {
    return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void Entry::set_has_version() {
    _has_bits_[0] |= 0x00000004u;
}
inline void Entry::clear_has_version() {
    _has_bits_[0] &= ~0x00000004u;
}
inline void Entry::clear_version() {
    if (version_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        version_->clear();
    }
    clear_has_version();
}
inline const ::std::string& Entry::version() const {
    // @@protoc_insertion_point(field_get:kaos.Entry.version)
    return *version_;
}
inline void Entry::set_version(const ::std::string& value) {
    set_has_version();
    if (version_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        version_ = new ::std::string;
    }
    version_->assign(value);
    // @@protoc_insertion_point(field_set:kaos.Entry.version)
}
inline void Entry::set_version(const char* value) {
    set_has_version();
    if (version_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        version_ = new ::std::string;
    }
    version_->assign(value);
    // @@protoc_insertion_point(field_set_char:kaos.Entry.version)
}
inline void Entry::set_version(const void* value, size_t size) {
    set_has_version();
    if (version_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        version_ = new ::std::string;
    }
    version_->assign(reinterpret_cast<const char*>(value), size);
    // @@protoc_insertion_point(field_set_pointer:kaos.Entry.version)
}
inline ::std::string* Entry::mutable_version() {
    set_has_version();
    if (version_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        version_ = new ::std::string;
    }
    // @@protoc_insertion_point(field_mutable:kaos.Entry.version)
    return version_;
}
inline ::std::string* Entry::release_version() {
    clear_has_version();
    if (version_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        return NULL;
    }
    else {
        ::std::string* temp = version_;
        version_ = const_cast<::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
        return temp;
    }
}
inline void Entry::set_allocated_version(::std::string* version) {
    if (version_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        delete version_;
    }
    if (version) {
        set_has_version();
        version_ = version;
    }
    else {
        clear_has_version();
        version_ = const_cast<::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    }
    // @@protoc_insertion_point(field_set_allocated:kaos.Entry.version)
}

// required bytes tag = 4;
inline bool Entry::has_tag() const {
    return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void Entry::set_has_tag() {
    _has_bits_[0] |= 0x00000008u;
}
inline void Entry::clear_has_tag() {
    _has_bits_[0] &= ~0x00000008u;
}
inline void Entry::clear_tag() {
    if (tag_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        tag_->clear();
    }
    clear_has_tag();
}
inline const ::std::string& Entry::tag() const {
    // @@protoc_insertion_point(field_get:kaos.Entry.tag)
    return *tag_;
}
inline void Entry::set_tag(const ::std::string& value) {
    set_has_tag();
    if (tag_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        tag_ = new ::std::string;
    }
    tag_->assign(value);
    // @@protoc_insertion_point(field_set:kaos.Entry.tag)
}
inline void Entry::set_tag(const char* value) {
    set_has_tag();
    if (tag_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        tag_ = new ::std::string;
    }
    tag_->assign(value);
    // @@protoc_insertion_point(field_set_char:kaos.Entry.tag)
}
inline void Entry::set_tag(const void* value, size_t size) {
    set_has_tag();
    if (tag_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        tag_ = new ::std::string;
    }
    tag_->assign(reinterpret_cast<const char*>(value), size);
    // @@protoc_insertion_point(field_set_pointer:kaos.Entry.tag)
}
inline ::std::string* Entry::mutable_tag() {
    set_has_tag();
    if (tag_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        tag_ = new ::std::string;
    }
    // @@protoc_insertion_point(field_mutable:kaos.Entry.tag)
    return tag_;
}
inline ::std::string* Entry::release_tag() {
    clear_has_tag();
    if (tag_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        return NULL;
    }
    else {
        ::std::string* temp = tag_;
        tag_ = const_cast<::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
        return temp;
    }
}
inline void Entry::set_allocated_tag(::std::string* tag) {
    if (tag_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        delete tag_;
    }
    if (tag) {
        set_has_tag();
        tag_ = tag;
    }
    else {
        clear_has_tag();
        tag_ = const_cast<::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    }
    // @@protoc_insertion_point(field_set_allocated:kaos.Entry.tag)
}

// optional .com.seagate.kinetic.proto.Command.Algorithm algorithm = 5;
inline bool Entry::has_algorithm() const {
    return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void Entry::set_has_algorithm() {
    _has_bits_[0] |= 0x00000010u;
}
inline void Entry::clear_has_algorithm() {
    _has_bits_[0] &= ~0x00000010u;
}
inline void Entry::clear_algorithm() {
    algorithm_ = -1;
    clear_has_algorithm();
}
inline ::com::seagate::kinetic::proto::Command_Algorithm Entry::algorithm() const {
    // @@protoc_insertion_point(field_get:kaos.Entry.algorithm)
    return static_cast<::com::seagate::kinetic::proto::Command_Algorithm>(algorithm_);
}
inline void Entry::set_algorithm(::com::seagate::kinetic::proto::Command_Algorithm value) {
    assert(::com::seagate::kinetic::proto::Command_Algorithm_IsValid(value));
    set_has_algorithm();
    algorithm_ = value;
    // @@protoc_insertion_point(field_set:kaos.Entry.algorithm)
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace kaos

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace protobuf
}  // namespace google
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_Entry_2eproto__INCLUDED
