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

#include <sstream>
#include <boost/none.hpp>
#include <boost/range/iterator_range.hpp>
#include <protoc/exceptions.hpp>
#include <protoc/msgpack/reader.hpp>

namespace protoc
{
namespace msgpack
{

reader::reader(const reader& other)
    : decoder(other.decoder)
{
}

protoc::token::value reader::type() const
{
    if (!stack.empty())
    {
        // Handle end of containers
        stack_type::const_reference top = stack.top();
        if (top.count == 0)
        {
            switch (top.token)
            {
            case protoc::token::token_array_end:
            case protoc::token::token_map_end:
                return top.token;

            default:
                break;
            }
        }
    }

    const detail::token current = decoder.type();
    switch (current)
    {
     case detail::token_eof:
        return protoc::token::token_eof;

    case detail::token_null:
        return protoc::token::token_null;

    case detail::token_true:
    case detail::token_false:
        return protoc::token::token_boolean;

    case detail::token_int8:
    case detail::token_uint8:
    case detail::token_int16:
    case detail::token_uint16:
    case detail::token_int32:
    case detail::token_uint32:
    case detail::token_int64:
    case detail::token_uint64:
        return protoc::token::token_integer;

    case detail::token_float32:
    case detail::token_float64:
        return protoc::token::token_floating;

    case detail::token_str8:
    case detail::token_str16:
    case detail::token_str32:
        return protoc::token::token_string;

    case detail::token_array8:
    case detail::token_array16:
    case detail::token_array32:
        return protoc::token::token_array_begin;

    case detail::token_map8:
    case detail::token_map16:
    case detail::token_map32:
        return protoc::token::token_map_begin;

    default:
        std::ostringstream error;
        error << current;
        throw unexpected_token(error.str());
    }
}

reader::size_type reader::size() const
{
    return stack.size();
}

bool reader::next()
{
    const detail::token current = decoder.type();
    switch (current)
    {
    case detail::token_array8:
    case detail::token_array16:
    case detail::token_array32:
        stack.push(frame(protoc::token::token_array_begin, decoder.get_count()));
        break;

    case detail::token_map8:
    case detail::token_map16:
    case detail::token_map32:
        stack.push(frame(protoc::token::token_map_begin, 2 * decoder.get_count()));
        break;

    default:
        break;
    }

    decoder.next();

    if (!stack.empty())
    {
        stack_type::reference top = stack.top();
        switch (top.token)
        {
        case protoc::token::token_array_begin:
            if (top.count == 0)
            {
                top.token = protoc::token::token_array_end;
                return true;
            }
            else
            {
                --(top.count);
            }
            break;

        case protoc::token::token_array_end:
            stack.pop();
            if (!stack.empty())
            {
                stack_type::reference parent = stack.top();
                parent.token = protoc::token::token_array_end;
                return true;
            }
            break;

        case protoc::token::token_map_begin:
            if (top.count == 0)
            {
                top.token = protoc::token::token_map_end;
                return true;
            }
            else
            {
                --(top.count);
            }
            break;

        case protoc::token::token_map_end:
            stack.pop();
            if (!stack.empty())
            {
                stack_type::reference parent = stack.top();
                parent.token = protoc::token::token_map_end;
                return true;
            }
            break;

        default:
            break;
        }
    }

    return (type() != protoc::token::token_eof);
}

bool reader::next(protoc::token::value expect)
{
    const protoc::token::value current = type();
    if (current != expect)
    {
        std::ostringstream error;
        error << current;
        throw unexpected_token(error.str());
    }
    return next();
}

void reader::next_sibling()
{
    throw unexpected_token("FIXME");
}

bool reader::get_bool() const
{
    const detail::token current = decoder.type();
    switch (current)
    {
    case detail::token_true:
        return true;

    case detail::token_false:
        return false;

    default:
        std::ostringstream error;
        error << current;
        throw invalid_value(error.str());
    }
}

int reader::get_int() const
{
    const detail::token current = decoder.type();
    switch (current)
    {
    case detail::token_int8:
        return decoder.get_int8();

    case detail::token_uint8:
        return decoder.get_uint8();

    case detail::token_int16:
        return decoder.get_int16();

    case detail::token_uint16:
        return decoder.get_uint16();

    case detail::token_int32:
        return decoder.get_int32();

    case detail::token_uint32:
        return decoder.get_uint32();

    case detail::token_int64:
    case detail::token_uint64:
        // FIXME: Use numeric_limits<T>::max() to check if value will fit
        // FIXME: But fall-through for now
    default:
        std::ostringstream error;
        error << current;
        throw invalid_value(error.str());
    }
}

long long reader::get_long_long() const
{
    const detail::token current = decoder.type();
    switch (current)
    {
    case detail::token_int8:
        return decoder.get_int8();

    case detail::token_uint8:
        return decoder.get_uint8();

    case detail::token_int16:
        return decoder.get_int16();

    case detail::token_uint16:
        return decoder.get_uint16();

    case detail::token_int32:
        return decoder.get_int32();

    case detail::token_uint32:
        return decoder.get_uint32();

    case detail::token_int64:
        return decoder.get_int64();

    case detail::token_uint64:
        return decoder.get_uint64();

    default:
        std::ostringstream error;
        error << current;
        throw invalid_value(error.str());
    }
}

double reader::get_double() const
{
    const detail::token current = decoder.type();
    switch (current)
    {
    case detail::token_float32:
        return decoder.get_float32();

    case detail::token_float64:
        return decoder.get_float64();

    default:
        std::ostringstream error;
        error << current;
        throw invalid_value(error.str());
    }
}

std::string reader::get_string() const
{
    const detail::token current = decoder.type();
    switch (current)
    {
    case detail::token_str8:
    case detail::token_str16:
    case detail::token_str32:
        return decoder.get_string();

    default:
        std::ostringstream error;
        error << current;
        throw invalid_value(error.str());
    }
}

reader::range_type reader::get_range() const
{
    detail::decoder::input_range range = decoder.get_range();
    return boost::make_iterator_range(range.begin(), range.end());
}

reader::frame::frame(protoc::token::value token, size_type count)
    : token(token),
      count(count)
{
}

} // namespace msgpack
} // namespace protoc
