#ifndef PROTOC_JSON_SERIALIZATION_HPP
#define PROTOC_JSON_SERIALIZATION_HPP

#include <protoc/json/iarchive.hpp>
#include <protoc/json/oarchive.hpp>
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
void save(protoc::json::oarchive& ar,
          const Value& data,
          const unsigned int version)
{
    save_functor<protoc::json::oarchive, Value>()(ar, data, version);
}

// Boost.Serialization does not support perfect forwarding so we need two
// overloads (for const and non-const values)

template <typename Value>
void serialize(protoc::json::oarchive& ar,
               const Value& data,
               const unsigned int version)
{
    serialize_functor<Value>()(ar, data, version);
}

template <typename Value>
void serialize(protoc::json::oarchive& ar,
               Value& data,
               const unsigned int version)
{
    serialize_functor<Value>()(ar, data, version);
}

//-----------------------------------------------------------------------------
// iarchive
//-----------------------------------------------------------------------------

template <typename Value>
void load(protoc::json::iarchive& ar,
          Value& data,
          const unsigned int version)
{
    load_functor<protoc::json::iarchive, Value>()(ar, data, version);
}

template <typename Value>
void serialize(protoc::json::iarchive& ar,
               Value& data,
               const unsigned int version)
{
    serialize_functor<Value>()(ar, data, version);
}

} // namespace serialization
} // namespace boost

#endif // PROTOC_JSON_SERIALIZATION_HPP
