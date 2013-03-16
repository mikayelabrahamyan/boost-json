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

#include <cassert>
#include <protoc/ubjson/decoder.hpp>

namespace protoc
{
namespace ubjson
{

decoder::decoder(const char *begin,
                 const char *end)
    : input(begin, end)
{
    current.type = token_eof;
    next();
}

token decoder::type() const
{
    return current.type;
}

void decoder::next()
{
    if (current.type == token_error)
    {
        return;
    }
 again:
    if (input.empty())
    {
        current.type = token_eof;
    }
    else
    {
        switch (*input)
        {
        case 'Z':
            current.type = token_null;
            ++input;
            break;

        case 'T':
            current.type = token_true;
            ++input;
            break;

        case 'F':
            current.type = token_false;
            ++input;
            break;

        case 'N':
            ++input;
            goto again;

        case 'B':
            current.type = next_int8();
            break;

        case 'i':
            current.type = next_int16();
            break;

        case 'I':
            current.type =  next_int32();
            break;

        case 'L':
            current.type = next_int64();
            break;

        case 'd':
            current.type = next_float32();
            break;

        case 'D':
            current.type = next_float64();
            break;

        case 's':
            current.type = next_string();
            break;

        case '{':
            current.type = token_object_begin;
            ++input;
            break;

        case '}':
            current.type = token_object_end;
            ++input;
            break;

        case '[':
            current.type = token_array_begin;
            ++input;
            break;

        case ']':
            current.type = token_array_end;
            ++input;
            break;

        default:
            current.type = token_error;
            break;
        }
    }
}

protoc::int8_t decoder::get_int8() const
{
    assert(current.type == token_int8);
    assert(current.range.size() == sizeof(protoc::int8_t));

    return static_cast<protoc::int8_t>(*current.range);
}

protoc::int16_t decoder::get_int16() const
{
    assert(current.type == token_int16);
    assert(current.range.size() == sizeof(protoc::int16_t));

    const protoc::int16_t ix = 0x0001;
    const input_range::const_iterator data = current.range.begin();
    protoc::int16_t result;
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&ix)[0]] = static_cast<protoc::int8_t>(data[0]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&ix)[1]] = static_cast<protoc::int8_t>(data[1]);
    return result;
}

protoc::int32_t decoder::get_int32() const
{
    assert(current.type == token_int32);
    assert(current.range.size() == sizeof(protoc::int32_t));

    const protoc::int32_t ix = 0x00010203;
    const input_range::const_iterator data = current.range.begin();
    protoc::int32_t result;
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&ix)[0]] = static_cast<protoc::int8_t>(data[0]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&ix)[1]] = static_cast<protoc::int8_t>(data[1]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&ix)[2]] = static_cast<protoc::int8_t>(data[2]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&ix)[3]] = static_cast<protoc::int8_t>(data[3]);
    return result;
}

protoc::int64_t decoder::get_int64() const
{
    assert(current.type == token_int64);
    assert(current.range.size() == sizeof(protoc::int64_t));

    const protoc::int64_t ix = 0x0001020304050607;
    const input_range::const_iterator data = current.range.begin();
    protoc::int64_t result;
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&ix)[0]] = static_cast<protoc::int8_t>(data[0]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&ix)[1]] = static_cast<protoc::int8_t>(data[1]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&ix)[2]] = static_cast<protoc::int8_t>(data[2]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&ix)[3]] = static_cast<protoc::int8_t>(data[3]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&ix)[4]] = static_cast<protoc::int8_t>(data[4]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&ix)[5]] = static_cast<protoc::int8_t>(data[5]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&ix)[6]] = static_cast<protoc::int8_t>(data[6]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&ix)[7]] = static_cast<protoc::int8_t>(data[7]);
    return result;
}

protoc::float32_t decoder::get_float32() const
{
    assert(current.type == token_float32);

    // FIXME: Report error for Infinity and NaN

    // IEEE 754 single precision
    const protoc::int32_t ix = 0x00010203;
    const input_range::const_iterator data = current.range.begin();
    protoc::float32_t result;
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&ix)[0]] = static_cast<protoc::int8_t>(data[0]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&ix)[1]] = static_cast<protoc::int8_t>(data[1]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&ix)[2]] = static_cast<protoc::int8_t>(data[2]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&ix)[3]] = static_cast<protoc::int8_t>(data[3]);
    return result;
}

