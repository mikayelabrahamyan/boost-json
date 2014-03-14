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

#include <limits>
#include <protoc/msgpack/detail/codes.hpp>
#include <protoc/msgpack/detail/encoder.hpp>

namespace protoc
{
namespace msgpack
{
namespace detail
{

encoder::encoder(output& buffer)
    : buffer(buffer)
{
};

std::size_t encoder::put()
{
    const output::value_type type(code_null);
    const std::size_t size = sizeof(type);

    if (!buffer.grow(size))
    {
        return 0;
    }

    buffer.write(type);

    return size;
}

std::size_t encoder::put(bool value)
{
    const std::size_t size = sizeof(output::value_type);

    if (!buffer.grow(size))
    {
        return 0;
    }

    buffer.write((value) ? code_true : code_false);

    return size;
}

std::size_t encoder::put(int value)
{
    if ((value <= std::numeric_limits<protoc::int8_t>::max()) &&
        (value >= std::numeric_limits<protoc::int8_t>::min()))
    {
        return put_int8(value);
    }
    else if ((value <= std::numeric_limits<protoc::int16_t>::max()) &&
             (value >= std::numeric_limits<protoc::int16_t>::min()))
    {
        return put_int16(value);
    }
    else if ((value <= std::numeric_limits<protoc::int32_t>::max()) &&
             (value >= std::numeric_limits<protoc::int32_t>::min()))
    {
        return put_int32(value);
    }
    else
    {
        return put_int64(value);
    }
}

std::size_t encoder::put_int8(protoc::int8_t value)
{
    if ( ((value >= 0) && (value <= 127)) ||
         ((value >= -32) && (value <= -1)) )
    {
        const std::size_t size = sizeof(output::value_type);

        if (!buffer.grow(size))
        {
            return 0;
        }

        buffer.write(value);
        return size;
    }
    else
    {
        const output::value_type type(code_int8);
        const std::size_t size = sizeof(type) + sizeof(protoc::int8_t);

        if (!buffer.grow(size))
        {
            return 0;
        }

        buffer.write(type);
        buffer.write(value);
        return size;
    }
}

std::size_t encoder::put_int16(protoc::int16_t value)
{
    const output::value_type type(code_int16);
    const std::size_t size = sizeof(type) + sizeof(protoc::int16_t);

    if (!buffer.grow(size))
    {
        return 0;
    }

    buffer.write(type);
    buffer.write(value);
    return size;
}

std::size_t encoder::put_int32(protoc::int32_t value)
{
    const output::value_type type(code_int32);
    const std::size_t size = sizeof(type) + sizeof(protoc::int32_t);

    if (!buffer.grow(size))
    {
        return 0;
    }

    buffer.write(type);
    buffer.write(value);
    return size;
}

std::size_t encoder::put_int64(protoc::int64_t value)
{
    const output::value_type type(code_int64);
    const std::size_t size = sizeof(type) + sizeof(protoc::int64_t);

    if (!buffer.grow(size))
    {
        return 0;
    }

    buffer.write(type);
    buffer.write(value);
    return size;
}

} // namespace detail
} // namespace msgpack
} // namespace protoc
