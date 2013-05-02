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

#include <sstream>
#include <protoc/transenc/oarchive.hpp>

namespace protoc
{
namespace transenc
{

oarchive::oarchive(const encoder& output)
    : boost::archive::detail::common_oarchive<oarchive>(),
      output(output)
{
}

oarchive::~oarchive()
{
}

void oarchive::save_override(const boost::serialization::nvp<bool>& data, int)
{
    output.put(data.value());
}

void oarchive::save_override(const boost::serialization::nvp<const bool>& data, int)
{
    output.put(data.value());
}

void oarchive::save_override(const boost::serialization::nvp<protoc::int8_t>& data, int)
{
    output.put(data.value());
}

void oarchive::save_override(const boost::serialization::nvp<const protoc::int8_t>& data, int)
{
    output.put(data.value());
}

void oarchive::save_override(const boost::serialization::nvp<protoc::int16_t>& data, int)
{
    output.put(data.value());
}

void oarchive::save_override(const boost::serialization::nvp<const protoc::int16_t>& data, int)
{
    output.put(data.value());
}

void oarchive::save_override(const boost::serialization::nvp<protoc::int32_t>& data, int)
{
    output.put(data.value());
}

void oarchive::save_override(const boost::serialization::nvp<const protoc::int32_t>& data, int)
{
    output.put(data.value());
}

void oarchive::save_override(const boost::serialization::nvp<protoc::int64_t>& data, int)
{
    output.put(data.value());
}

void oarchive::save_override(const boost::serialization::nvp<const protoc::int64_t>& data, int)
{
    output.put(data.value());
}

void oarchive::save_override(const boost::serialization::nvp<protoc::float32_t>& data, int)
{
    output.put(data.value());
}

void oarchive::save_override(const boost::serialization::nvp<const protoc::float32_t>& data, int)
{
    output.put(data.value());
}

void oarchive::save_override(const boost::serialization::nvp<protoc::float64_t>& data, int)
{
    output.put(data.value());
}

void oarchive::save_override(const boost::serialization::nvp<const protoc::float64_t>& data, int)
{
    output.put(data.value());
}

void oarchive::save_override(const boost::serialization::nvp<std::string>& data, int)
{
    output.put(data.value());
}

void oarchive::save_override(const boost::serialization::nvp<const std::string>& data, int)
{
    output.put(data.value());
}

stream_oarchive::stream_oarchive(std::ostream& stream)
    : boost::base_from_member<member_type>(member_type(stream)),
      base_type(member)
{
}

}
}
