#ifndef PROTOC_UBJSON_ENCODER_HPP
#define PROTOC_UBJSON_ENCODER_HPP

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
#include <protoc/output_range.hpp>

namespace protoc
{
namespace ubjson
{

class encoder
{
public:
    encoder(char *begin, char *end);

    std::size_t put(); // Null
    std::size_t put(bool);
    std::size_t put(protoc::int8_t);
    std::size_t put(protoc::int16_t);
    std::size_t put(protoc::int32_t);
    std::size_t put(protoc::int64_t);
    std::size_t put(protoc::float32_t);
    std::size_t put(protoc::float64_t);
    std::size_t put(const char *);
    std::size_t put(const std::string&);

private:
    output_range output;
};

}
}

#endif /* PROTOC_UBJSON_ENCODER_HPP */
