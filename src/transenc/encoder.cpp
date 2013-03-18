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
#include <protoc/transenc/encoder.hpp>

namespace protoc
{
namespace transenc
{

encoder::encoder(output& buffer)
    : buffer(buffer)
{
};

std::size_t encoder::put()
{
    const output::value_type type('\x82');
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

    buffer.write((value) ? '\x81' : '\x80');

    return size;
}

std::size_t encoder::put(protoc::int8_t value)
{
    if (value >= -32)
    {
        const std::size_t size = sizeof(output::value_type);

        if (!buffer.grow(size))
        {
            return 0;
        }

        write(value);
        return size;
    }
    else
    {
        const output::value_type type('\xA0');
        const std::size_t size = sizeof(type) + sizeof(protoc::int8_t);

        if (!buffer.grow(size))
        {
            return 0;
        }

        buffer.write(type);
        write(value);
        return size;
    }
}

std::size_t encoder::put(protoc::int16_t value)
{
    const output::value_type type('\xB0');
    const std::size_t size = sizeof(type) + sizeof(protoc::int16_t);

    if (!buffer.grow(size))
    {
        return 0;
    }

    buffer.write(type);
    write(value);

    return size;
}

std::size_t encoder::put(protoc::int32_t value)
{
    const output::value_type type('\xC0');
    const std::size_t size = sizeof(type) + sizeof(protoc::int32_t);

    if (!buffer.grow(size))
    {
        return 0;
    }

    buffer.write(type);
    write(value);

    return size;
}

std::size_t encoder::put(protoc::int64_t value)
{
    const output::value_type type('\xD0');
    const std::size_t size = sizeof(type) + sizeof(protoc::int64_t);

    if (!buffer.grow(size))
    {
        return 0;
    }

    buffer.write(type);
    write(value);

    return size;
}

std::size_t encoder::put(protoc::float32_t value)
{
    const output::value_type type('\xC2');
    const std::size_t size = sizeof(type) + sizeof(protoc::float32_t);

    if (!buffer.grow(size))
    {
        return 0;
    }

    buffer.write(type);
    // IEEE 754 single precision
    const protoc::int32_t ix = 0x00010203;
    protoc::int8_t *value_buffer = (protoc::int8_t *)&value;
    buffer.write(static_cast<output::value_type>(value_buffer[((protoc::int8_t *)&ix)[0]]));
    buffer.write(static_cast<output::value_type>(value_buffer[((protoc::int8_t *)&ix)[1]]));
    buffer.write(static_cast<output::value_type>(value_buffer[((protoc::int8_t *)&ix)[2]]));
    buffer.write(static_cast<output::value_type>(value_buffer[((protoc::int8_t *)&ix)[3]]));

    return size;
}

std::size_t encoder::put(protoc::float64_t value)
{
    const output::value_type type('\xD2');
    const std::size_t size = sizeof(type) + sizeof(protoc::float64_t);

    if (!buffer.grow(size))
    {
        return 0;
    }

    buffer.write(type);
    // IEEE 754 double precision
    const protoc::int64_t ix = 0x0001020304050607;
    protoc::int8_t *value_buffer = (protoc::int8_t *)&value;
    buffer.write(static_cast<output::value_type>(value_buffer[((protoc::int8_t *)&ix)[0]]));
    buffer.write(static_cast<output::value_type>(value_buffer[((protoc::int8_t *)&ix)[1]]));
    buffer.write(static_cast<output::value_type>(value_buffer[((protoc::int8_t *)&ix)[2]]));
    buffer.write(static_cast<output::value_type>(value_buffer[((protoc::int8_t *)&ix)[3]]));
    buffer.write(static_cast<output::value_type>(value_buffer[((protoc::int8_t *)&ix)[4]]));
    buffer.write(static_cast<output::value_type>(value_buffer[((protoc::int8_t *)&ix)[5]]));
    buffer.write(static_cast<output::value_type>(value_buffer[((protoc::int8_t *)&ix)[6]]));
    buffer.write(static_cast<output::value_type>(value_buffer[((protoc::int8_t *)&ix)[7]]));

    return size;
}

std::size_t encoder::put(const char *value)
{
    return put(std::string(value));
}

std::size_t encoder::put(const std::string& value)
{
    const std::string::size_type length = value.size();

    std::size_t size = 0;

    if (length < static_cast<std::string::size_type>(std::numeric_limits<protoc::int8_t>::max()))
    {
        if (!buffer.grow(sizeof(output::value_type) + sizeof(protoc::int8_t) + length))
        {
            return 0;
        }
        buffer.write('\xA9');
        size = write(static_cast<int8_t>(length));
    }
    else if (length < static_cast<std::string::size_type>(std::numeric_limits<protoc::int16_t>::max()))
    {
        if (!buffer.grow(sizeof(output::value_type) + sizeof(protoc::int16_t) + length))
        {
            return 0;
        }
        buffer.write('\xB9');
        size = write(static_cast<int16_t>(length));
    }
    else if (length < static_cast<std::string::size_type>(std::numeric_limits<protoc::int32_t>::max()))
    {
        if (!buffer.grow(sizeof(output::value_type) + sizeof(protoc::int32_t) + length))
        {
            return 0;
        }
        buffer.write('\xC9');
        size = write(static_cast<int32_t>(length));
    }
    else
    {
        if (!buffer.grow(sizeof(output::value_type) + sizeof(protoc::int64_t) + length))
        {
            return 0;
        }
        buffer.write('\xD9');
        size = write(static_cast<int64_t>(length));
    }

    for (std::string::const_iterator it = value.begin(); it != value.end(); ++it)
    {
        buffer.write(*it);
    }

    return sizeof(output::value_type) + size + length;
}

std::size_t encoder::write(protoc::int8_t value)
{
    buffer.write(static_cast<output::value_type>(value));
    return sizeof(protoc::int8_t);
}

std::size_t encoder::write(protoc::int16_t value)
{
    buffer.write(static_cast<output::value_type>((value >> 8) & 0xFF));
    buffer.write(static_cast<output::value_type>(value & 0xFF));
    return sizeof(protoc::int16_t);
}

std::size_t encoder::write(protoc::int32_t value)
{
    buffer.write(static_cast<output::value_type>((value >> 24) & 0xFF));
    buffer.write(static_cast<output::value_type>((value >> 16) & 0xFF));
    buffer.write(static_cast<output::value_type>((value >> 8) & 0xFF));
    buffer.write(static_cast<output::value_type>(value & 0xFF));
    return sizeof(protoc::int32_t);
}

std::size_t encoder::write(protoc::int64_t value)
{
    buffer.write(static_cast<output::value_type>((value >> 54) & 0xFF));
    buffer.write(static_cast<output::value_type>((value >> 48) & 0xFF));
    buffer.write(static_cast<output::value_type>((value >> 40) & 0xFF));
    buffer.write(static_cast<output::value_type>((value >> 32) & 0xFF));
    buffer.write(static_cast<output::value_type>((value >> 24) & 0xFF));
    buffer.write(static_cast<output::value_type>((value >> 16) & 0xFF));
    buffer.write(static_cast<output::value_type>((value >> 8) & 0xFF));
    buffer.write(static_cast<output::value_type>(value & 0xFF));
    return sizeof(protoc::int64_t);
}

}
}
