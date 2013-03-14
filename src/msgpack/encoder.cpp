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

#include <protoc/msgpack/encoder.hpp>

namespace protoc
{
namespace msgpack
{

encoder::encoder(output& buffer)
    : buffer(buffer)
{
};

std::size_t encoder::put()
{
    const output::value_type type('\xC0');
    const std::size_t size = sizeof(type);

    if (!buffer.grow(size))
    {
        return 0;
    }

    buffer.write(type);

    return size;
}

std::size_t encoder::put(bool value)
{
    const std::size_t size = sizeof(output::value_type);

    if (!buffer.grow(size))
    {
        return 0;
    }

    buffer.write((value) ? '\xC3' : '\xC2');

    return size;
}

std::size_t encoder::put(protoc::int8_t value)
{
    const output::value_type type('\xD0');
    const std::size_t size = sizeof(type) + sizeof(protoc::int8_t);

    if (!buffer.grow(size))
    {
        return 0;
    }

    buffer.write(type);
    buffer.write(static_cast<output::value_type>(value));

    return size;
}

}
}
