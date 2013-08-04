#ifndef PROTOC_SERIALIZATION_BASIC_OARCHIVE_HPP
#define PROTOC_SERIALIZATION_BASIC_OARCHIVE_HPP

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
#include <boost/archive/detail/common_oarchive.hpp>

namespace protoc
{

class basic_oarchive : public boost::archive::detail::common_oarchive<basic_oarchive>
{
    friend class boost::archive::save_access;

public:
    virtual ~basic_oarchive() {}

    virtual void save() = 0; // Null
    virtual void save(bool) = 0;
    virtual void save(int) = 0;
    virtual void save(long long) = 0;
    virtual void save(float) = 0;
    virtual void save(double) = 0;
    virtual void save(const char *) = 0;
    virtual void save(const std::string&) = 0;
    virtual void save_binary(const char *, std::size_t) = 0;


    virtual void save_record_begin() = 0;
    virtual void save_record_end() = 0;
    virtual void save_array_begin() = 0;
    virtual void save_array_begin(std::size_t) = 0;
    virtual void save_array_end() = 0;
    virtual void save_map_begin() = 0;
    virtual void save_map_begin(std::size_t) = 0;
    virtual void save_map_end() = 0;

    template<typename T>
    void save_override(const T& data, long /*version*/)
    {
        boost::archive::save(*this->This(), data);
    }

    // String literal
    void save_override(const char *data, int /*version*/)
    {
        save(data);
    }

    // Ignore these
    void save_override(const boost::archive::version_type, int) {}
    void save_override(const boost::archive::object_id_type, int) {}
    void save_override(const boost::archive::object_reference_type, int) {}
    void save_override(const boost::archive::class_id_type, int) {}
    void save_override(const boost::archive::class_id_optional_type, int) {}
    void save_override(const boost::archive::class_id_reference_type, int) {}
    void save_override(const boost::archive::tracking_type, int) {}
    void save_override(const boost::archive::class_name_type&, int) {}
};

} // namespace protoc

#endif // PROTOC_SERIALIZATION_BASIC_OARCHIVE_HPP
