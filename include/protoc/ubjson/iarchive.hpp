#ifndef PROTOC_UBJSON_IARCHIVE_HPP
#define PROTOC_UBJSON_IARCHIVE_HPP

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

#include <string>
#include <boost/serialization/nvp.hpp>
#include <boost/archive/detail/common_iarchive.hpp>
#include <boost/archive/detail/register_archive.hpp>
#include <protoc/types.hpp>
#include <protoc/ubjson/decoder.hpp>

namespace protoc
{
namespace ubjson
{

class iarchive : public boost::archive::detail::common_iarchive<iarchive>
{
    friend class boost::archive::load_access;

public:
    iarchive(const std::string&);
    ~iarchive();

  void load_override(boost::serialization::nvp<bool>, int);

  // Ignore these
  void load_override(boost::archive::version_type, int) {}
  void load_override(boost::archive::object_id_type, int) {}
  void load_override(boost::archive::object_reference_type, int) {}
  void load_override(boost::archive::class_id_type, int) {}
  void load_override(boost::archive::class_id_optional_type, int) {}
  void load_override(boost::archive::class_id_reference_type, int) {}
  void load_override(boost::archive::tracking_type, int) {}
  void load_override(boost::archive::class_name_type&, int) {}

  void load_binary(void *, std::size_t) {}

private:
    const std::string& buffer;
    decoder input;
};

}
}

BOOST_SERIALIZATION_REGISTER_ARCHIVE(protoc::ubjson::iarchive);

#endif /* PROTOC_UBJSON_ARCHIVE_HPP */
