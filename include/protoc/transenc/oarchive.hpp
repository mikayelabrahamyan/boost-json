#ifndef PROTOC_TRANSENC_OARCHIVE_HPP
#define PROTOC_TRANSENC_OARCHIVE_HPP

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
#include <vector>
#include <map>
#include <utility> // std::pair
#include <ostream>
#include <istream>
#include <boost/optional.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/detail/common_oarchive.hpp>
#include <boost/archive/detail/register_archive.hpp>
#include <protoc/types.hpp>
#include <protoc/output_stream.hpp>
#include <protoc/transenc/encoder.hpp>

namespace protoc
{
namespace transenc
{

// FIXME: Null via boost::optional
class oarchive : public boost::archive::detail::common_oarchive<oarchive>
{
    friend class boost::archive::save_access;

public:
    oarchive(std::ostream& stream);
    ~oarchive();

    template<typename value_type>
    void save_override(const boost::serialization::nvp<value_type>& data, long)
    {
        output.put_tuple_begin();
        boost::archive::save(*this->This(), const_cast<const value_type&>(data.value()));
        output.put_tuple_end();
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

    // std::pair
    template<typename first_type, typename second_type>
    void save_override(const boost::serialization::nvp< const std::pair<first_type, second_type> >& data, int)
    {
        output.put_tuple_begin();
        *this << boost::serialization::make_nvp("first", data.value().first);
        *this << boost::serialization::make_nvp("second", data.value().second);
        output.put_tuple_end();
    }

    template<typename first_type, typename second_type>
    void save_override(const boost::serialization::nvp< std::pair<first_type, second_type> >& data, int version)
    {
        this->save_override(boost::serialization::make_nvp(data.name(), const_cast<const std::pair<first_type, second_type>&>(data.value())), version);
    }

    // boost::optional
    template<typename value_type>
    void save_override(const boost::serialization::nvp< const boost::optional<value_type> >& data, int)
    {
        if (data.value())
        {
            *this << boost::serialization::make_nvp("optional", *data.value());
        }
        else
        {
            output.put();
        }
    }

    template<typename value_type>
    void save_override(const boost::serialization::nvp< boost::optional<value_type> >& data, int version)
    {
        this->save_override(boost::serialization::make_nvp(data.name(), const_cast<const boost::optional<value_type>&>(data.value())), version);
    }
    
    // std::vector
    template<typename value_type, typename allocator_type>
    void save_override(const boost::serialization::nvp< const std::vector<value_type, allocator_type> >& data, int)
    {
        output.put_array_begin();
        for (typename std::vector<value_type, allocator_type>::const_iterator it = data.value().begin();
             it != data.value().end();
             ++it)
        {
            value_type value = *it;
            *this << boost::serialization::make_nvp(data.name(), value);
        }
        output.put_array_end();
    }

    // Specialization of std::vector<int8> for binary data
    template<typename allocator_type>
    void save_override(const boost::serialization::nvp< const std::vector<protoc::int8_t, allocator_type> >& data, int)
    {
        output.put(data.value());
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
        output.put_array_begin();
        for (typename std::map<key_type, mapped_type>::const_iterator it = data.value().begin();
             it != data.value().end();
             ++it)
        {
            *this << boost::serialization::make_nvp(data.name(), *it);
        }
        output.put_array_end();
    }

    template<typename key_type, typename mapped_type, typename key_compare, typename allocator_type>
    void save_override(const boost::serialization::nvp< std::map<key_type, mapped_type, key_compare, allocator_type> >& data, int version)
    {
        this->save_override(boost::serialization::make_nvp(data.name(), const_cast<const std::map<key_type, mapped_type, key_compare, allocator_type>&>(data.value())), version);
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
    protoc::output_stream<protoc::uint8_t> buffer;
    encoder output;
};

}
}

BOOST_SERIALIZATION_REGISTER_ARCHIVE(protoc::transenc::oarchive);

#endif /* PROTOC_TRANSENC_OARCHIVE_HPP */
