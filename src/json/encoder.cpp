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

#define BOOST_LEXICAL_CAST_ASSUME_C_LOCALE 1
#include <boost/lexical_cast.hpp>
#include <boost/math/special_functions/fpclassify.hpp>
#include <protoc/json/encoder.hpp>

namespace
{

char null_text[] = { 'n', 'u', 'l', 'l' };
char true_text[] = { 't', 'r', 'u', 'e' };
char false_text[] = { 'f', 'a', 'l', 's', 'e' };

} // anonymous namespace

namespace protoc
{
namespace json
{
namespace detail
{

encoder::encoder(output_type& buffer)
    : buffer(buffer)
{
};

std::size_t encoder::put()
{
    return put_text(null_text, sizeof(null_text));
}

std::size_t encoder::put(bool value)
{
    if (value)
    {
        return put_text(true_text, sizeof(true_text));
    }
    else
    {
        return put_text(false_text, sizeof(false_text));
    }
}

std::size_t encoder::put(protoc::int32_t value)
{
    std::string work = boost::lexical_cast<std::string>(value);
    const std::string::size_type size = work.size();

    if (!buffer.grow(size))
    {
        return 0;
    }

    for (std::string::const_iterator it = work.begin();
         it != work.end();
         ++it)
    {
        buffer.write(*it);
    }

    return size;
}

std::size_t encoder::put(protoc::int64_t value)
{
    std::string work = boost::lexical_cast<std::string>(value);
    const std::string::size_type size = work.size();

    if (!buffer.grow(size))
    {
        return 0;
    }

    for (std::string::const_iterator it = work.begin();
         it != work.end();
         ++it)
    {
        buffer.write(*it);
    }

    return size;
}

std::size_t encoder::put(protoc::float32_t value)
{
    return put(double(value));
}

std::size_t encoder::put(protoc::float64_t value)
{
    const int fpclass = boost::math::fpclassify(value);
    if ((fpclass == FP_INFINITE) || (fpclass == FP_NAN))
    {
        // Infinity and NaN must be encoded as null
        return put();
    }

    std::string work = boost::lexical_cast<std::string>(value);
    const std::string::size_type size = work.size();

    if (!buffer.grow(size))
    {
        return 0;
    }

    for (std::string::const_iterator it = work.begin();
         it != work.end();
         ++it)
    {
        buffer.write(*it);
    }

    return size;
}

std::size_t encoder::put(const char *value)
{
    return put(std::string(value));
}

std::size_t encoder::put(const std::string& value)
{
    std::size_t size = sizeof('"') + value.size() + sizeof('"');

    if (!buffer.grow(size))
    {
        return 0;
    }

    put_value('"');
    for (std::string::const_iterator it = value.begin(); it != value.end(); ++it)
    {
        switch (*it)
        {
        case '"':
        case '\\':
        case '/':
            if (!buffer.grow(1))
            {
                return 0;
            }
            ++size;
            buffer.write('\\');
            buffer.write(*it);
            break;

        case '\b':
            if (!buffer.grow(1))
            {
                return 0;
            }
            ++size;
            buffer.write('\\');
            buffer.write('b');
            break;

        case '\f':
            if (!buffer.grow(1))
            {
                return 0;
            }
            ++size;
            buffer.write('\\');
            buffer.write('f');
            break;

        case '\n':
            if (!buffer.grow(1))
            {
                return 0;
            }
            ++size;
            buffer.write('\\');
            buffer.write('n');
            break;

        case '\r':
            if (!buffer.grow(1))
            {
                return 0;
            }
            ++size;
            buffer.write('\\');
            buffer.write('r');
            break;

        case '\t':
            if (!buffer.grow(1))
            {
                return 0;
            }
            ++size;
            buffer.write('\\');
            buffer.write('t');
            break;

        default:
            buffer.write(*it);
            break;
        }
    }
    put_value('"');

    return size;
}

std::size_t encoder::put_record_begin()
{
    return put_value('[');
}

std::size_t encoder::put_record_end()
{
    return put_value(']');
}

std::size_t encoder::put_array_begin()
{
    return put_value('[');
}

std::size_t encoder::put_array_begin(std::size_t)
{
    return put_array_begin();
}

std::size_t encoder::put_array_end()
{
    return put_value(']');
}

std::size_t encoder::put_map_begin()
{
    return put_value('{');
}

std::size_t encoder::put_map_begin(std::size_t)
{
    return put_map_begin();
}

std::size_t encoder::put_map_end()
{
    return put_value('}');
}

std::size_t encoder::put_comma()
{
    return put_value(',');
}

std::size_t encoder::put_colon()
{
    return put_value(':');
}

std::size_t encoder::put_text(const char *value, std::size_t size)
{
    if (!buffer.grow(size))
    {
        return 0;
    }

    for (std::size_t i = 0; i < size; ++i)
    {
        buffer.write(value[i]);
    }

    return size;
}

std::size_t encoder::put_value(output_type::value_type value)
{
    const std::size_t size = sizeof(value);

    if (!buffer.grow(size))
    {
        return 0;
    }

    buffer.write(value);

    return size;
}

} // namespace detail
} // namespace json
} // namespace protoc
