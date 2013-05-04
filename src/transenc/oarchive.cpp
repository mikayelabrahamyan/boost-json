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

void oarchive::save_override(bool value, int)
{
    output.put(value);
}

void oarchive::save_override(protoc::int8_t value, int)
{
    output.put(value);
}

void oarchive::save_override(protoc::int16_t value, int)
{
    output.put(value);
}

void oarchive::save_override(protoc::int32_t value, int)
{
    output.put(value);
}

void oarchive::save_override(protoc::int64_t value, int)
{
    output.put(value);
}

void oarchive::save_override(protoc::float32_t value, int)
{
    output.put(value);
}

void oarchive::save_override(protoc::float64_t value, int)
{
    output.put(value);
}

void oarchive::save_override(const char *value, int)
{
    output.put(value);
}

void oarchive::save_override(const std::string& value, int)
{
    output.put(value);
}

stream_oarchive::stream_oarchive(std::ostream& stream)
    : boost::base_from_member<member_type>(member_type(stream)),
      base_type(member)
{
}

}
}
