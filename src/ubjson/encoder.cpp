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
#include <algorithm> // std::copy
#include <boost/math/special_functions/fpclassify.hpp>
#include <protoc/ubjson/encoder.hpp>

namespace protoc
{
namespace ubjson
{

encoder::encoder(output& buffer)
    : buffer(buffer)
{
};

std::size_t encoder::capacity() const
{
    return sizeof('Z');
}

std::size_t encoder::put()
{
    const std::size_t size = capacity();

    if (!buffer.grow(size))
    {
        return 0;
    }

    buffer.write('Z');

    return size;
}

std::size_t encoder::put(bool value)
{
    const std::size_t size = sizeof('T');

    if (!buffer.grow(size))
    {
        return 0;
    }

    buffer.write((value) ? 'T' : 'F');

    return size;
}

std::size_t encoder::put(protoc::int8_t value)
{
    const output::value_type type('B');
    const std::size_t size = sizeof(type) + sizeof(protoc::int8_t);

    if (!buffer.grow(size))
    {
        return 0;
    }

    buffer.write(type);
    buffer.write(static_cast<output::value_type>(value));

    return size;
}

std::size_t encoder::put(protoc::int16_t value)
{
    const output::value_type type('i');
    const std::size_t size = sizeof(type) + sizeof(protoc::int16_t);

    if (!buffer.grow(size))
    {
        return 0;
    }

    buffer.write(type);
    buffer.write(static_cast<output::value_type>((value >> 8) & 0xFF));
    buffer.write(static_cast<output::value_type>(value & 0xFF));

    return size;
}

std::size_t encoder::put(protoc::int32_t value)
{
    const output::value_type type('I');
    const std::size_t size = sizeof(type) + sizeof(protoc::int32_t);

    if (!buffer.grow(size))
    {
        return 0;
    }

    buffer.write(type);
    buffer.write(static_cast<output::value_type>((value >> 24) & 0xFF));
    buffer.write(static_cast<output::value_type>((value >> 16) & 0xFF));
    buffer.write(static_cast<output::value_type>((value >> 8) & 0xFF));
    buffer.write(static_cast<output::value_type>(value & 0xFF));

    return size;
}

std::size_t encoder::put(protoc::int64_t value)
{
    const output::value_type type('L');
    const std::size_t size = sizeof(type) + sizeof(protoc::int64_t);

    if (!buffer.grow(size))
    {
        return 0;
    }

    buffer.write(type);
    buffer.write(static_cast<output::value_type>((value >> 54) & 0xFF));
    buffer.write(static_cast<output::value_type>((value >> 48) & 0xFF));
    buffer.write(static_cast<output::value_type>((value >> 40) & 0xFF));
    buffer.write(static_cast<output::value_type>((value >> 32) & 0xFF));
    buffer.write(static_cast<output::value_type>((value >> 24) & 0xFF));
    buffer.write(static_cast<output::value_type>((value >> 16) & 0xFF));
    buffer.write(static_cast<output::value_type>((value >> 8) & 0xFF));
    buffer.write(static_cast<output::value_type>(value & 0xFF));

    return size;
}

std::size_t encoder::put(float value)
{
    const int fpclass = boost::math::fpclassify(value);
    if ((fpclass == FP_INFINITE) || (fpclass == FP_NAN))
    {
        // Infinity and NaN must be encoded as null
        return put();
    }

    const output::value_type type('d');
    const std::size_t size = sizeof(type) + sizeof(float);

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

std::size_t encoder::put(double value)
{
    const int fpclass = boost::math::fpclassify(value);
    if ((fpclass == FP_INFINITE) || (fpclass == FP_NAN))
    {
        // Infinity and NaN must be encoded as null
        return put();
    }


    const output::value_type type('D');
    const std::size_t size = sizeof(type) + sizeof(double);

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
    const output::value_type type('s');
    const std::string::size_type length = value.size();
    std::size_t size = 0;

    if (length < static_cast<std::string::size_type>(std::numeric_limits<protoc::int8_t>::max()))
    {
        if (buffer.grow(sizeof(type) + sizeof('B') + sizeof(protoc::int8_t) + length))
        {
            buffer.write(type);
            size = put(static_cast<int8_t>(length));
            if (size > 0)
                goto success;
        }
    }
    else if (length < static_cast<std::string::size_type>(std::numeric_limits<protoc::int16_t>::max()))
    {
        if (buffer.grow(sizeof(type) + sizeof('i') + sizeof(protoc::int16_t) + length))
        {
            buffer.write(type);
            size = put(static_cast<int16_t>(length));
            if (size > 0)
                goto success;
        }
    }
    else if (length < static_cast<std::string::size_type>(std::numeric_limits<protoc::int32_t>::max()))
    {
        if (buffer.grow(sizeof(type) + sizeof('I') + sizeof(protoc::int32_t) + length))
        {
            buffer.write(type);
            size = put(static_cast<int32_t>(length));
            if (size > 0)
                goto success;
            
        }
    }
    else
    {
        if (buffer.grow(sizeof(type) + sizeof('L') + sizeof(protoc::int64_t) + length))
        {
            buffer.write(type);
            size = put(static_cast<int64_t>(length));
            if (size > 0)
                goto success;
        }
    }
    return 0;

 success:
    for (std::string::const_iterator it = value.begin(); it != value.end(); ++it)
    {
        buffer.write(*it);
    }

    return sizeof(type) + size + length;
}

}
}
