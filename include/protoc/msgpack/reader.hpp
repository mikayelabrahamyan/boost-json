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
    struct frame
    {
        frame(protoc::token::value token, size_type count);

        protoc::token::value token;
        size_type count;
    };
    typedef std::stack<frame> stack_type;
    stack_type stack;
};

} // namespace msgpack
} // namespace protoc

namespace protoc
{
namespace msgpack
{

template <typename ForwardIterator>
reader::reader(ForwardIterator begin, ForwardIterator end)
    : decoder(begin, end)
{
}

} // namespace msgpack
} // namespace protoc

#endif // PROTOC_MSGPACK_READER_HPP
