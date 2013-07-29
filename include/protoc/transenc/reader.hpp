#ifndef PROTOC_TRANSENC_READER_HPP
#define PROTOC_TRANSENC_READER_HPP

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

#include <stack>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_floating_point.hpp>
#include <protoc/transenc/token.hpp>
#include <protoc/transenc/decoder.hpp>

namespace protoc
{
namespace transenc
{

class reader
{
public:
    enum token
    {
        token_eof,
        token_null,
        token_boolean,
        token_integer,
        token_floating,
        token_string,
        token_binary,

        token_record_begin,
        token_record_end,
        token_array_begin,
        token_array_end,
        token_map_begin,
        token_map_end
    };

    template <typename ForwardIterator>
    reader(ForwardIterator begin, ForwardIterator end);
    reader(const reader&);

    token type() const;

    token next();
    token next(token);
    token next_sibling();

    // Get boolean value
    // Usage: get<bool>
    template <typename T>
    typename boost::enable_if<boost::is_same<T, bool>, T>::type
    get() const;

    // Get integer number
    // Usage: get<int> or get<long long>
    template <typename T>
    typename boost::enable_if_c<boost::is_integral<T>::value &&
                                !boost::is_same<T, bool>::value, T>::type
    get() const;

    // Get floating-point number
    // Usage: get<float> or get<double>
    template <typename T>
    typename boost::enable_if<boost::is_floating_point<T>, T>::type
    get() const;

    // Get string
    // Usage: get<std::string>
    template <typename T>
    typename boost::enable_if<boost::is_same<T, std::string>, T>::type
    get() const;

private:
    transenc::decoder decoder;
    std::stack<transenc::token> stack;
};

} // namespace transenc
} // namespace protoc

#include <sstream>
#include <protoc/exceptions.hpp>

namespace protoc
{
namespace transenc
{

template <typename ForwardIterator>
reader::reader(ForwardIterator begin, ForwardIterator end)
    : decoder(begin, end)
{
}

reader::reader(const reader& other)
    : decoder(other.decoder)
{
}

reader::token reader::type() const
{
    const transenc::token current = decoder.type();
    switch (current)
    {
    case transenc::token_null:
        return token_null;

    case transenc::token_true:
    case transenc::token_false:
        return token_boolean;

    case transenc::token_int8:
    case transenc::token_int16:
    case transenc::token_int32:
    case transenc::token_int64:
        return token_integer;

    case transenc::token_float32:
    case transenc::token_float64:
        return token_floating;

    case transenc::token_string:
        return token_string;

    case transenc::token_binary:
        return token_binary;

    case transenc::token_record_begin:
        return token_record_begin;

    case transenc::token_record_end:
        return token_record_end;

    case transenc::token_array_begin:
        return token_array_begin;

    case transenc::token_array_end:
        return token_array_end;

    case transenc::token_map_begin:
        return token_map_begin;

    case transenc::token_map_end:
        return token_map_end;

    case transenc::token_eof:
        return token_eof;

    case transenc::token_error:
        throw unexpected_token("token_error");

    default:
        std::ostringstream error;
        error << current;
        throw unexpected_token(error.str());
    }
}

// FIXME: Add next_if(token) ?
reader::token reader::next()
{
    const transenc::token current = decoder.type();
    switch (current)
    {
    case transenc::token_record_begin:
        stack.push(transenc::token_record_end);
        break;

    case transenc::token_array_begin:
        stack.push(transenc::token_array_end);
        break;

    case transenc::token_map_begin:
        stack.push(transenc::token_map_end);
        break;

    default:
        break;
    }

    decoder.next();

    switch (current)
    {
    case transenc::token_record_end:
        if (stack.top() != transenc::token_record_end)
        {
            throw unexpected_token("expected record end");
        }
        stack.pop();
        break;

    case transenc::token_array_end:
        if (stack.top() != transenc::token_array_end)
        {
            throw unexpected_token("expected array end");
        }
        stack.pop();
        break;

    case transenc::token_map_end:
        if (stack.top() != transenc::token_map_end)
        {
            throw unexpected_token("expected map end");
        }
        stack.pop();
        break;

    default:
        break;
    }

    return type();
}

reader::token reader::next(token expect)
{
    const token current = type();
    if (current != expect)
    {
        std::ostringstream error;
        error << current;
        throw unexpected_token(error.str());
    }
    return next();
}

reader::token reader::next_sibling()
{
    // FIXME: Skip over children
    throw unexpected_token("not implemented");
}

template <typename T>
typename boost::enable_if<boost::is_same<T, bool>, T>::type
reader::get() const
{
    const transenc::token current = decoder.type();
    switch (current)
    {
    case transenc::token_true:
        return true;

    case transenc::token_false:
        return false;

    default:
        std::ostringstream error;
        error << current;
        throw invalid_value(error.str());
    }
}

template <typename T>
typename boost::enable_if_c<boost::is_integral<T>::value && !boost::is_same<T, bool>::value, T>::type
reader::get() const
{
    // FIXME: Use numeric_limits<T>::max() to check if value will fit
    const transenc::token current = decoder.type();
    switch (current)
    {
    case transenc::token_int8:
        return decoder.get_int8();

    case transenc::token_int16:
        return decoder.get_int16();

    case transenc::token_int32:
        return decoder.get_int32();

    case transenc::token_int64:
        return decoder.get_int64();

    default:
        std::ostringstream error;
        error << current;
        throw invalid_value(error.str());
    }
}

template <typename T>
typename boost::enable_if<boost::is_floating_point<T>, T>::type
reader::get() const
{
    // FIXME: Use numeric_limits<T>::max() to check if value will fit
    const transenc::token current = decoder.type();
    switch (current)
    {
    case transenc::token_float32:
        return decoder.get_float32();

    case transenc::token_float64:
        return decoder.get_float64();

    default:
        std::ostringstream error;
        error << current;
        throw invalid_value(error.str());
    }
}

template <typename T>
typename boost::enable_if<boost::is_same<T, std::string>, T>::type
reader::get() const
{
    const transenc::token current = decoder.type();
    switch (current)
    {
    case transenc::token_string:
        return decoder.get_string();

    default:
        std::ostringstream error;
        error << current;
        throw invalid_value(error.str());
    }
}

} // namespace transenc
} // namespace protoc

#endif // PROTOC_TRANSENC_READER_HPP
