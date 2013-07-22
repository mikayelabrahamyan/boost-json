#ifndef PROTOC_BASIC_OARCHIVE_HPP
#define PROTOC_BASIC_OARCHIVE_HPP

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

#include <cstdlib> // std::size_t
#include <boost/optional.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/archive/detail/common_oarchive.hpp>
#include <protoc/encoder_base.hpp>

namespace protoc
{

class basic_oarchive : public boost::archive::detail::common_oarchive<basic_oarchive>
{
    friend class boost::archive::save_access;

public:
    basic_oarchive(encoder_base& encoder);
    ~basic_oarchive();

    virtual void save_record_begin() = 0;
    virtual void save_record_end() = 0;
    virtual void save_array_begin() = 0;
    virtual void save_array_begin(std::size_t) = 0;
    virtual void save_array_end() = 0;
    virtual void save_map_begin() = 0;
    virtual void save_map_begin(std::size_t) = 0;
    virtual void save_map_end() = 0;

    // The const variants are needed when used in containers
    void save_override(bool, int);
    void save_override(int, int);
    void save_override(long long, int);
    void save_override(float, int);
    void save_override(double, int);
    void save_override(const char *, int);
    void save_override(const std::string&, int);

    template<typename value_type>
    void save_override(const value_type& data, long version)
    {
        boost::archive::save(*this->This(), data);
    }

    // boost::optional
    template<typename value_type>
    void save_override(const boost::optional<value_type>& data, int);
    template<typename value_type>
    void save_override(boost::optional<value_type>& data, int version);

    // boost::serialization::nvp
    template<typename value_type>
    void save_override(const boost::serialization::nvp<value_type>& data, int);

    // Ignore these
    void save_override(const boost::archive::version_type, int) {}
    void save_override(const boost::archive::object_id_type, int) {}
    void save_override(const boost::archive::object_reference_type, int) {}
    void save_override(const boost::archive::class_id_type, int) {}
    void save_override(const boost::archive::class_id_optional_type, int) {}
    void save_override(const boost::archive::class_id_reference_type, int) {}
    void save_override(const boost::archive::tracking_type, int) {}
    void save_override(const boost::archive::class_name_type&, int) {}

    void save_binary(void *, std::size_t);

private:
    encoder_base& encoder;
};

} // namespace protoc

namespace protoc
{

inline basic_oarchive::basic_oarchive(encoder_base& encoder)
    : boost::archive::detail::common_oarchive<basic_oarchive>(),
      encoder(encoder)
{
}

inline basic_oarchive::~basic_oarchive()
{
}

inline void basic_oarchive::save_override(bool value, int)
{
    encoder.put(value);
}

inline void basic_oarchive::save_override(int value, int)
{
    encoder.put(value);
}

inline void basic_oarchive::save_override(long long value, int)
{
    encoder.put(value);
}

inline void basic_oarchive::save_override(float value, int)
{
    encoder.put(value);
}

inline void basic_oarchive::save_override(double value, int)
{
    encoder.put(value);
}

inline void basic_oarchive::save_override(const char *value, int)
{
    encoder.put(value);
}

inline void basic_oarchive::save_override(const std::string& value, int)
{
    encoder.put(value);
}

// boost::optional
template<typename value_type>
void basic_oarchive::save_override(const boost::optional<value_type>& data, int)
{
    if (data)
    {
        *this << *data;
    }
    else
    {
        encoder.put();
    }
}

template<typename value_type>
void basic_oarchive::save_override(boost::optional<value_type>& data, int version)
{
    this->save_override(const_cast<const boost::optional<value_type>&>(data), version);
}

// boost::serialization::nvp
template<typename value_type>
void basic_oarchive::save_override(const boost::serialization::nvp<value_type>& data, int)
{
    *this << data.value();
}

inline void basic_oarchive::save_binary(void *data, std::size_t size)
{
    encoder.put(data, size);
}

} // namespace protoc

#endif // PROTOC_BASIC_OARCHIVE_HPP
