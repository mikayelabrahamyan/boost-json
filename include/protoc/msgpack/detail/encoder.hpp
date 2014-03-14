#ifndef PROTOC_MSGPACK_DETAIL_ENCODER_HPP
#define PROTOC_MSGPACK_DETAIL_ENCODER_HPP

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
    std::size_t put(protoc::int64_t);
    std::size_t put(unsigned int);
    std::size_t put(protoc::uint64_t);
    std::size_t put(protoc::float32_t);
    std::size_t put(protoc::float64_t);
    std::size_t put(const char *);
    std::size_t put(const std::string&);

private:
    std::size_t put_int8(protoc::int8_t);
    std::size_t put_int16(protoc::int16_t);
    std::size_t put_int32(protoc::int32_t);
    std::size_t put_int64(protoc::int64_t);

private:
    output& buffer;
};

} // namespace detail
} // namespace msgpack
} // namespace protoc

#endif /* PROTOC_MSGPACK_DETAIL_ENCODER_HPP */
