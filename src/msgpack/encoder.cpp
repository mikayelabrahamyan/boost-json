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

#include <limits>
#include <algorithm> // std::copy
#include <protoc/msgpack/encoder.hpp>

namespace protoc
{
namespace msgpack
{

encoder::encoder(char *begin, char *end)
    : output(begin, end)
{
};

std::size_t encoder::put()
{
    const output_range::value_type type = '\xC0';
    const std::size_t size = sizeof(type);

    if (output.size() < size)
    {
        return 0;
    }

    *output = type;
    ++output;

    return size;
}

std::size_t encoder::put(bool value)
{
    const std::size_t size = sizeof(output_range::value_type);

    if (output.size() < size)
    {
        return 0;
    }

    *output = (value) ? '\xC3' : '\xC2';
    ++output;

    return size;
}

}
}
