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
#include <protoc/json/decoder.hpp>

//-----------------------------------------------------------------------------

namespace
{

const char false_text[] = "false";
const char true_text[] = "true";
const char null_text[] = "null";

const unsigned int lookup_invalid = 0x01;
const unsigned int lookup_whitespace = 0x02;
const unsigned int lookup_keyword = 0x04;
const unsigned int lookup_digit = 0x08;

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
    lookup_digit,
    lookup_digit,
    lookup_digit,
    lookup_digit,
    lookup_digit,
    lookup_digit,
    lookup_digit,
    lookup_digit,
    /* 56 - 63 */
    lookup_digit,
    lookup_digit,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* 64 - 71 */
    0x00,
    lookup_keyword,
    lookup_keyword,
    lookup_keyword,
    lookup_keyword,
    lookup_keyword,
    lookup_keyword,
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
    lookup_keyword,
    lookup_keyword,
    lookup_keyword,
    lookup_keyword,
    lookup_keyword,
    lookup_keyword,
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
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

} // anonymous namespace

//-----------------------------------------------------------------------------

namespace protoc
{
namespace json
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
    return token_error;
}

token decoder::next_string()
{
    return token_error;
}

void decoder::skip_whitespaces()
{
    while (!input.empty() && is_whitespace())
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
    const int ix = static_cast<int>(*input);
    if ((lookup[ix] & lookup_keyword))
    {
        return false;
    }
    return true;
}

bool decoder::is_whitespace() const
{
    const int ix = static_cast<int>(*input);
    return ((lookup[ix] & lookup_whitespace) == lookup_whitespace);
}

bool decoder::is_digit() const
{
    const int ix = static_cast<int>(*input);
    return ((lookup[ix] & lookup_digit) == lookup_digit);
}

}
}