protoc::float64_t decoder::get_float64() const
{
    assert(current.type == token_float64);

    // IEEE 754 double precision
    const protoc::int64_t ix = 0x0001020304050607;
    const input_range::const_iterator data = current.range.begin();
    protoc::float64_t result;
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&ix)[0]] = static_cast<protoc::int8_t>(data[0]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&ix)[1]] = static_cast<protoc::int8_t>(data[1]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&ix)[2]] = static_cast<protoc::int8_t>(data[2]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&ix)[3]] = static_cast<protoc::int8_t>(data[3]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&ix)[4]] = static_cast<protoc::int8_t>(data[4]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&ix)[5]] = static_cast<protoc::int8_t>(data[5]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&ix)[6]] = static_cast<protoc::int8_t>(data[6]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&ix)[7]] = static_cast<protoc::int8_t>(data[7]);
    return result;
}

std::string decoder::get_string() const
{
    assert(current.type == token_string);

    // FIXME: Validate string [ http://www.w3.org/International/questions/qa-forms-utf-8 ]
    return std::string(current.range.begin(), current.range.size());
}

token decoder::next_int8()
{
    ++input; // Skip token

    const std::size_t size = sizeof(protoc::int8_t);
    if (input.size() < size)
    {
        return token_eof;
    }

    current.range = input_range(input.begin(), input.begin() + size);
    input += size;

    return token_int8;
}

token decoder::next_int16()
{
    ++input; // Skip token

    const std::size_t size = sizeof(protoc::int16_t);
    if (input.size() < size)
    {
        return token_eof;
    }

    current.range = input_range(input.begin(), input.begin() + size);
    input += size;

    return token_int16;
}

token decoder::next_int32()
{
    ++input; // Skip token

    const std::size_t size = sizeof(protoc::int32_t);
    if (input.size() < size)
    {
        return token_eof;
    }

    current.range = input_range(input.begin(), input.begin() + size);
    input += size;

    return token_int32;
}

token decoder::next_int64()
{
    ++input; // Skip token

    const std::size_t size = sizeof(protoc::int64_t);
    if (input.size() < size)
    {
        return token_eof;
    }

    current.range = input_range(input.begin(), input.begin() + size);
    input += size;

    return token_int64;
}

token decoder::next_float32()
{
    // FIXME: Handle Infinity and NaN

    ++input; // Skip token

    const std::size_t size = sizeof(protoc::float32_t);
    if (input.size() < size)
    {
        return token_eof;
    }

    current.range = input_range(input.begin(), input.begin() + size);
    input += size;

    return token_float32;
}

token decoder::next_float64()
{
    // FIXME: Handle Infinity and NaN

    ++input; // Skip token

    const std::size_t size = sizeof(protoc::float64_t);
    if (input.size() < size)
    {
        return token_eof;
    }

    current.range = input_range(input.begin(), input.begin() + size);
    input += size;

    return token_float64;
}

token decoder::next_string()
{
    ++input; // Skip token

    if (input.empty())
    {
        return token_eof;
    }

    protoc::int64_t length;
    switch (*input)
    {
    case 'B':
        {
            current.type = next_int8();
            if (current.type == token_eof)
            {
                return token_eof;
            }
            length = static_cast<protoc::int64_t>(get_int8());
        }
        break;

    case 'i':
        {
            current.type = next_int16();
            if (current.type == token_eof)
            {
                return token_eof;
            }
            length = static_cast<protoc::int64_t>(get_int16());
        }
        break;

    case 'I':
        {
            current.type = next_int32();
            if (current.type == token_eof)
            {
                return token_eof;
            }
            length = static_cast<protoc::int64_t>(get_int32());
        }
        break;

    case 'L':
        {
            current.type = next_int64();
            if (current.type == token_eof)
            {
                return token_eof;
            }
            length = get_int64();
        }
        break;

    default:
        return token_error;
    }

    if (length < 0)
    {
        return token_error;
    }
    if (input.size() < length)
    {
        return token_eof;
    }

    current.range = input_range(input.begin(), input.begin() + length);
    input += length;
    return token_string;
}

}
}
