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
#include <protoc/exceptions.hpp>
#include <protoc/ubjson/iarchive.hpp>

namespace protoc
{
namespace ubjson
{

iarchive::iarchive(const char *begin, const char * end)
    : input(begin, end)
{
}

iarchive::~iarchive()
{
}

void iarchive::load_override(boost::serialization::nvp<bool> data, int)
{
    const ubjson::token type = input.next();
    switch (type)
    {
    case token_true:
        data.value() = true;
        break;

    case token_false:
        data.value() = false;
        break;

    default:
        std::ostringstream error;
        error << type;
        throw unexpected_token(error.str());
    }
}

void iarchive::load_override(boost::serialization::nvp<protoc::int8_t> data, int)
{
    const ubjson::token type = input.next();
    switch (type)
    {
    case token_int8:
        data.value() = input.get_int8();
        break;

    default:
        std::ostringstream error;
        error << type;
        throw unexpected_token(error.str());
    }
}

void iarchive::load_override(boost::serialization::nvp<protoc::int16_t> data, int)
{
    const ubjson::token type = input.next();
    switch (type)
    {
    case token_int16:
        data.value() = input.get_int16();
        break;

    case token_int8:
        data.value() = input.get_int8();
        break;

    default:
        std::ostringstream error;
        error << type;
        throw unexpected_token(error.str());
    }
}

void iarchive::load_override(boost::serialization::nvp<protoc::int32_t> data, int)
{
    const ubjson::token type = input.next();
    switch (type)
    {
    case token_int32:
        data.value() = input.get_int32();
        break;

    case token_int16:
        data.value() = input.get_int16();
        break;

    case token_int8:
        data.value() = input.get_int8();
        break;

    default:
        std::ostringstream error;
        error << type;
        throw unexpected_token(error.str());
    }
}

void iarchive::load_override(boost::serialization::nvp<protoc::int64_t> data, int)
{
    const ubjson::token type = input.next();
    switch (type)
    {
    case token_int64:
        data.value() = input.get_int64();
        break;

    case token_int32:
        data.value() = input.get_int32();
        break;

    case token_int16:
        data.value() = input.get_int16();
        break;

    case token_int8:
        data.value() = input.get_int8();
        break;

    default:
        std::ostringstream error;
        error << type;
        throw unexpected_token(error.str());
    }
}

void iarchive::load_override(boost::serialization::nvp<protoc::float32_t> data, int)
{
    const ubjson::token type = input.next();
    switch (type)
    {
    case token_float32:
        data.value() = input.get_float32();
        break;

    default:
        std::ostringstream error;
        error << type;
        throw unexpected_token(error.str());
    }
}

void iarchive::load_override(boost::serialization::nvp<protoc::float64_t> data, int)
{
    const ubjson::token type = input.next();
    switch (type)
    {
    case token_float64:
        data.value() = input.get_float64();
        break;

    case token_float32:
        data.value() = input.get_float32();
        break;

    default:
        std::ostringstream error;
        error << type;
        throw unexpected_token(error.str());
    }
}

void iarchive::load_override(boost::serialization::nvp<std::string> data, int)
{
    const ubjson::token type = input.next();
    switch (type)
    {
    case token_string:
        data.value() = input.get_string();
        break;

    default:
        std::ostringstream error;
        error << type;
        throw unexpected_token(error.str());
    }
}

}
}
