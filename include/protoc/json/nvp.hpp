#ifndef PROTOC_JSON_NVP_HPP
#define PROTOC_JSON_NVP_HPP

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

#include <boost/serialization/nvp.hpp>
#include <protoc/exceptions.hpp>
#include <protoc/serialization/nvp.hpp>
#include <protoc/json/serialization.hpp>

namespace boost
{
namespace serialization
{

template <typename T>
struct save_functor< protoc::json::oarchive, typename boost::serialization::nvp<T> >
{
    void operator () (protoc::json::oarchive& ar,
                      const boost::serialization::nvp<T>& data,
                      const unsigned int version)
    {
        ar.save_map_begin();
        const char *name = data.name();
        ar << name;
        ar << data.value();
        ar.save_map_end();
    }
};

template <typename T>
struct load_functor< protoc::json::iarchive, typename boost::serialization::nvp<T> >
{
    void operator () (protoc::json::iarchive& ar,
                      boost::serialization::nvp<T>& data,
                      const unsigned int version)
    {
        ar.load_map_begin();
        if (ar.at_map_end())
            throw protoc::invalid_scope("empty map");
        ar >> data.name();
        ar >> data.value();
        ar.load_map_end();
        if (!ar.at_map_end())
            throw protoc::invalid_scope("too many elements");
    }
};

} // namespace serialization
} // namespace boost

#endif // PROTOC_JSON_NVP_HPP
