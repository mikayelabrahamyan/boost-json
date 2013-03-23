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
#include <protoc/transenc/codes.hpp>
#include <protoc/transenc/decoder.hpp>

namespace
{
// Code patterns (masked by 1111 10000)
const char code_pattern8 = '\xA0';
const char code_pattern_length8 = '\xA8';
const char code_pattern16 = '\xB0';
const char code_pattern_length16 = '\xB8';
const char code_pattern32 = '\xC0';
const char code_pattern_length32 = '\xC8';
const char code_pattern64 = '\xD0';
const char code_pattern_length64 = '\xD8';
} // anonymous namespace

namespace protoc
{
namespace transenc
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
    if (input.empty())
    {
        current.type = token_eof;
        return;
    }

    const input_range::value_type value = *input;
    if ((value & 0x80) == 0x00)
    {
        // Small positive integer
        current.type = token_int8;
        current.range = input_range(input.begin(), input.begin() + 1);
        ++input;
    }
    else if ((value & 0xE0) == 0xE0)
    {
        // Small negative integer
        current.type = token_int8;
        current.range = input_range(input.begin(), input.begin() + 1);
        ++input;
    }
    else
    {
        assert(value & 0x80);

        switch (value)
        {
        case code_false:
            current.type = token_false;
            ++input;
            break;

        case code_true:
            current.type = token_true;
            ++input;
            break;

        case code_null:
            current.type = token_null;
            ++input;
            break;

        case code_tuple_begin:
            current.type = token_tuple_begin;
            ++input;
            break;

        case code_tuple_end:
            current.type = token_tuple_end;
            ++input;
            break;

        case code_array_begin:
            current.type = token_array_begin;
            ++input;
            break;

        case code_array_end:
            current.type = token_array_end;
            ++input;
            break;

        case code_int8:
            current.type = next_int8();
            break;

        case code_int16:
            current.type = next_int16();
            break;

        case code_int32:
            current.type = next_int32();
            break;

        case code_int64:
            current.type = next_int64();
            break;

        case code_float32:
            current.type = next_float32();
            break;

        case code_float64:
            current.type = next_float64();
            break;

        case code_binary_int8:
            current.type = next_binary();
            break;

        case code_binary_int16:
            current.type = next_binary();
            break;

        case code_binary_int32:
            current.type = next_binary();
            break;

        case code_binary_int64:
            current.type = next_binary();
            break;

        case code_string_int8:
            current.type = next_string();
            break;

        case code_string_int16:
            current.type = next_string();
            break;

        case code_string_int32:
            current.type = next_string();
            break;

        case code_string_int64:
            current.type = next_string();
            break;

        default:
            current.type = next_unknown();
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

    const protoc::int16_t endian = 0x0001;
    const input_range::const_iterator data = current.range.begin();
    protoc::int16_t result;
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&endian)[0]] = static_cast<protoc::int8_t>(data[0]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&endian)[1]] = static_cast<protoc::int8_t>(data[1]);
    return result;
}

protoc::int32_t decoder::get_int32() const
{
    assert(current.type == token_int32);
    assert(current.range.size() == sizeof(protoc::int32_t));

    const protoc::int32_t endian = 0x00010203;
    const input_range::const_iterator data = current.range.begin();
    protoc::int32_t result;
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&endian)[0]] = static_cast<protoc::int8_t>(data[0]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&endian)[1]] = static_cast<protoc::int8_t>(data[1]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&endian)[2]] = static_cast<protoc::int8_t>(data[2]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&endian)[3]] = static_cast<protoc::int8_t>(data[3]);
    return result;
}

protoc::int64_t decoder::get_int64() const
{
    assert(current.type == token_int64);
    assert(current.range.size() == sizeof(protoc::int64_t));

    const protoc::int64_t endian = 0x0001020304050607;
    const input_range::const_iterator data = current.range.begin();
    protoc::int64_t result;
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&endian)[0]] = static_cast<protoc::int8_t>(data[0]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&endian)[1]] = static_cast<protoc::int8_t>(data[1]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&endian)[2]] = static_cast<protoc::int8_t>(data[2]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&endian)[3]] = static_cast<protoc::int8_t>(data[3]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&endian)[4]] = static_cast<protoc::int8_t>(data[4]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&endian)[5]] = static_cast<protoc::int8_t>(data[5]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&endian)[6]] = static_cast<protoc::int8_t>(data[6]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&endian)[7]] = static_cast<protoc::int8_t>(data[7]);
    return result;
}

protoc::float32_t decoder::get_float32() const
{
    assert(current.type == token_float32);
    assert(current.range.size() == sizeof(protoc::float32_t));

    // IEEE 754 single precision
    const protoc::int32_t endian = 0x00010203;
    const input_range::const_iterator data = current.range.begin();
    protoc::float32_t result;
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&endian)[0]] = static_cast<protoc::int8_t>(data[0]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&endian)[1]] = static_cast<protoc::int8_t>(data[1]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&endian)[2]] = static_cast<protoc::int8_t>(data[2]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&endian)[3]] = static_cast<protoc::int8_t>(data[3]);
    return result;
}

