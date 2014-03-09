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
#include <protoc/msgpack/detail/codes.hpp>
#include <protoc/msgpack/detail/decoder.hpp>

// https://github.com/msgpack/msgpack/blob/master/spec.md

namespace protoc
{
namespace msgpack
{
namespace detail
{

decoder::decoder(input_range::const_iterator begin,
                 input_range::const_iterator end)
    : input(begin, end)
{
    current.type = token_eof;
    next();
}

decoder::decoder(const decoder& other)
    : input(other.input)
{
    current.type = other.current.type;
    current.range = other.current.range;
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

    input_range::value_type value = *input;
    if ((value & 0x80) == 0x00)
    {
        // Positive fixnum
        current.type = token_int8;
        current.range = input_range(input.begin(), input.begin() + 1);
        ++input;
    }
    else if ((value & 0xE0) == 0xE0)
    {
        // Negative fixnum
        current.type = token_int8;
        current.range = input_range(input.begin(), input.begin() + 1);
        ++input;
    }
    else if ((value & 0xE0) == 0xA0)
    {
        // Fix raw
        current.type = next_fixraw();
    }
    else if ((value & 0xF0) == 0x90)
    {
        // Fix array
        current.type = next_array8();
    }
    else
    {
        switch (value)
        {
        case code_null:
            current.type = token_null;
            ++input;
            break;

        case code_false:
            current.type = token_false;
            ++input;
            break;

        case code_true:
            current.type = token_true;
            ++input;
            break;

        case code_float32:
            current.type = next_float32();
            break;

        case code_float64:
            current.type = next_float64();
            break;

        case code_uint8:
            current.type = next_uint8();
            break;

        case code_uint16:
            current.type = next_uint16();
            break;

        case code_uint32:
            current.type = next_uint32();
            break;

        case code_uint64:
            current.type = next_uint64();
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

        case code_str8:
            current.type = next_raw8();
            break;

        case code_str16:
            current.type = next_raw16();
            break;

        case code_str32:
            current.type = next_raw32();
            break;

        case code_array16:
            current.type = next_array16();
            break;

        case code_array32:
            current.type = next_array32();
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

protoc::uint8_t decoder::get_uint8() const
{
    assert(current.type == token_uint8);
    assert(current.range.size() == sizeof(protoc::uint8_t));

    return static_cast<protoc::uint8_t>(*current.range);
}

protoc::uint16_t decoder::get_uint16() const
{
    assert((current.type == token_uint16) ||
           (current.type == token_array16));
    assert(current.range.size() == sizeof(protoc::uint16_t));

    const protoc::uint16_t endian = 0x0001;
    const input_range::const_iterator data = current.range.begin();
    protoc::uint16_t result;
    ((volatile protoc::uint8_t *)&result)[((protoc::uint8_t *)&endian)[0]] = static_cast<protoc::uint8_t>(data[0]);
    ((volatile protoc::uint8_t *)&result)[((protoc::uint8_t *)&endian)[1]] = static_cast<protoc::uint8_t>(data[1]);
    return result;
}

protoc::uint32_t decoder::get_uint32() const
{
    assert((current.type == token_uint32) ||
           (current.type == token_array32));
    assert(current.range.size() == sizeof(protoc::uint32_t));

    const protoc::uint32_t endian = 0x00010203;
    const input_range::const_iterator data = current.range.begin();
    protoc::uint32_t result;
    ((volatile protoc::uint8_t *)&result)[((protoc::uint8_t *)&endian)[0]] = static_cast<protoc::uint8_t>(data[0]);
    ((volatile protoc::uint8_t *)&result)[((protoc::uint8_t *)&endian)[1]] = static_cast<protoc::uint8_t>(data[1]);
    ((volatile protoc::uint8_t *)&result)[((protoc::uint8_t *)&endian)[2]] = static_cast<protoc::uint8_t>(data[2]);
    ((volatile protoc::uint8_t *)&result)[((protoc::uint8_t *)&endian)[3]] = static_cast<protoc::uint8_t>(data[3]);
    return result;
}

protoc::uint64_t decoder::get_uint64() const
{
    assert(current.type == token_uint64);
    assert(current.range.size() == sizeof(protoc::uint64_t));

    const protoc::uint64_t endian = 0x0001020304050607;
    const input_range::const_iterator data = current.range.begin();
    protoc::uint64_t result;
    ((volatile protoc::uint8_t *)&result)[((protoc::uint8_t *)&endian)[0]] = static_cast<protoc::uint8_t>(data[0]);
    ((volatile protoc::uint8_t *)&result)[((protoc::uint8_t *)&endian)[1]] = static_cast<protoc::uint8_t>(data[1]);
    ((volatile protoc::uint8_t *)&result)[((protoc::uint8_t *)&endian)[2]] = static_cast<protoc::uint8_t>(data[2]);
    ((volatile protoc::uint8_t *)&result)[((protoc::uint8_t *)&endian)[3]] = static_cast<protoc::uint8_t>(data[3]);
    ((volatile protoc::uint8_t *)&result)[((protoc::uint8_t *)&endian)[4]] = static_cast<protoc::uint8_t>(data[4]);
    ((volatile protoc::uint8_t *)&result)[((protoc::uint8_t *)&endian)[5]] = static_cast<protoc::uint8_t>(data[5]);
    ((volatile protoc::uint8_t *)&result)[((protoc::uint8_t *)&endian)[6]] = static_cast<protoc::uint8_t>(data[6]);
    ((volatile protoc::uint8_t *)&result)[((protoc::uint8_t *)&endian)[7]] = static_cast<protoc::uint8_t>(data[7]);
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

std::vector<protoc::uint8_t> decoder::get_raw() const
{
    assert((current.type == token_raw8) ||
           (current.type == token_raw16) ||
           (current.type == token_raw32));

    std::vector<protoc::uint8_t> result(current.range.begin(), current.range.end());
    return result;
}

protoc::uint32_t decoder::get_count() const
{
    assert((current.type == token_array8) ||
           (current.type == token_array16) ||
           (current.type == token_array32));

    switch (current.type)
    {
    case token_array8:
        return static_cast<protoc::uint32_t>(*current.range & 0x0F);

    case token_array16:
        return get_uint16();

    case token_array32:
        return get_uint32();

    default:
        assert(false);
        return 0;
    }
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

token decoder::next_uint8()
{
    ++input; // Skip token

    const std::size_t size = sizeof(protoc::uint8_t);
    if (input.size() < size)
    {
        return token_eof;
    }

    current.range = input_range(input.begin(), input.begin() + size);
    input += size;

    return token_uint8;
}

token decoder::next_uint16()
{
    ++input; // Skip token

    const std::size_t size = sizeof(protoc::uint16_t);
    if (input.size() < size)
    {
        return token_eof;
    }

    current.range = input_range(input.begin(), input.begin() + size);
    input += size;

    return token_uint16;
}

token decoder::next_uint32()
{
    ++input; // Skip token

    const std::size_t size = sizeof(protoc::uint32_t);
    if (input.size() < size)
    {
        return token_eof;
    }

    current.range = input_range(input.begin(), input.begin() + size);
    input += size;

    return token_uint32;
}

token decoder::next_uint64()
{
    ++input; // Skip token

    const std::size_t size = sizeof(protoc::uint64_t);
    if (input.size() < size)
    {
        return token_eof;
    }

    current.range = input_range(input.begin(), input.begin() + size);
    input += size;

    return token_uint64;
}

token decoder::next_float32()
{
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

token decoder::next_fixraw()
{
    assert(*input >= code_fixstr_0);
    assert(*input <= code_fixstr_31);

    const std::size_t size = *input - code_fixstr_0;

    ++input; // Skip token

    if (input.size() < size)
    {
        return token_eof;
    }

    current.range = input_range(input.begin(), input.begin() + size);
    input += size;

    return token_raw8;
}

token decoder::next_raw8()
{
    ++input; // Skip token

    const std::size_t size = sizeof(protoc::uint8_t);
    if (input.size() < size)
    {
        return token_eof;
    }

    current.type = token_uint8;
    current.range = input_range(input.begin(), input.begin() + size);

    protoc::uint8_t length = get_uint8();
    input += size;

    if (input.size() < length)
    {
        return token_eof;
    }

    current.range = input_range(input.begin(), input.begin() + length);
    input += length;

    return token_raw8;
}

token decoder::next_raw16()
{
    ++input; // Skip token

    const std::size_t size = sizeof(protoc::uint16_t);
    if (input.size() < size)
    {
        return token_eof;
    }

    current.type = token_uint16;
    current.range = input_range(input.begin(), input.begin() + size);

    protoc::uint16_t length = get_uint16();
    input += size;

    if (input.size() < length)
    {
        return token_eof;
    }

    current.range = input_range(input.begin(), input.begin() + length);
    input += length;

    return token_raw16;
}

token decoder::next_raw32()
{
    ++input; // Skip token

    const std::size_t size = sizeof(protoc::uint32_t);
    if (input.size() < size)
    {
        return token_eof;
    }

    current.type = token_uint32;
    current.range = input_range(input.begin(), input.begin() + size);

    protoc::uint32_t length = get_uint32();
    input += size;

    if (input.size() < length)
    {
        return token_eof;
    }

    current.range = input_range(input.begin(), input.begin() + length);
    input += length;

    return token_raw32;
}

token decoder::next_array8()
{
    current.range = input_range(input.begin(), input.begin() + 1);

    ++input; // Skip token

    return token_array8;
}

token decoder::next_array16()
{
    ++input; // Skip token

    const std::size_t size = sizeof(protoc::uint16_t);
    if (input.size() < size)
    {
        return token_eof;
    }

    current.range = input_range(input.begin(), input.begin() + size);
    input += size;

    return token_array16;
}

token decoder::next_array32()
{
    ++input; // Skip token

    const std::size_t size = sizeof(protoc::uint32_t);
    if (input.size() < size)
    {
        return token_eof;
    }

    current.range = input_range(input.begin(), input.begin() + size);
    input += size;

    return token_array32;
}

} // namespace detail
} // namespace msgpack
} // namespace protoc
