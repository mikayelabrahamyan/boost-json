#ifndef PROTOC_MSGPACK_READER_HPP
#define PROTOC_MSGPACK_READER_HPP

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

#include <stack>
#include <boost/optional.hpp>
#include <protoc/reader.hpp>
#include <protoc/token.hpp>
#include <protoc/msgpack/detail/token.hpp>
#include <protoc/msgpack/detail/decoder.hpp>

namespace protoc
{
namespace msgpack
{

class reader : public protoc::reader
{
public:
    template <typename ForwardIterator>
    reader(ForwardIterator begin, ForwardIterator end);
    reader(const reader&);

    virtual protoc::token::value type() const;
    virtual size_type size() const;

    virtual bool next();
    virtual bool next(protoc::token::value);
    virtual void next_sibling();

    virtual bool get_bool() const;
    virtual int get_int() const;
    virtual long long get_long_long() const;
    virtual double get_double() const;
    virtual std::string get_string() const;
    virtual range_type get_range() const;

private:
    msgpack::detail::decoder decoder;
    struct element
    {
        element(protoc::token::value token, size_type count)
            : token(token),
              count(count)
        {}

        protoc::token::value token;
        size_type count;
    };
    typedef std::stack<element> stack_type;
    stack_type stack;
};

} // namespace msgpack
} // namespace protoc

#include <sstream>
#include <boost/none.hpp>
#include <boost/range/iterator_range.hpp>
#include <protoc/exceptions.hpp>

namespace protoc
{
namespace msgpack
{

template <typename ForwardIterator>
reader::reader(ForwardIterator begin, ForwardIterator end)
    : decoder(begin, end)
{
}

inline reader::reader(const reader& other)
    : decoder(other.decoder)
{
}

inline protoc::token::value reader::type() const
{
    if (!stack.empty())
    {
        // Handle end of containers
        stack_type::const_reference top = stack.top();
        if ((top.count == 0) && (top.token == protoc::token::token_array_end))
        {
            return top.token;
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

    // FIXME: etc.

    default:
        std::ostringstream error;
        error << current;
        throw unexpected_token(error.str());
    }
}

inline reader::size_type reader::size() const
{
    return stack.size();
}

inline bool reader::next()
{
    const detail::token current = decoder.type();
    switch (current)
    {
    case detail::token_array8:
        stack.push(element(protoc::token::token_array_begin, decoder.get_uint8()));
        break;

    case detail::token_array16:
        stack.push(element(protoc::token::token_array_begin, decoder.get_uint16()));
        break;

    case detail::token_array32:
        stack.push(element(protoc::token::token_array_begin, decoder.get_uint32()));
        break;

    // FIXME: map

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

        default:
            break;
        }
    }

    return (type() != protoc::token::token_eof);
}

inline bool reader::next(protoc::token::value expect)
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

inline void reader::next_sibling()
{
    throw unexpected_token("FIXME");
}

inline bool reader::get_bool() const
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

inline int reader::get_int() const
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

inline long long reader::get_long_long() const
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

inline double reader::get_double() const
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

inline std::string reader::get_string() const
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

inline reader::range_type reader::get_range() const
{
    detail::decoder::input_range range = decoder.get_range();
    return boost::make_iterator_range(range.begin(), range.end());
}

} // namespace msgpack
} // namespace protoc

#endif // PROTOC_MSGPACK_READER_HPP
