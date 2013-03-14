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

#include <cassert>
#include <protoc/output_vector.hpp>

namespace protoc
{

output_vector::const_iterator output_vector::begin() const
{
    return buffer.begin();
}

output_vector::const_iterator output_vector::end() const
{
    return buffer.end();
}

output_vector::size_type output_vector::size() const
{
    return buffer.size();
}

output_vector::size_type output_vector::capacity() const
{
    return buffer.capacity();
}

output_vector::const_reference output_vector::operator [] (size_type ix) const
{
    assert(ix < buffer.size());
    return buffer[ix];
}

bool output_vector::grow(size_type delta)
{
    const size_type size = buffer.size() + delta;
    if (size > buffer.capacity())
    {
        if (size > buffer.max_size())
            return false;
        buffer.reserve(size);
    }
    return true;
}

void output_vector::write(value_type value)
{
    buffer.push_back(value);
}

}
