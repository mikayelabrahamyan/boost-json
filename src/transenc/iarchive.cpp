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
#include <protoc/transenc/iarchive.hpp>

namespace protoc
{
namespace transenc
{

iarchive::iarchive(decoder::input_range::const_iterator begin,
                   decoder::input_range::const_iterator end)
    : input(begin, end)
{
}

iarchive::iarchive(const decoder& input)
    : input(input)
{
}

iarchive::~iarchive()
{
}

void iarchive::load_override(bool& value, int)
{
    const token type = input.type();
    switch (type)
    {
    case token_true:
        value = true;
        input.next();
        break;

    case token_false:
        value = false;
        input.next();
        break;

    default:
        std::ostringstream error;
        error << type;
        throw unexpected_token(error.str());
    }
}

void iarchive::load_override(protoc::int8_t& value, int)
{
    const token type = input.type();
    switch (type)
    {
    case token_int8:
        value = input.get_int8();
        input.next();
        break;

    default:
        std::ostringstream error;
        error << type;
        throw unexpected_token(error.str());
    }
}

void iarchive::load_override(protoc::int16_t& value, int)
{
    const token type = input.type();
    switch (type)
    {
    case token_int16:
        value = input.get_int16();
        input.next();
        break;

    case token_int8:
        value = input.get_int8();
        input.next();
        break;

    default:
        std::ostringstream error;
        error << type;
        throw unexpected_token(error.str());
    }
}

void iarchive::load_override(protoc::int32_t& value, int)
{
    const token type = input.type();
    switch (type)
    {
    case token_int32:
        value = input.get_int32();
        input.next();
        break;

    case token_int16:
        value = input.get_int16();
        input.next();
        break;

    case token_int8:
        value = input.get_int8();
        input.next();
        break;

    default:
        std::ostringstream error;
        error << type;
        throw unexpected_token(error.str());
    }
}

void iarchive::load_override(protoc::int64_t& value, int)
{
    const token type = input.type();
    switch (type)
    {
    case token_int64:
        value = input.get_int64();
        input.next();
        break;

    case token_int32:
        value = input.get_int32();
        input.next();
        break;

    case token_int16:
        value = input.get_int16();
        input.next();
        break;

    case token_int8:
        value = input.get_int8();
        input.next();
        break;

    default:
        std::ostringstream error;
        error << type;
        throw unexpected_token(error.str());
    }
}

void iarchive::load_override(protoc::float32_t& value, int)
{
    const token type = input.type();
    switch (type)
    {
    case token_float32:
        value = input.get_float32();
        input.next();
        break;

    default:
        std::ostringstream error;
        error << type;
        throw unexpected_token(error.str());
    }
}

void iarchive::load_override(protoc::float64_t& value, int)
{
    const token type = input.type();
    switch (type)
    {
    case token_float64:
        value = input.get_float64();
        input.next();
        break;

    case token_float32:
        value = input.get_float32();
        input.next();
        break;

    default:
        std::ostringstream error;
        error << type;
        throw unexpected_token(error.str());
    }
}

void iarchive::load_override(std::string& value, int)
{
    const token type = input.type();
    switch (type)
    {
    case token_string:
        value = input.get_string();
        input.next();
        break;

    default:
        std::ostringstream error;
        error << type;
        throw unexpected_token(error.str());
    }
}

}
}