protoc::float64_t decoder::get_float64() const
{
    assert(current.type == token_float64);
    assert(current.range.size() == sizeof(protoc::float64_t));

    // IEEE 754 double precision
    const protoc::int64_t endian = 0x0001020304050607;
    const input_range::const_iterator data = current.range.begin();
    protoc::float64_t result;
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&endian)[0]] = static_cast<protoc::int8_t>(data[0]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&endian)[1]] = static_cast<protoc::int8_t>(data[1]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&endian)[2]] = static_cast<protoc::int8_t>(data[2]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&endian)[3]] = static_cast<protoc::int8_t>(data[3]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&endian)[4]] = static_cast<protoc::int8_t>(data[4]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&endian)[5]] = static_cast<protoc::int8_t>(data[5]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&endian)[6]] = static_cast<protoc::int8_t>(data[6]);
    ((volatile protoc::int8_t *)&result)[((protoc::int8_t *)&endian)[7]] = static_cast<protoc::int8_t>(data[7]);
    return result;
}

std::string decoder::get_binary() const
{
    assert(current.type == token_binary);

    return std::string(current.range.begin(), current.range.size());
}

std::string decoder::get_string() const
{
    assert(current.type == token_string);

    // FIXME: Validate string [ http://www.w3.org/International/questions/qa-forms-utf-8 ]
    return std::string(current.range.begin(), current.range.size());
}

token decoder::next_unknown()
{
    // Determine field length
    const input_range::value_type value = *input & 0xF8;
    switch (value)
    {
    case code_pattern8:
        ++input; // Skip token
        return next(token_null, sizeof(protoc::int8_t));

    case code_pattern16:
        ++input; // Skip token
        return next(token_null, sizeof(protoc::int16_t));

    case code_pattern32:
        ++input; // Skip token
        return next(token_null, sizeof(protoc::int32_t));

    case code_pattern64:
        ++input; // Skip token
        return next(token_null, sizeof(protoc::int64_t));

    case code_pattern_length8:
    case code_pattern_length16:
    case code_pattern_length32:
    case code_pattern_length64:
        {
            token type = next_binary();
            return (type == token_binary) ? token_null : type;
        }

    default:
        ++input;
        return token_null;
    }

    return token_error;
}

token decoder::next_int8()
{
    ++input; // Skip token

    return next(token_int8, sizeof(protoc::int8_t));
}

token decoder::next_int16()
{
    ++input; // Skip token

    return next(token_int16, sizeof(protoc::int16_t));
}

token decoder::next_int32()
{
    ++input; // Skip token

    return next(token_int32, sizeof(protoc::int32_t));
}

token decoder::next_int64()
{
    ++input; // Skip token

    return next(token_int64, sizeof(protoc::int64_t));
}

token decoder::next_float32()
{
    ++input; // Skip token

    return next(token_float32, sizeof(protoc::float32_t));
}

token decoder::next_float64()
{
    ++input; // Skip token

    return next(token_float64, sizeof(protoc::float64_t));
}

token decoder::next_string()
{
    token type = next_binary();
    return (type == token_binary) ? token_string : type;
}

token decoder::next_binary()
{
    const input_range::value_type value = *input & 0xF8;

    ++input; // Skip token

    if (input.empty())
    {
        return token_eof;
    }

    protoc::int64_t length;

    assert(value & 0x08);

    switch (value)
    {
    case code_binary_int8:
        // current.type must be set to make get_int8() work
        current.type = next(token_int8, sizeof(protoc::int8_t));
        if (current.type == token_eof)
        {
            return token_eof;
        }
        length = static_cast<protoc::uint8_t>(get_int8());
        break;

    case code_binary_int16:
        current.type = next(token_int16, sizeof(protoc::int16_t));
        if (current.type == token_eof)
        {
            return token_eof;
        }
        length = static_cast<protoc::uint16_t>(get_int16());
        break;

    case code_binary_int32:
        current.type = next(token_int32, sizeof(protoc::int32_t));
        if (current.type == token_eof)
        {
            return token_eof;
        }
        length = static_cast<protoc::uint32_t>(get_int32());
        break;

    case code_binary_int64:
        current.type = next(token_int64, sizeof(protoc::int64_t));
        if (current.type == token_eof)
        {
            return token_eof;
        }
        length = get_int64();
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
    return token_binary;
}

token decoder::next(token type, std::size_t size)
{
    if (input.size() < size)
    {
        return token_eof;
    }

    current.range = input_range(input.begin(), input.begin() + size);
    input += size;

    return type;
}

} // namespace transenc
} // namespace protoc
