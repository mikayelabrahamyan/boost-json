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

encoder::encoder(output& buffer)
    : buffer(buffer),
      need_whitespace(false)
{
};

std::size_t encoder::put()
{
    std::size_t size = put_text(null_text, sizeof(null_text));
    need_whitespace = true;
    return size;
}

std::size_t encoder::put(bool value)
{
    std::size_t size;
    if (value)
    {
        size = put_text(true_text, sizeof(true_text));
    }
    else
    {
        size = put_text(false_text, sizeof(false_text));
    }
    need_whitespace = true;
    return size;
}

std::size_t encoder::put(protoc::int64_t value)
{
    put_whitespace();

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

    need_whitespace = true;

    return size;
}

std::size_t encoder::put(protoc::float64_t value)
{
    put_whitespace();

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

    need_whitespace = true;

    return size;
}

std::size_t encoder::put_whitespace()
{
    if (need_whitespace)
    {
        if (buffer.grow(1))
        {
            buffer.write(' ');
            return 1;
        }
        need_whitespace = false;
    }
    return 0;
}

std::size_t encoder::put_text(const char *value, std::size_t size)
{
    put_whitespace();

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

} // namespace json
} // namespace protoc
