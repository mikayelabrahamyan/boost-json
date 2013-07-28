#ifndef PROTOC_OPTIONAL_HPP
#define PROTOC_OPTIONAL_HPP

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

#include <boost/optional.hpp>
#include <boost/none.hpp>
#include <boost/serialization/split_free.hpp>
#include <protoc/serialization/serialization.hpp>
#include <protoc/basic_oarchive.hpp>

namespace boost
{
namespace serialization
{

template <typename T>
struct save_functor< typename boost::optional<T> >
{
    void operator () (protoc::basic_oarchive& ar,
                      const boost::optional<T>& data,
                      const unsigned int version)
    {
        if (data)
        {
            ar.save_override(*data, version);
        }
        else
        {
            ar.save();
        }
    }
};

template <typename T>
struct load_functor< typename boost::optional<T> >
{
    void operator () (protoc::basic_iarchive& ar,
                      boost::optional<T>& data,
                      const unsigned int version)
    {
        if (ar.load_null())
        {
            data = boost::none;
        }
        else
        {
            T value; // FIXME: Replace with InPlaceFactory
            ar.load_override(value, version);
            data = value;
        }
    }
};

template <typename T>
struct serialize_functor< typename boost::optional<T> >
{
    void operator () (protoc::basic_iarchive& ar,
                      boost::optional<T>& data,
                      const unsigned int version)
    {
        split_free(ar, data, version);
    }

    void operator () (protoc::basic_oarchive& ar,
                      const boost::optional<T>& data,
                      const unsigned int version)
    {
        split_free(ar, data, version);
    }
};

} // namespace serialization
} // namespace boost

#endif // PROTOC_OPTIONAL_HPP
