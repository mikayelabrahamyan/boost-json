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
#include <cstring> // std::memcmp
#include <cstdlib> // std::atoll, std::atof
#include <sstream>
#include <protoc/json/decoder.hpp>

// http://www.ietf.org/rfc/rfc4627.txt

//-----------------------------------------------------------------------------

#if defined(_MSC_VER) && (_MSC_VER < 1200)
#define PROTOC_ATOLL(x) _atoi64((x))
#else
#define PROTOC_ATOLL(x) std::atoll((x))
#endif

namespace
{

const char false_text[] = "false";
const char true_text[] = "true";
const char null_text[] = "null";

const unsigned int lookup_invalid = 0x08;
const unsigned int lookup_whitespace = 0x10;
const unsigned int lookup_keyword = 0x20;
const unsigned int lookup_digit = 0x40;
const unsigned int lookup_hex = 0x80;

const unsigned int lookup_count_mask = 0x07;

const unsigned char lookup[256] =
{
    /* 0 - 7 */
    lookup_invalid,
    lookup_invalid,
    lookup_invalid,
    lookup_invalid,
    lookup_invalid,
    lookup_invalid,
    lookup_invalid,
    lookup_invalid,
    /* 8 - 15 */
    lookup_invalid,
    lookup_whitespace,
    lookup_whitespace,
    lookup_whitespace,
    lookup_invalid,
    lookup_whitespace,
    lookup_invalid,
    lookup_invalid,
    /* 16 - 23 */
    lookup_invalid,
    lookup_invalid,
    lookup_invalid,
    lookup_invalid,
    lookup_invalid,
    lookup_invalid,
    lookup_invalid,
    lookup_invalid,
    /* 24 - 31 */
    lookup_invalid,
    lookup_invalid,
    lookup_invalid,
    lookup_invalid,
    lookup_invalid,
    lookup_invalid,
    lookup_invalid,
    lookup_invalid,
    /* 32 - 39 */
    lookup_whitespace,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* 40 - 47 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* 48 - 55 */
    lookup_digit | lookup_hex,
    lookup_digit | lookup_hex,
    lookup_digit | lookup_hex,
    lookup_digit | lookup_hex,
    lookup_digit | lookup_hex,
    lookup_digit | lookup_hex,
    lookup_digit | lookup_hex,
    lookup_digit | lookup_hex,
    /* 56 - 63 */
    lookup_digit | lookup_hex,
    lookup_digit | lookup_hex,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* 64 - 71 */
    0x00,
    lookup_keyword | lookup_hex,
    lookup_keyword | lookup_hex,
    lookup_keyword | lookup_hex,
    lookup_keyword | lookup_hex,
    lookup_keyword | lookup_hex,
    lookup_keyword | lookup_hex,
    lookup_keyword,
    /* 72 - 79 */
    lookup_keyword,
    lookup_keyword,
    lookup_keyword,
    lookup_keyword,
    lookup_keyword,
    lookup_keyword,
    lookup_keyword,
    lookup_keyword,
    /* 80 - 87 */
    lookup_keyword,
    lookup_keyword,
    lookup_keyword,
    lookup_keyword,
    lookup_keyword,
    lookup_keyword,
    lookup_keyword,
    lookup_keyword,
    /* 88 - 95 */
    lookup_keyword,
    lookup_keyword,
    lookup_keyword,
    0x00, 0x00, 0x00, 0x00, 0x00,
    /* 96 - 103 */
    0x00,
    lookup_keyword | lookup_hex,
    lookup_keyword | lookup_hex,
    lookup_keyword | lookup_hex,
    lookup_keyword | lookup_hex,
    lookup_keyword | lookup_hex,
    lookup_keyword | lookup_hex,
    lookup_keyword,
    /* 104 - 111 */
    lookup_keyword,
    lookup_keyword,
    lookup_keyword,
    lookup_keyword,
    lookup_keyword,
    lookup_keyword,
    lookup_keyword,
    lookup_keyword,
    /* 112 - 119 */
    lookup_keyword,
    lookup_keyword,
    lookup_keyword,
    lookup_keyword,
    lookup_keyword,
    lookup_keyword,
    lookup_keyword,
    lookup_keyword,
    /* 120 - 127 */
    lookup_keyword,
    lookup_keyword,
    lookup_keyword,
    0x00, 0x00, 0x00, 0x00, 0x00,
    /* 128 - */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* 192 */
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x04, 0x04, 0x04, 0x04, 0x05, 0x05, 0x05, 0x05
};

inline unsigned char lookup_at(const protoc::json::detail::decoder::value_type& value)
{
    const int index = static_cast<int>(static_cast<unsigned char>(value));
    return lookup[index];
}

inline bool is_whitespace(const protoc::json::detail::decoder::value_type& value)
{
    return ((lookup_at(value) & lookup_whitespace) == lookup_whitespace);
}

inline bool is_digit(const protoc::json::detail::decoder::value_type& value)
{
    return ((lookup_at(value) & lookup_digit) == lookup_digit);
}

inline bool is_hex(const protoc::json::detail::decoder::value_type& value)
{
    return ((lookup_at(value) & lookup_hex) == lookup_hex);
}

inline bool is_hexdigit(const protoc::json::detail::decoder::value_type& value)
{
    return (lookup_at(value) & (lookup_digit | lookup_hex));
}

inline bool is_keyword(const protoc::json::detail::decoder::value_type& value)
{
    return ((lookup_at(value) & lookup_keyword) == lookup_keyword);
}

inline int extra_bytes(const protoc::json::detail::decoder::value_type& value)
{
    return (lookup_at(value) & lookup_count_mask);
}

} // anonymous namespace

