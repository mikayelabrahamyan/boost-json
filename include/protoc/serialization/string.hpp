#ifndef PROTOC_STRING_HPP
#define PROTOC_STRING_HPP

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

#include <string>
#include <boost/serialization/split_free.hpp>
#include <protoc/serialization/serialization.hpp>
#include <protoc/basic_iarchive.hpp>
#include <protoc/basic_oarchive.hpp>

namespace boost
{
namespace serialization
{

template <typename CharT, typename Traits, typename Allocator>
struct save_functor< typename std::basic_string<CharT, Traits, Allocator> >
{
    void operator () (protoc::basic_oarchive& ar,
                      const std::basic_string<CharT, Traits, Allocator>& data,
                      const unsigned int)
    {
        ar.save(data);
    }
};

template <typename CharT, typename Traits, typename Allocator>
struct load_functor< typename std::basic_string<CharT, Traits, Allocator> >
{
    void operator () (protoc::basic_iarchive& ar,
                      std::basic_string<CharT, Traits, Allocator>& data,
                      const unsigned int)
    {
        ar.load(data);
    }
};

template <typename CharT, typename Traits, typename Allocator>
struct serialize_functor< typename std::basic_string<CharT, Traits, Allocator> >
{
    void operator () (protoc::basic_iarchive& ar,
                      std::basic_string<CharT, Traits, Allocator>& data,
                      const unsigned int version)
    {
        split_free(ar, data, version);
    }

    void operator () (protoc::basic_oarchive& ar,
                      const std::basic_string<CharT, Traits, Allocator>& data,
                      const unsigned int version)
    {
        split_free(ar, data, version);
    }
};

} // namespace serialization
} // namespace boost

#endif // PROTOC_STRING_HPP
