#ifndef PROTOC_MSGPACK_MAP_HPP
#define PROTOC_MSGPACK_MAP_HPP

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
#include <protoc/msgpack/pair.hpp>
#include <protoc/serialization/map.hpp>

namespace boost
{
namespace serialization
{

template <typename Key, typename T, typename Compare, typename Allocator>
struct save_functor< protoc::msgpack::oarchive, typename std::map<Key, T, Compare, Allocator> >
{
    void operator () (protoc::msgpack::oarchive& ar,
                      const std::map<Key, T, Compare, Allocator>& data,
                      const unsigned int version)
    {
        ar.save_map_begin(data.size());
        for (typename std::map<Key, T, Compare, Allocator>::const_iterator it = data.begin();
             it != data.end();
             ++it)
        {
            ar.save_override(*it, version);
        }
        ar.save_map_end();
    }
};

} // namespace serialization
} // namespace boost

#endif // PROTOC_MSGPACK_MAP_HPP
