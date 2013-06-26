#ifndef PROTOC_TRANSENC_OARCHIVE_HPP
#define PROTOC_TRANSENC_OARCHIVE_HPP

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
#include <vector>
#include <map>
#include <set>
#include <utility> // std::pair
#include <limits>
#include <ostream>
#include <istream>
#include <boost/optional.hpp>
#include <boost/ref.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/detail/common_oarchive.hpp>
#include <boost/archive/detail/register_archive.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_enum.hpp>
#include <boost/utility/base_from_member.hpp>
#include <protoc/types.hpp>
#include <protoc/output_stream.hpp>
#include <protoc/transenc/encoder.hpp>
#include <protoc/basic_oarchive.hpp>

namespace protoc
{
namespace transenc
{

// FIXME: Change to use basic_oarchive (with base_from_member for transenc::encoder?)

class basic_oarchive
    : public protoc::basic_oarchive
{
public:
    basic_oarchive(transenc::encoder& enc)
        : protoc::basic_oarchive(enc)
    {}
};

// base_from_member is needed because we want to add a member that must be
// initialized before oarchive because it is passed as an argument to its
// constructor
class stream_oarchive
    : private boost::base_from_member< protoc::output_stream<protoc::uint8_t> >,
      private boost::base_from_member<transenc::encoder>,
      public basic_oarchive
{
    typedef protoc::output_stream<protoc::uint8_t> member1_type;
    typedef transenc::encoder member2_type;
    typedef boost::base_from_member<member1_type> base_member1_type;
    typedef boost::base_from_member<member2_type> base_member2_type;

public:
    stream_oarchive(std::ostream& stream)
        : base_member1_type(member1_type(stream)),
          base_member2_type(boost::ref(base_member1_type::member)),
          basic_oarchive(base_member2_type::member)
    {}
};

}
}

BOOST_SERIALIZATION_REGISTER_ARCHIVE(protoc::transenc::basic_oarchive);
BOOST_SERIALIZATION_REGISTER_ARCHIVE(protoc::transenc::stream_oarchive);

#endif /* PROTOC_TRANSENC_OARCHIVE_HPP */
