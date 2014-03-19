#ifndef PROTOC_MSGPACK_WRITER_HPP
#define PROTOC_MSGPACK_WRITER_HPP

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
#include <protoc/writer.hpp>
#include <protoc/token.hpp>
#include <protoc/msgpack/detail/encoder.hpp>

namespace protoc
{
namespace msgpack
{

class writer : public protoc::writer
{
    typedef msgpack::detail::encoder encoder_type;

public:
    typedef encoder_type::output_type output_type;

    writer(output_type&);

    virtual size_type size();

    virtual size_type write(); // Null
    virtual size_type write(bool);
    virtual size_type write(int);
    virtual size_type write(long long);
    virtual size_type write(float);
    virtual size_type write(double);
    virtual size_type write(const char *);
    virtual size_type write(const std::string&);
    virtual size_type write(const value_type *, size_type);

    virtual size_type record_begin();
    virtual size_type record_end();

    virtual size_type array_begin();
    virtual size_type array_begin(size_type count);
    virtual size_type array_end();

    virtual size_type map_begin();
    virtual size_type map_begin(size_type count);
    virtual size_type map_end();

private:
    size_type track(size_type);

private:
    encoder_type encoder;
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

#endif // PROTOC_MSGPACK_WRITER_HPP
