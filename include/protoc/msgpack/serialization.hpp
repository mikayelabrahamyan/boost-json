#ifndef PROTOC_MSGPACK_SERIALIZATION_HPP
#define PROTOC_MSGPACK_SERIALIZATION_HPP

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

#include <protoc/msgpack/iarchive.hpp>
#include <protoc/msgpack/oarchive.hpp>
#include <protoc/serialization/serialization.hpp>

namespace boost
{
namespace serialization
{

//-----------------------------------------------------------------------------
// oarchive
//-----------------------------------------------------------------------------

// C++ does not have partial specialization of template functions so we use
// functors to achieve the same effect.

template <typename Value>
void save(protoc::msgpack::oarchive& ar,
          const Value& data,
          const unsigned int version)
{
    save_functor<protoc::msgpack::oarchive, Value>()(ar, data, version);
}

template <typename Value>
void serialize(protoc::msgpack::oarchive& ar,
               const Value& data,
               const unsigned int version)
{
    serialize_functor<Value>()(ar, data, version);
}

template <typename Value>
void serialize(protoc::msgpack::oarchive& ar,
               Value& data,
               const unsigned int version)
{
    serialize_functor<Value>()(ar, data, version);
}

//-----------------------------------------------------------------------------
// iarchive
//-----------------------------------------------------------------------------

template <typename Value>
void load(protoc::msgpack::iarchive& ar,
          Value& data,
          const unsigned int version)
{
    load_functor<protoc::msgpack::iarchive, Value>()(ar, data, version);
}

template <typename Value>
void serialize(protoc::msgpack::iarchive& ar,
               Value& data,
               const unsigned int version)
{
    serialize_functor<Value>()(ar, data, version);
}

} // namespace serialization
} // namespace boost


#endif // PROTOC_MSGPACK_SERIALIZATION_HPP
