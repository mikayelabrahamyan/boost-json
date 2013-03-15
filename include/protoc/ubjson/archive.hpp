#ifndef PROTOC_UBJSON_ARCHIVE_HPP
#define PROTOC_UBJSON_ARCHIVE_HPP

///////////////////////////////////////////////////////////////////////////////
//
// http://protoc.sourceforge.net/
//
// Copyright (C) 2013 Bjorn Reese <breese@users.sourceforge.net>
//
// Permission to use, copy, modify, and distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
// MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE AUTHORS AND
// CONTRIBUTORS ACCEPT NO RESPONSIBILITY IN ANY CONCEIVABLE MANNER.
//
///////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <ostream>
#include <boost/archive/detail/common_oarchive.hpp>
#include <protoc/types.hpp>
#include <protoc/output_stream.hpp>
#include <protoc/ubjson/encoder.hpp>

namespace protoc
{
namespace ubjson
{

// FIXME: Null via boost::optional
class oarchive : public boost::archive::detail::common_oarchive<oarchive>
{
    friend class boost::archive::save_access;

public:
    oarchive(std::ostream& stream);
    ~oarchive();

    // The const variants are needed when used in containers
    void save_override(const boost::serialization::nvp<bool>&, int);
    void save_override(const boost::serialization::nvp<const bool>&, int);
    void save_override(const boost::serialization::nvp<protoc::int8_t>&, int);
    void save_override(const boost::serialization::nvp<const protoc::int8_t>&, int);
    void save_override(const boost::serialization::nvp<protoc::int16_t>&, int);
    void save_override(const boost::serialization::nvp<const protoc::int16_t>&, int);
    void save_override(const boost::serialization::nvp<protoc::int32_t>&, int);
    void save_override(const boost::serialization::nvp<const protoc::int32_t>&, int);
    void save_override(const boost::serialization::nvp<protoc::int64_t>&, int);
    void save_override(const boost::serialization::nvp<const protoc::int64_t>&, int);
    void save_override(const boost::serialization::nvp<protoc::float32_t>&, int);
    void save_override(const boost::serialization::nvp<const protoc::float32_t>&, int);
    void save_override(const boost::serialization::nvp<protoc::float64_t>&, int);
    void save_override(const boost::serialization::nvp<const protoc::float64_t>&, int);
    void save_override(const boost::serialization::nvp<std::string>&, int);
    void save_override(const boost::serialization::nvp<const std::string>&, int);

  // Ignore these
  void save_override(const boost::archive::version_type, int) {}
  void save_override(const boost::archive::object_id_type, int) {}
  void save_override(const boost::archive::object_reference_type, int) {}
  void save_override(const boost::archive::class_id_type, int) {}
  void save_override(const boost::archive::class_id_optional_type, int) {}
  void save_override(const boost::archive::class_id_reference_type, int) {}
  void save_override(const boost::archive::tracking_type, int) {}
  void save_override(const boost::archive::class_name_type&, int) {}

  void save_binary(void *, std::size_t) {}

private:
    protoc::output_stream buffer;
    encoder output;
};

}
}

#endif /* PROTOC_UBJSON_ARCHIVE_HPP */
