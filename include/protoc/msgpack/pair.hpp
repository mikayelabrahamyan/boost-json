#ifndef PROTOC_MSGPACK_PAIR_HPP
#define PROTOC_MSGPACK_PAIR_HPP

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
#include <protoc/serialization/pair.hpp>

namespace boost
{
namespace serialization
{

template <typename T1, typename T2>
struct save_functor< protoc::msgpack::oarchive, typename std::pair<T1, T2> >
{
    void operator () (protoc::msgpack::oarchive& ar,
                      const std::pair<T1, T2>& data,
                      const unsigned int version)
    {
        ar << data.first;
        ar << data.second;
    }
};

template <typename T1, typename T2>
struct load_functor< protoc::msgpack::iarchive, typename std::pair<T1, T2> >
{
    void operator () (protoc::msgpack::iarchive& ar,
                      std::pair<T1, T2>& data,
                      const unsigned int version)
    {
        ar >> data.first;
        ar >> data.second;
    }
};

} // namespace serialization
} // namespace boost

#endif // PROTOC_MSGPACK_PAIR_HPP
