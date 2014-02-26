#ifndef PROTOC_TRANSENC_VECTOR_HPP
#define PROTOC_TRANSENC_VECTOR_HPP

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

#include <protoc/transenc/serialization.hpp>
#include <protoc/serialization/vector.hpp>

namespace boost
{
namespace serialization
{

// Specialization of std::vector<unsigned char> for binary data

template <typename Allocator>
struct save_functor< protoc::transenc::oarchive, typename std::vector<unsigned char, Allocator> >
{
    void operator () (protoc::transenc::oarchive& ar,
                      const std::vector<unsigned char, Allocator>& data,
                      const unsigned int version)
    {
        ar.save(data.data(), data.size());
    }
};

template <typename Allocator>
struct load_functor< protoc::transenc::iarchive, typename std::vector<unsigned char, Allocator> >
{
    void operator () (protoc::transenc::iarchive& ar,
                      std::vector<unsigned char, Allocator>& data,
                      const unsigned int version)
    {
        typename std::vector<unsigned char, Allocator>::size_type size = ar.load_binary_begin();
        if (size == 0)
        {
            data.clear();
        }
        else
        {
            data.resize(size);
            ar.load(data.data(), size);
        }
    }
};

} // namespace serialization
} // namespace boost

#endif // PROTOC_TRANSENC_VECTOR_HPP
