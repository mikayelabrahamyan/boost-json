#ifndef PROTOC_JSON_ENCODER_HPP
#define PROTOC_JSON_ENCODER_HPP

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
namespace json
{

class encoder
{
public:
    encoder(output&);

    std::size_t put(); // Null
    std::size_t put(bool);
    std::size_t put(protoc::int64_t);
    std::size_t put(protoc::float64_t);
    std::size_t put(const char *);
    std::size_t put(const std::string&);

    std::size_t put_object_begin();
    std::size_t put_object_end();
    std::size_t put_array_begin();
    std::size_t put_array_end();

private:
    std::size_t put_text(const char *, std::size_t);

private:
    output& buffer;
    bool need_whitespace;
};

} // namespace json
} // namespace protoc

#endif // PROTOC_JSON_ENCODER_HPP
