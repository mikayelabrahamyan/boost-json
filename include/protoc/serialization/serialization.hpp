#ifndef PROTOC_SERIALIZATION_HPP
#define PROTOC_SERIALIZATION_HPP

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

#include <boost/serialization/split_free.hpp>

namespace protoc { class basic_iarchive; }
namespace protoc { class basic_oarchive; }

// Non-intrusive serialization
namespace boost
{
namespace serialization
{

template <typename T>
struct save_functor
{
    void operator () (protoc::basic_oarchive& ar,
                      const T& data,
                      const unsigned int)
    {
        ar.save_record_begin();
        data.save(ar);
        ar.save_record_end();
    }
};

template <typename T>
struct load_functor
{
    void operator () (protoc::basic_iarchive& ar,
                      const T& data,
                      const unsigned int)
    {
        ar.load_record_begin();
        data.load(ar);
        ar.load_record_end();
    }
};

template <typename T>
struct serialize_functor
{
    void operator () (protoc::basic_iarchive& ar,
                      T& data,
                      const unsigned int version)
    {
        ar.load_record_begin(); // basic_iarchive can only load
        data.serialize(ar, version);
        ar.load_record_end();
    }

    void operator () (protoc::basic_oarchive& ar,
                      const T& data,
                      const unsigned int version)
    {
        ar.save_record_begin(); // basic_oarchive can only save
        data.serialize(ar, version);
        ar.save_record_end();
    }
};

//-----------------------------------------------------------------------------
// basic_oarchive
//-----------------------------------------------------------------------------

// C++ does not have partial specialization of template functions so we use
// functors to achieve the same effect.

template <typename value_type>
inline void save(protoc::basic_oarchive& ar,
                 const value_type& data,
                 const unsigned int version)
{
    save_functor<value_type>()(ar, data, version);
}

template <typename value_type>
inline void serialize(protoc::basic_oarchive& ar,
                      const value_type& data,
                      const unsigned int version)
{
    serialize_functor<value_type>()(ar, data, version);
}

template <typename value_type>
inline void serialize(protoc::basic_oarchive& ar,
                      value_type& data,
                      const unsigned int version)
{
    serialize_functor<value_type>()(ar, data, version);
}

//-----------------------------------------------------------------------------
// basic_iarchive
//-----------------------------------------------------------------------------

template <typename value_type>
inline void load(protoc::basic_iarchive& ar,
                 value_type& data,
                 const unsigned int version)
{
    load_functor<value_type>()(ar, data, version);
}

template <typename value_type>
inline void serialize(protoc::basic_iarchive& ar,
                      value_type& data,
                      const unsigned int version)
{
    serialize_functor<value_type>()(ar, data, version);
}

} // namespace serialization
} // namespace boost

#endif // PROTOC_SERIALIZATION_HPP
