#ifndef PROTOC_BASIC_IARCHIVE_HPP
#define PROTOC_BASIC_IARCHIVE_HPP

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

#include <cstddef> // std::size_t
#include <string>
#include <boost/optional.hpp>
#include <boost/archive/detail/common_iarchive.hpp>

namespace protoc
{

class basic_iarchive : public boost::archive::detail::common_iarchive<basic_iarchive>
{
    friend class boost::archive::load_access;

public:
    virtual void load(bool&) = 0;
    virtual void load(int&) = 0;
    virtual void load(long long&) = 0;
    virtual void load(float&) = 0;
    virtual void load(double&) = 0;
    virtual void load(std::string&) = 0;

    virtual void load_record_begin() = 0;
    virtual void load_record_end() = 0;
    virtual boost::optional<std::size_t> load_array_begin() = 0;
    virtual void load_array_end() = 0;
    virtual bool at_array_end() = 0;

    virtual bool load_null() = 0; // FIXME: Change to templated load_null_override() ?

    template<typename value_type>
    void load_override(value_type& data, long /*version*/)
    {
        boost::archive::load(*this->This(), data);
    }

    // Ignore these
    void load_override(boost::archive::version_type, int) {}
    void load_override(boost::archive::object_id_type, int) {}
    void load_override(boost::archive::object_reference_type, int) {}
    void load_override(boost::archive::class_id_type, int) {}
    void load_override(boost::archive::class_id_optional_type, int) {}
    void load_override(boost::archive::class_id_reference_type, int) {}
    void load_override(boost::archive::tracking_type, int) {}
    void load_override(boost::archive::class_name_type&, int) {}
};

} // namespace protoc

#endif // PROTOC_BASIC_IARCHIVE_HPP
