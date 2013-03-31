#ifndef PROTOC_JSON_OARCHIVE_HPP
#define PROTOC_JSON_OARCHIVE_HPP

///////////////////////////////////////////////////////////////////////////////
//
// http://protoc.sourceforge.net/
//
// Copyright (C) 2013 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <string>
#include <vector>
#include <map>
#include <ostream>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/detail/common_oarchive.hpp>
#include <boost/archive/detail/register_archive.hpp>
#include <protoc/types.hpp>
#include <protoc/output_stream.hpp>
#include <protoc/json/encoder.hpp>

namespace protoc
{
namespace json
{

class oarchive : public boost::archive::detail::common_oarchive<oarchive>
{
    friend class boost::archive::save_access;

public:
    oarchive(std::ostream& stream);
    ~oarchive();

    template<typename value_type>
    void save(const value_type& data)
    {
    }

    template<typename value_type>
    void save_override(const boost::serialization::nvp<value_type>& data, long)
    {
        this->This()->save_start(data.name());
        boost::archive::save(*this->This(), const_cast<const value_type&>(data.value()));
        this->This()->save_end(data.name());
    }

    // The const variants are needed when used in containers
    void save_override(const boost::serialization::nvp<bool>&, int);
    void save_override(const boost::serialization::nvp<const bool>&, int);
    void save_override(const boost::serialization::nvp<protoc::int8_t>&, int);
    void save_override(const boost::serialization::nvp<const protoc::int8_t>&, int);
    void save_override(const boost::serialization::nvp<protoc::int16_t>&, int);
    void save_override(const boost::serialization::nvp<const protoc::int16_t>&, int);
    void save_override(const boost::serialization::nvp<protoc::int32_t>&, int);
    void save_override(const boost::serialization::nvp<const protoc::int32_t>&, int);
    void save_override(const boost::serialization::nvp<protoc::int64_t>&, int);
    void save_override(const boost::serialization::nvp<const protoc::int64_t>&, int);
    void save_override(const boost::serialization::nvp<protoc::float32_t>&, int);
    void save_override(const boost::serialization::nvp<const protoc::float32_t>&, int);
    void save_override(const boost::serialization::nvp<protoc::float64_t>&, int);
    void save_override(const boost::serialization::nvp<const protoc::float64_t>&, int);
    void save_override(const boost::serialization::nvp<std::string>&, int);
    void save_override(const boost::serialization::nvp<const std::string>&, int);

    // std::vector
    template<typename value_type, typename allocator_type>
    void save_override(const boost::serialization::nvp< const std::vector<value_type, allocator_type> >& data, int)
    {
        bool is_first = true;
        output.put_array_begin();
        for (typename std::vector<value_type, allocator_type>::const_iterator it = data.value().begin();
             it != data.value().end();
             ++it)
        {
            if (is_first)
            {
                is_first = false;
            }
            else
            {
                output.put_comma();
            }
            value_type value = *it;
            *this << boost::serialization::make_nvp(data.name(), value);
        }
        output.put_array_end();
    }

    template<typename value_type, typename allocator_type>
    void save_override(const boost::serialization::nvp< std::vector<value_type, allocator_type> >& data, int version)
    {
        this->save_override(boost::serialization::make_nvp(data.name(), const_cast<const std::vector<value_type, allocator_type>&>(data.value())), version);
    }

    // std::map
    template<typename key_type, typename mapped_type, typename key_compare, typename allocator_type>
    void save_override(const boost::serialization::nvp< const std::map<key_type, mapped_type, key_compare, allocator_type> >& data, int)
    {
        bool is_first = true;
        output.put_array_begin();
        for (typename std::map<key_type, mapped_type>::const_iterator it = data.value().begin();
             it != data.value().end();
             ++it)
        {
            if (is_first)
            {
                is_first = false;
            }
            else
            {
                output.put_comma();
            }
            // FIXME: std::pair
            output.put_array_begin();
            *this << boost::serialization::make_nvp(data.name()/*FIXME*/, it->first);
            output.put_comma();
            *this << boost::serialization::make_nvp(data.name()/*FIXME*/, it->second);
            output.put_array_end();
        }
        output.put_array_end();
    }

    template<typename key_type, typename mapped_type, typename key_compare, typename allocator_type>
    void save_override(const boost::serialization::nvp< std::map<key_type, mapped_type, key_compare, allocator_type> >& data, int version)
    {
        this->save_override(boost::serialization::make_nvp(data.name(), const_cast<const std::map<key_type, mapped_type, key_compare, allocator_type>&>(data.value())), version);
    }

    // Specialization for std::map<std::string, T>
    template<typename mapped_type, typename key_compare, typename allocator_type>
    void save_override(const boost::serialization::nvp< const std::map<std::string, mapped_type, key_compare, allocator_type> >& data, int)
    {
        bool is_first = true;
        output.put_object_begin();
        for (typename std::map<std::string, mapped_type>::const_iterator it = data.value().begin();
             it != data.value().end();
             ++it)
        {
            if (is_first)
            {
                is_first = false;
            }
            else
            {
                output.put_comma();
            }
            *this << boost::serialization::make_nvp(data.name()/*FIXME*/, it->first);
            output.put_colon();
            *this << boost::serialization::make_nvp(data.name()/*FIXME*/, it->second);
        }
        output.put_object_end();
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

    void save_binary(void *, std::size_t) {}

private:
    protoc::output_stream buffer;
    encoder output;
};

} // namespace json
} // namespace protoc

BOOST_SERIALIZATION_REGISTER_ARCHIVE(protoc::json::oarchive);

#endif // PROTOC_JSON_OARCHIVE_HPP
