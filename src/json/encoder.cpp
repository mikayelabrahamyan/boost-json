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
#include <limits>
#include <protoc/json/encoder.hpp>

namespace
{

char digits_array[] = { '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

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

    // Conversion for optimal performance

    // FIXME: Change to binary search
    const std::size_t count = (value < 0L
                               ? ((value > -10L ? 1 :
                                   (value > -100L ? 2 :
                                    (value > -1000L ? 3 :
                                     (value > -10000L ? 4 :
                                      (value > -100000L ? 5 :
                                       (value > -1000000L ? 6 :
                                        (value > -10000000L ? 7 :
                                         (value > -100000000L ? 8 :
                                          (value > -1000000000L ? 9 :
                                           (value > -10000000000L ? 10 :
                                            (value > -100000000000L ? 11 :
                                             (value > -1000000000000L ? 12 :
                                              (value > -10000000000000L ? 13 :
                                               (value > -100000000000000L ? 14 :
                                                (value > -1000000000000000L ? 15 :
                                                 (value > -10000000000000000L ? 16 :
                                                  (value > -100000000000000000L ? 17 :
                                                   (value > -1000000000000000000L ? 18 :
                                                    19)))))))))))))))))))
                               : ((value < 10L ? 1 :
                                   (value < 100L ? 2 :
                                    (value < 1000L ? 3 :
                                     (value < 10000L ? 4 :
                                      (value < 100000L ? 5 :
                                       (value < 1000000L ? 6 :
                                        (value < 10000000L ? 7 :
                                         (value < 100000000L ? 8 :
                                          (value < 1000000000L ? 9 :
                                           (value < 10000000000L ? 10 :
                                            (value < 100000000000L ? 11 :
                                             (value < 1000000000000L ? 12 :
                                              (value < 10000000000000L ? 13 :
                                               (value < 100000000000000L ? 14 :
                                                (value < 1000000000000000L ? 15 :
                                                 (value < 10000000000000000L ? 16 :
                                                  (value < 100000000000000000L ? 17 :
                                                   (value < 1000000000000000000L ? 18 :
                                                    19))))))))))))))))))));
    assert(count <= static_cast<const std::size_t>(std::numeric_limits<protoc::int64_t>::digits10) + 1);
    const bool is_negative = (value < 0);
    const std::size_t size = count + (is_negative ? 1 : 0);

    if (!buffer.grow(size))
    {
        return 0;
    }

    const char *digits = digits_array + sizeof(digits_array) / 2; // Point to the middle '0'
    char reversed[count];
    for (std::size_t i = 0; i < count; ++i)
    {
        reversed[i] = digits[value % 10];
        value /= 10;
    }

    if (is_negative)
    {
        buffer.write('-');
    }
    for (std::size_t i = 1; i <= count; ++i)
    {
        buffer.write(reversed[count - i]);
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
