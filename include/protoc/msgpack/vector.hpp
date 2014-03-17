#ifndef PROTOC_MSGPACK_VECTOR_HPP
#define PROTOC_MSGPACK_VECTOR_HPP

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

#include <protoc/msgpack/serialization.hpp>
#include <protoc/serialization/vector.hpp>

namespace boost
{
namespace serialization
{

// Specialization of std::vector<unsigned char> for binary data

template <typename Allocator>
struct save_functor< protoc::msgpack::oarchive, typename std::vector<unsigned char, Allocator> >
{
    void operator () (protoc::msgpack::oarchive& ar,
                      const std::vector<unsigned char, Allocator>& data,
                      const unsigned int version)
    {
        ar.save(data.data(), data.size());
    }
};

template <typename Allocator>
struct load_functor< protoc::msgpack::iarchive, typename std::vector<unsigned char, Allocator> >
{
    void operator () (protoc::msgpack::iarchive& ar,
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

#endif // PROTOC_MSGPACK_VECTOR_HPP
