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

#include <protoc/basic_oarchive.hpp>

namespace protoc
{

basic_oarchive::basic_oarchive(encoder_base& encoder)
    : boost::archive::detail::common_oarchive<basic_oarchive>(),
      encoder(encoder)
{
}

basic_oarchive::~basic_oarchive()
{
}

void basic_oarchive::save_override(bool value, int)
{
    encoder.put(value);
}

void basic_oarchive::save_override(int value, int)
{
    encoder.put(value);
}

void basic_oarchive::save_override(long long value, int)
{
    encoder.put(value);
}

void basic_oarchive::save_override(float value, int)
{
    encoder.put(value);
}

void basic_oarchive::save_override(double value, int)
{
    encoder.put(value);
}

void basic_oarchive::save_override(const char *value, int)
{
    encoder.put(value);
}

void basic_oarchive::save_override(const std::string& value, int)
{
    encoder.put(value);
}

} // namespace protoc
