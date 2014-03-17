///////////////////////////////////////////////////////////////////////////////
//
// http://protoc.sourceforge.net/
//
// Copyright (C) 2014 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
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

encoder::encoder(output_type& buffer)
    : buffer(buffer)
{
};

std::size_t encoder::put()
{
    return put_token(code_null);
}

std::size_t encoder::put(bool value)
{
    return put_token((value) ? code_true : code_false);
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

std::size_t encoder::put(unsigned int value)
{
    if ((value <= std::numeric_limits<protoc::uint8_t>::max()) &&
        (value >= std::numeric_limits<protoc::uint8_t>::min()))
    {
        return put_uint8(value);
    }
    else if ((value <= std::numeric_limits<protoc::uint16_t>::max()) &&
             (value >= std::numeric_limits<protoc::uint16_t>::min()))
    {
        return put_uint16(value);
    }
    else if ((value <= std::numeric_limits<protoc::uint32_t>::max()) &&
             (value >= std::numeric_limits<protoc::uint32_t>::min()))
    {
        return put_uint32(value);
    }
    else
    {
        return put_uint64(value);
    }
}

std::size_t encoder::put(protoc::int64_t value)
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

std::size_t encoder::put(protoc::uint64_t value)
{
    if ((value <= std::numeric_limits<protoc::uint8_t>::max()) &&
        (value >= std::numeric_limits<protoc::uint8_t>::min()))
    {
        return put_uint8(value);
    }
    else if ((value <= std::numeric_limits<protoc::uint16_t>::max()) &&
             (value >= std::numeric_limits<protoc::uint16_t>::min()))
    {
        return put_uint16(value);
    }
    else if ((value <= std::numeric_limits<protoc::uint32_t>::max()) &&
             (value >= std::numeric_limits<protoc::uint32_t>::min()))
    {
        return put_uint32(value);
    }
    else
    {
        return put_uint64(value);
    }
}

std::size_t encoder::put(protoc::float32_t value)
{
    const value_type type(code_float32);
    const std::size_t size = sizeof(type) + sizeof(protoc::float32_t);

    if (!buffer.grow(size))
    {
        return 0;
    }

    buffer.write(type);
    write(value);
    return size;
}

std::size_t encoder::put(protoc::float64_t value)
{
    const value_type type(code_float64);
    const std::size_t size = sizeof(type) + sizeof(protoc::float64_t);

    if (!buffer.grow(size))
    {
        return 0;
    }

    buffer.write(type);
    write(value);
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

    if (length <= (code_fixstr_31 - code_fixstr_0))
    {
        if (!buffer.grow(sizeof(value_type)))
        {
            return 0;
        }
        buffer.write(code_fixstr_0 | length);
        size = 0;
    }
    else if (length < static_cast<std::string::size_type>(std::numeric_limits<protoc::uint8_t>::max()))
    {
        if (!buffer.grow(sizeof(value_type) + sizeof(protoc::uint8_t) + length))
        {
            return 0;
        }
        buffer.write(code_str8);
        size = write(static_cast<uint8_t>(length));
    }
    else if (length < static_cast<std::string::size_type>(std::numeric_limits<protoc::uint16_t>::max()))
    {
        if (!buffer.grow(sizeof(value_type) + sizeof(protoc::uint16_t) + length))
        {
            return 0;
        }
        buffer.write(code_str16);
        size = write(static_cast<uint16_t>(length));
    }
    else if (length < static_cast<std::string::size_type>(std::numeric_limits<protoc::uint32_t>::max()))
    {
        if (!buffer.grow(sizeof(value_type) + sizeof(protoc::uint32_t) + length))
        {
            return 0;
        }
        buffer.write(code_str32);
        size = write(static_cast<uint32_t>(length));
    }
    else
    {
        return 0;
    }

    for (std::string::const_iterator it = value.begin(); it != value.end(); ++it)
    {
        buffer.write(*it);
    }

    return sizeof(value_type) + size + length;
}

std::size_t encoder::put(const unsigned char * value, std::size_t length)
{
    std::size_t size = 0;

    if (length < static_cast<std::string::size_type>(std::numeric_limits<protoc::uint8_t>::max()))
    {
        if (!buffer.grow(sizeof(value_type) + sizeof(protoc::uint8_t) + length))
        {
            return 0;
        }
        buffer.write(code_bin8);
        size = write(static_cast<uint8_t>(length));
    }
    else if (length < static_cast<std::string::size_type>(std::numeric_limits<protoc::uint16_t>::max()))
    {
        if (!buffer.grow(sizeof(value_type) + sizeof(protoc::uint16_t) + length))
        {
            return 0;
        }
        buffer.write(code_bin16);
        size = write(static_cast<uint16_t>(length));
    }
    else if (length < static_cast<std::string::size_type>(std::numeric_limits<protoc::uint32_t>::max()))
    {
        if (!buffer.grow(sizeof(value_type) + sizeof(protoc::uint32_t) + length))
        {
            return 0;
        }
        buffer.write(code_bin32);
        size = write(static_cast<uint32_t>(length));
    }
    else
    {
        return 0;
    }

    for (std::size_t i = 0; i < length; ++i)
    {
        buffer.write(value[i]);
    }

    return sizeof(value_type) + size + length;
}

std::size_t encoder::put_array_begin(std::size_t count)
{
    switch (count)
    {
    case 0:
        return put_token(code_fixarray_0);

    case 1:
        return put_token(code_fixarray_1);

    case 2:
        return put_token(code_fixarray_2);

    case 3:
        return put_token(code_fixarray_3);

    case 4:
        return put_token(code_fixarray_4);

    case 5:
        return put_token(code_fixarray_5);

    case 6:
        return put_token(code_fixarray_6);

    case 7:
        return put_token(code_fixarray_7);

    case 8:
        return put_token(code_fixarray_8);

    case 9:
        return put_token(code_fixarray_9);

    case 10:
        return put_token(code_fixarray_10);

    case 11:
        return put_token(code_fixarray_11);

    case 12:
        return put_token(code_fixarray_12);

    case 13:
        return put_token(code_fixarray_13);

    case 14:
        return put_token(code_fixarray_14);

    case 15:
        return put_token(code_fixarray_15);

    default:
        if (count <= 0xFFFF)
        {
            const std::size_t size = sizeof(value_type) + sizeof(protoc::uint16_t);
            if (!buffer.grow(size))
            {
                return 0;
            }
            buffer.write(code_array16);
            write(protoc::uint16_t(count));
            return size;
        }
        else
        {
            const std::size_t size = sizeof(value_type) + sizeof(protoc::uint32_t);
            if (!buffer.grow(size))
            {
                return 0;
            }
            buffer.write(code_array32);
            write(protoc::uint32_t(count));
            return size;
        }
    }

    return 0;
}

std::size_t encoder::put_map_begin(std::size_t count)
{
    switch (count)
    {
    case 0:
        return put_token(code_fixmap_0);

    case 1:
        return put_token(code_fixmap_1);

    case 2:
        return put_token(code_fixmap_2);

    case 3:
        return put_token(code_fixmap_3);

    case 4:
        return put_token(code_fixmap_4);

    case 5:
        return put_token(code_fixmap_5);

    case 6:
        return put_token(code_fixmap_6);

    case 7:
        return put_token(code_fixmap_7);

    case 8:
        return put_token(code_fixmap_8);

    case 9:
        return put_token(code_fixmap_9);

    case 10:
        return put_token(code_fixmap_10);

    case 11:
        return put_token(code_fixmap_11);

    case 12:
        return put_token(code_fixmap_12);

    case 13:
        return put_token(code_fixmap_13);

    case 14:
        return put_token(code_fixmap_14);

    case 15:
        return put_token(code_fixmap_15);

    default:
        if (count <= 0xFFFF)
        {
            const std::size_t size = sizeof(value_type) + sizeof(protoc::uint16_t);
            if (!buffer.grow(size))
            {
                return 0;
            }
            buffer.write(code_map16);
            write(protoc::uint16_t(count));
            return size;
        }
        else
        {
            const std::size_t size = sizeof(value_type) + sizeof(protoc::uint32_t);
            if (!buffer.grow(size))
            {
                return 0;
            }
            buffer.write(code_map32);
            write(protoc::uint32_t(count));
            return size;
        }
    }

    return 0;
}

std::size_t encoder::put_token(value_type value)
{
    const std::size_t size = sizeof(value);

    if (!buffer.grow(size))
    {
        return 0;
    }

    buffer.write(value);

    return size;
}

std::size_t encoder::put_int8(protoc::int8_t value)
{
    if (value >= -32)
    {
        const std::size_t size = sizeof(value_type);

        if (!buffer.grow(size))
        {
            return 0;
        }

        write(value);
        return size;
    }
    else
    {
        const value_type type(code_int8);
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

std::size_t encoder::put_uint8(protoc::uint8_t value)
{
    if (value <= 127)
    {
        const std::size_t size = sizeof(value_type);

        if (!buffer.grow(size))
        {
            return 0;
        }

        write(value);
        return size;
    }
    else
    {
        const value_type type(code_uint8);
        const std::size_t size = sizeof(type) + sizeof(protoc::uint8_t);

        if (!buffer.grow(size))
        {
            return 0;
        }

        buffer.write(type);
        write(value);
        return size;
    }
}

std::size_t encoder::put_int16(protoc::int16_t value)
{
    const value_type type(code_int16);
    const std::size_t size = sizeof(type) + sizeof(protoc::int16_t);

    if (!buffer.grow(size))
    {
        return 0;
    }

    buffer.write(type);
    write(value);
    return size;
}

std::size_t encoder::put_uint16(protoc::uint16_t value)
{
    const value_type type(code_uint16);
    const std::size_t size = sizeof(type) + sizeof(protoc::uint16_t);

    if (!buffer.grow(size))
    {
        return 0;
    }

    buffer.write(type);
    write(value);
    return size;
}

std::size_t encoder::put_int32(protoc::int32_t value)
{
    const value_type type(code_int32);
    const std::size_t size = sizeof(type) + sizeof(protoc::int32_t);

    if (!buffer.grow(size))
    {
        return 0;
    }

    buffer.write(type);
    write(value);
    return size;
}

std::size_t encoder::put_uint32(protoc::uint32_t value)
{
    const value_type type(code_uint32);
    const std::size_t size = sizeof(type) + sizeof(protoc::uint32_t);

    if (!buffer.grow(size))
    {
        return 0;
    }

    buffer.write(type);
    write(value);
    return size;
}

std::size_t encoder::put_int64(protoc::int64_t value)
{
    const value_type type(code_int64);
    const std::size_t size = sizeof(type) + sizeof(protoc::int64_t);

    if (!buffer.grow(size))
    {
        return 0;
    }

    buffer.write(type);
    write(value);
    return size;
}

std::size_t encoder::put_uint64(protoc::uint64_t value)
{
    const value_type type(code_uint64);
    const std::size_t size = sizeof(type) + sizeof(protoc::uint64_t);

    if (!buffer.grow(size))
    {
        return 0;
    }

    buffer.write(type);
    write(value);
    return size;
}

std::size_t encoder::write(protoc::int8_t value)
{
    buffer.write(static_cast<value_type>(value));
    return sizeof(protoc::int8_t);
}

std::size_t encoder::write(protoc::uint8_t value)
{
    buffer.write(static_cast<value_type>(value));
    return sizeof(protoc::uint8_t);
}

std::size_t encoder::write(protoc::int16_t value)
{
    // Big-endian
    buffer.write(static_cast<value_type>((value >> 8) & 0xFF));
    buffer.write(static_cast<value_type>(value & 0xFF));
    return sizeof(protoc::int16_t);
}

std::size_t encoder::write(protoc::uint16_t value)
{
    buffer.write(static_cast<value_type>((value >> 8) & 0xFF));
    buffer.write(static_cast<value_type>(value & 0xFF));
    return sizeof(protoc::uint16_t);
}

std::size_t encoder::write(protoc::int32_t value)
{
    buffer.write(static_cast<value_type>((value >> 24) & 0xFF));
    buffer.write(static_cast<value_type>((value >> 16) & 0xFF));
    buffer.write(static_cast<value_type>((value >> 8) & 0xFF));
    buffer.write(static_cast<value_type>(value & 0xFF));
    return sizeof(protoc::int32_t);
}

std::size_t encoder::write(protoc::uint32_t value)
{
    buffer.write(static_cast<value_type>((value >> 24) & 0xFF));
    buffer.write(static_cast<value_type>((value >> 16) & 0xFF));
    buffer.write(static_cast<value_type>((value >> 8) & 0xFF));
    buffer.write(static_cast<value_type>(value & 0xFF));
    return sizeof(protoc::uint32_t);
}

std::size_t encoder::write(protoc::int64_t value)
{
    buffer.write(static_cast<value_type>((value >> 56) & 0xFF));
    buffer.write(static_cast<value_type>((value >> 48) & 0xFF));
    buffer.write(static_cast<value_type>((value >> 40) & 0xFF));
    buffer.write(static_cast<value_type>((value >> 32) & 0xFF));
    buffer.write(static_cast<value_type>((value >> 24) & 0xFF));
    buffer.write(static_cast<value_type>((value >> 16) & 0xFF));
    buffer.write(static_cast<value_type>((value >> 8) & 0xFF));
    buffer.write(static_cast<value_type>(value & 0xFF));
    return sizeof(protoc::int64_t);
}

std::size_t encoder::write(protoc::uint64_t value)
{
    buffer.write(static_cast<value_type>((value >> 56) & 0xFF));
    buffer.write(static_cast<value_type>((value >> 48) & 0xFF));
    buffer.write(static_cast<value_type>((value >> 40) & 0xFF));
    buffer.write(static_cast<value_type>((value >> 32) & 0xFF));
    buffer.write(static_cast<value_type>((value >> 24) & 0xFF));
    buffer.write(static_cast<value_type>((value >> 16) & 0xFF));
    buffer.write(static_cast<value_type>((value >> 8) & 0xFF));
    buffer.write(static_cast<value_type>(value & 0xFF));
    return sizeof(protoc::uint64_t);
}

std::size_t encoder::write(protoc::float32_t value)
{
    // Big-endian IEEE 754 single precision
    const protoc::int32_t endian = 0x00010203;
    protoc::int8_t *value_buffer = (protoc::int8_t *)&value;
    buffer.write(static_cast<value_type>(value_buffer[((protoc::int8_t *)&endian)[0]]));
    buffer.write(static_cast<value_type>(value_buffer[((protoc::int8_t *)&endian)[1]]));
    buffer.write(static_cast<value_type>(value_buffer[((protoc::int8_t *)&endian)[2]]));
    buffer.write(static_cast<value_type>(value_buffer[((protoc::int8_t *)&endian)[3]]));
    return sizeof(protoc::float32_t);
}

std::size_t encoder::write(protoc::float64_t value)
{
    // Big-endian IEEE 754 double precision
    const protoc::int64_t endian = 0x0001020304050607;
    protoc::int8_t *value_buffer = (protoc::int8_t *)&value;
    buffer.write(static_cast<value_type>(value_buffer[((protoc::int8_t *)&endian)[0]]));
    buffer.write(static_cast<value_type>(value_buffer[((protoc::int8_t *)&endian)[1]]));
    buffer.write(static_cast<value_type>(value_buffer[((protoc::int8_t *)&endian)[2]]));
    buffer.write(static_cast<value_type>(value_buffer[((protoc::int8_t *)&endian)[3]]));
    buffer.write(static_cast<value_type>(value_buffer[((protoc::int8_t *)&endian)[4]]));
    buffer.write(static_cast<value_type>(value_buffer[((protoc::int8_t *)&endian)[5]]));
    buffer.write(static_cast<value_type>(value_buffer[((protoc::int8_t *)&endian)[6]]));
    buffer.write(static_cast<value_type>(value_buffer[((protoc::int8_t *)&endian)[7]]));
    return sizeof(protoc::float64_t);
}

} // namespace detail
} // namespace msgpack
} // namespace protoc
