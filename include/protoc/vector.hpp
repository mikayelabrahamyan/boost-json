#ifndef PROTOC_VECTOR_HPP
#define PROTOC_VECTOR_HPP

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

#include <vector>
#include <boost/type_traits/remove_const.hpp>
#include <boost/serialization/split_free.hpp>
#include <protoc/serialization.hpp>
#include <protoc/basic_oarchive.hpp>

namespace boost
{
namespace serialization
{

template <typename T, typename Allocator>
struct save_functor< typename std::vector<T, Allocator> >
{
    void operator () (protoc::basic_oarchive& ar,
                      const std::vector<T, Allocator>& data,
                      const unsigned int version)
    {
        ar.save_array_begin(data.size());
        for (typename std::vector<T, Allocator>::const_iterator it = data.begin();
             it != data.end();
             ++it)
        {
            ar.save_override(*it, version);
        }
        ar.save_array_end();
    }
};

// Specialization of std::vector<char> for binary data
template <typename Allocator>
struct save_functor< typename std::vector<char, Allocator> >
{
    void operator () (protoc::basic_oarchive& ar,
                      const std::vector<char, Allocator>& data,
                      const unsigned int version)
    {
        // archive::save_binary takes "void *" as the first argument, so we
        // need to remove const.
        typedef typename std::vector<char, Allocator>::pointer pointer;
        ar.save_binary(const_cast<typename boost::remove_const<pointer>::type>(data.data()),
                       data.size());
    }
};

template <typename T, typename Allocator>
struct serialize_functor< typename std::vector<T, Allocator> >
{
    void operator () (protoc::basic_oarchive& ar,
                      const std::vector<T, Allocator>& data,
                      const unsigned int version)
    {
        split_free(ar, data, version);
    }
};

} // namespace serialization
} // namespace boost

#endif // PROTOC_VECTOR_HPP
