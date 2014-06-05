#ifndef PROTOC_IMPL_OUTPUT_CONTAINER_IPP
#define PROTOC_IMPL_OUTPUT_CONTAINER_IPP

///////////////////////////////////////////////////////////////////////////////
//
// http://protoc.sourceforge.net/
//
// Copyright (C) 2014 Bjorn Reese <breese@users.sourceforge.net>
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

namespace protoc
{

template <typename Value, template <typename, typename = std::allocator<Value> > class Container>
bool output_container<Value, Container>::grow(size_type delta)
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

template <typename Value, template <typename, typename = std::allocator<Value> > class Container>
void output_container<Value, Container>::write(value_type value)
{
    buffer.push_back(value);
}

template <typename Value, template <typename, typename = std::allocator<Value> > class Container>
void output_container<Value, Container>::write(const value_type *values,
                                               size_type size)
{
    if (grow(size))
    {
        for (size_type i = 0; i < size; ++i)
        {
            write(values[i]);
        }
    }
}

} // namespace protoc

#endif // PROTOC_IMPL_OUTPUT_CONTAINER_IPP
