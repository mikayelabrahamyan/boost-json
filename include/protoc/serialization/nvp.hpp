#ifndef PROTOC_NVP_HPP
#define PROTOC_NVP_HPP

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

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/split_free.hpp>
#include <protoc/serialization/basic_oarchive.hpp>
#include <protoc/serialization/serialization.hpp>

namespace boost
{
namespace serialization
{

template <typename T>
struct save_functor< typename boost::serialization::nvp<T> >
{
    void operator () (protoc::basic_oarchive& ar,
                      const boost::serialization::nvp<T>& data,
                      const unsigned int version)
    {
        // Drop the name
        ar.save_override(data.value(), version);
    }
};

template <typename T>
struct serialize_functor< typename boost::serialization::nvp<T> >
{
    void operator () (protoc::basic_oarchive& ar,
                      const boost::serialization::nvp<T>& data,
                      const unsigned int version)
    {
        split_free(ar, data, version);
    }
};

} // namespace serialization
} // namespace boost

#endif // PROTOC_NVP_HPP