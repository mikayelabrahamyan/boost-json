#ifndef PROTOC_OUTPUT_CONTAINER_HPP
#define PROTOC_OUTPUT_CONTAINER_HPP

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

#include <cstddef> // std::size_t
#include <vector>
#include <protoc/output.hpp>

namespace protoc
{

template <typename Value, template <typename, typename = std::allocator<Value> > class Container>
class output_container
    : public output<typename Container<Value>::value_type>
{
public:
    typedef Value value_type;
    typedef typename output< Container<value_type> >::size_type size_type;

    typedef typename Container<value_type>::iterator iterator;
    typedef typename Container<value_type>::const_iterator const_iterator;
    typedef value_type& reference;
    typedef const value_type& const_reference;

public:
    output_container(Container<value_type>& buffer)
        : buffer(buffer)
    {
    }

    const_iterator begin() const { return buffer.begin(); }
    const_iterator end() const { return buffer.end(); }

    size_type size() const { return buffer.size(); }
    size_type capacity() const { return buffer.capacity(); }

    const_reference operator [] (size_type ix) const { return buffer[ix]; }

private:
    // Implementation of protoc::output interface
    virtual bool grow(size_type delta);
    virtual void write(value_type value);
    virtual void write(const value_type *, size_type);

private:
    Container<value_type>& buffer;
};

} // namespace protoc

#include <protoc/impl/output_container.ipp>

#endif /* PROTOC_OUTPUT_CONTAINER_HPP */