//-----------------------------------------------------------------------------

namespace protoc
{
namespace json
{
namespace detail
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

    skip_whitespaces();

    if (input.empty())
    {
        current.type = token_eof;
        return;
    }

    switch (*input)
    {
    case 'f':
        current.type = next_f_keyword();
        break;

    case 'n':
        current.type = next_n_keyword();
        break;

    case 't':
        current.type = next_t_keyword();
        break;

    case '-':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        current.type = next_number();
        break;

    case '"':
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

    case ',':
        current.type = token_comma;
        ++input;
        break;

    case ':':
        current.type = token_colon;
        ++input;
        break;

    default:
        current.type = token_error;
        break;
    }
}

std::string decoder::get_string() const
{
    assert(current.type == token_string);

    // FIXME: Validate string [ http://www.w3.org/International/questions/qa-forms-utf-8 ]
    std::ostringstream result;
    for (input_range::const_iterator it = current.range.begin();
         it != current.range.end();
         ++it)
    {
        if (*it == '\\')
        {
            assert(current.range.size() >= 2);
            ++it;
            switch (*it)
            {
            case '"':
            case '\\':
            case '/':
                result << *it;
                break;

            case 'b':
                result << '\b';
                break;

            case 'f':
                result << '\f';
                break;

            case 'n':
                result << '\n';
                break;

            case 'r':
                result << '\r';
                break;

            case 't':
                result << '\t';
                break;

            case 'u':
                {
                    // Convert U+XXXX value to UTF-8
                    assert(current.range.size() >= 5);
                    protoc::uint32_t value = 0;
                    for (int i = 0; i < 4; ++i)
                    {
                        ++it;
                        value <<= 4;
                        if (is_digit(*it))
                        {
                            value += (*it - '0');
                        }
                        else if (is_hex(*it))
                        {
                            value += (*it - 'A') + 10;
                        }
                    }
                    if (value <= 0x007F)
                    {
                        // 0xxxxxxx
                        const unsigned char byte1 = static_cast<unsigned char>(value & 0x7F);
                        result << byte1;
                    }
                    else if (value <= 0x07FF)
                    {
                        // 110xxxxx 10xxxxxx
                        const unsigned char byte1 = 0xC0 | static_cast<unsigned char>((value >> 6) & 0x1F);
                        const unsigned char byte2 = 0x80 | static_cast<unsigned char>(value & 0x3F);
                        result << byte1 << byte2;
                    }
                    else
                    {
                        // 1110xxxx 10xxxxxx 10xxxxxx
                        const unsigned char byte1 = 0xE0 | static_cast<unsigned char>((value >> 12) & 0x0F);
                        const unsigned char byte2 = 0x80 | static_cast<unsigned char>((value >> 6) & 0x3F);
                        const unsigned char byte3 = 0x80 | static_cast<unsigned char>(value & 0x3F);
                        result << byte1 << byte2 << byte3;
                    }
                }
                break;

            default:
                assert(false);
                break;
            }
        }
        else
        {
            result << *it;
        }
    }
    return result.str();
}

protoc::int64_t decoder::get_integer() const
{
    assert(current.type == token_integer);

    return PROTOC_ATOLL(current.range.begin());
}

protoc::float64_t decoder::get_float() const
{
    assert(current.type == token_float);

    return std::atof(current.range.begin());
}

token decoder::next_f_keyword()
{
    assert(*input == 'f');

    ++input; // Skip 'f'

    const std::size_t size = sizeof(false_text) - sizeof('f') - sizeof('\0');
    if (input.size() < size)
    {
        return token_eof;
    }

    if (std::memcmp(input.begin(), false_text + 1, size) != 0)
    {
        return token_error;
    }

    input += size;
    return (at_keyword_end()) ? token_false : token_error;
}

token decoder::next_n_keyword()
{
    assert(*input == 'n');

    ++input; // Skip 'n'

    const std::size_t size = sizeof(null_text) - sizeof('n') - sizeof('\0');
    if (input.size() < size)
    {
        return token_eof;
    }

    if (std::memcmp(input.begin(), null_text + 1, size) != 0)
    {
        return token_error;
    }

    input += size;
    return (at_keyword_end()) ? token_null : token_error;
}

token decoder::next_t_keyword()
{
    assert(*input == 't');

    ++input; // Skip 't'

    const std::size_t size = sizeof(true_text) - sizeof('t') - sizeof('\0');
    if (input.size() < size)
    {
        return token_eof;
    }

    if (std::memcmp(input.begin(), true_text + 1, size) != 0)
    {
        return token_error;
    }

    input += size;
    return (at_keyword_end()) ? token_true : token_error;
}

token decoder::next_number()
{
    input_range::const_iterator begin = input.begin();

    const bool is_negative = (*input == '-');
    if (is_negative)
    {
        ++input; // Skip '-'
        if (input.empty())
        {
            return token_eof;
        }
    }

    input_range::const_iterator digit_begin = input.begin();
    while (is_digit(*input))
    {
        ++input;
    }
    if (input.begin() == digit_begin)
    {
        return token_error;
    }
    token type = token_integer;
    if (*input == '.')
    {
        type = token_float;
        ++input;
        if (input.empty())
            return token_eof;
        input_range::const_iterator fraction_begin = input.begin();
        while (is_digit(*input))
        {
            ++input;
        }
        if (input.begin() == fraction_begin)
        {
            return token_error;
        }
    }
    if ((*input == 'E') || (*input == 'e'))
    {
        type = token_float;
        ++input;
        if (input.empty())
            return token_eof;

        if (*input == '+')
        {
            ++input;
            if (input.empty())
                return token_eof;
        }
        else if (*input == '-')
        {
            ++input;
            if (input.empty())
                return token_eof;
        }
        input_range::const_iterator exponent_begin = input.begin();
        while (is_digit(*input))
        {
            ++input;
        }
        if (input.begin() == exponent_begin)
        {
            return token_error;
        }
    }
    current.range = input_range(begin, input.begin());
    return type;
}

token decoder::next_string()
{
    assert(*input == '"');

    ++input; // Skip initial '"'

    input_range::const_iterator begin = input.begin();
    input_range::const_iterator last = input.end();
    input_range::const_iterator first = begin;
    while (first <= last)
    {
        const input_range::size_type amount = extra_bytes(*first);

        if (amount > 0)
        {
            // Skip UTF-8 characters

            if ((first - begin) <= amount)
                goto error;

            ++first;

            for (input_range::size_type i = 0; i < amount; ++i)
            {
                // Check for 10xxxxxx pattern of subsequent bytes
                if ((*first & 0xC0) != 0x80)
                    return token_error;
                ++first;
            }
        }
        else
        {
            if (*first == '\\')
            {
                // Handle escaped character
                ++first;
                if (first == last)
                    goto eof;
                switch (*first)
                {
                case '"':
                case '\\':
                case '/':
                case 'b':
                case 'f':
                case 'n':
                case 'r':
                case 't':
                    break;

                case 'u':
                    ++first;
                    switch (last - first)
                    {
                    case 0:
                        goto eof;
                    case 1:
                        if (!is_hexdigit(first[0]))
                            goto error;
                        goto eof;
                    case 2:
                        if (!is_hexdigit(first[0]))
                            goto error;
                        if (!is_hexdigit(first[1]))
                            goto error;
                        goto eof;
                    case 3:
                        if (!is_hexdigit(first[0]))
                            goto error;
                        if (!is_hexdigit(first[1]))
                            goto error;
                        if (!is_hexdigit(first[2]))
                            goto error;
                        goto eof;
                    default:
                        break;
                    }
                    if (!is_hexdigit(first[0]))
                        goto error;
                    if (!is_hexdigit(first[1]))
                        goto error;
                    if (!is_hexdigit(first[2]))
                        goto error;
                    if (!is_hexdigit(first[3]))
                        goto error;
                    break;

                default:
                    goto error;
                }
            }
            else if (*first == '"')
            {
                // Handle end of string
                current.range = input_range(begin, first);
                input += (first - begin) + 1; // Skip terminating '"'
                return token_string;
            }
            ++first;
        }
    }
 eof:
    return token_eof;

 error:
    input += (first - begin);
    return token_error;
}

void decoder::skip_whitespaces()
{
    while (!input.empty() && is_whitespace(*input))
    {
        ++input;
    }
}

bool decoder::at_keyword_end() const
{
    if (input.empty())
    {
        return true;
    }
    return !is_keyword(*input);
}

} // namespace detail
} // namespace json
} // namespace protoc
