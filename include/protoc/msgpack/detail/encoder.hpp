#ifndef PROTOC_MSGPACK_DETAIL_ENCODER_HPP
#define PROTOC_MSGPACK_DETAIL_ENCODER_HPP

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

#include <string>
#include <protoc/types.hpp>
#include <protoc/output.hpp>

namespace protoc
{
namespace msgpack
{
namespace detail
{

class encoder
{
public:
    typedef unsigned char value_type;
    typedef protoc::output<value_type> output;

    encoder(output&);

    std::size_t put(); // Null
    std::size_t put(bool);
    std::size_t put(int);
    std::size_t put(unsigned int);
    std::size_t put(protoc::int64_t);
    std::size_t put(protoc::uint64_t);
    std::size_t put(protoc::float32_t);
    std::size_t put(protoc::float64_t);
    std::size_t put(const char *);
    std::size_t put(const std::string&);
    std::size_t put(const unsigned char *, std::size_t);

    std::size_t put_array_begin(std::size_t);

private:
    std::size_t put_token(output::value_type);
    std::size_t put_int8(protoc::int8_t);
    std::size_t put_uint8(protoc::uint8_t);
    std::size_t put_int16(protoc::int16_t);
    std::size_t put_uint16(protoc::uint16_t);
    std::size_t put_int32(protoc::int32_t);
    std::size_t put_uint32(protoc::uint32_t);
    std::size_t put_int64(protoc::int64_t);
    std::size_t put_uint64(protoc::uint64_t);

    std::size_t write(protoc::int8_t);
    std::size_t write(protoc::uint8_t);
    std::size_t write(protoc::int16_t);
    std::size_t write(protoc::uint16_t);
    std::size_t write(protoc::int32_t);
    std::size_t write(protoc::uint32_t);
    std::size_t write(protoc::int64_t);
    std::size_t write(protoc::uint64_t);
    std::size_t write(protoc::float32_t);
    std::size_t write(protoc::float64_t);

private:
    output& buffer;
};

} // namespace detail
} // namespace msgpack
} // namespace protoc

#endif /* PROTOC_MSGPACK_DETAIL_ENCODER_HPP */
