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
#include <limits>
#include <ostream>
#include <istream>
#include <boost/optional.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/detail/common_oarchive.hpp>
#include <boost/archive/detail/register_archive.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_enum.hpp>
#include <boost/utility/base_from_member.hpp>
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
    oarchive(const encoder& output);
    ~oarchive();

    // struct
    template<typename value_type>
    void save_override(const value_type& data, long)
    {
        output.put_record_begin();
        boost::archive::save(*this->This(), const_cast<const value_type&>(data));
        output.put_record_end();
    }

    // The const variants are needed when used in containers
    void save_override(bool, int);
    void save_override(protoc::int8_t, int);
    void save_override(protoc::int16_t, int);
    void save_override(protoc::int32_t, int);
    void save_override(protoc::int64_t, int);
    void save_override(protoc::float32_t, int);
    void save_override(protoc::float64_t, int);
    void save_override(const char *, int);
    void save_override(const std::string&, int);

    // enum
    template<typename value_type>
    typename boost::enable_if< boost::is_enum<value_type> >::type
    save_override(value_type data, int)
    {
        if (data <= std::numeric_limits<protoc::int8_t>::max())
        {
            output.put_tag(protoc::int8_t(data));
        }
        else if (data <= std::numeric_limits<protoc::int16_t>::max())
        {
            output.put_tag(protoc::int16_t(data));
        }
        else if (data <= std::numeric_limits<protoc::int32_t>::max())
        {
            output.put_tag(protoc::int32_t(data));
        }
        else
        {
            output.put_tag(protoc::int64_t(data));
        }
    }

    // std::pair
    template<typename first_type, typename second_type>
    void save_override(const std::pair<first_type, second_type>& data, int)
    {
        output.put_record_begin();
        *this << data.first;
        *this << data.second;
        output.put_record_end();
    }

    template<typename first_type, typename second_type>
    void save_override(std::pair<first_type, second_type>& data, int version)
    {
        this->save_override(const_cast<const std::pair<first_type, second_type>&>(data), version);
    }

    // boost::optional
    template<typename value_type>
    void save_override(const boost::optional<value_type>& data, int)
    {
        if (data)
        {
            *this << *data;
        }
        else
        {
            output.put();
        }
    }

    template<typename value_type>
    void save_override(boost::optional<value_type>& data, int version)
    {
        this->save_override(const_cast<const boost::optional<value_type>&>(data), version);
    }
    
    // std::vector
    template<typename value_type, typename allocator_type>
    void save_override(const std::vector<value_type, allocator_type>& data, int version)
    {
        output.put_array_begin(data.size());
        for (typename std::vector<value_type, allocator_type>::const_iterator it = data.begin();
             it != data.end();
             ++it)
        {
            this->save_override(*it, version);
        }
        output.put_array_end();
    }

    // Specialization of std::vector<int8> for binary data
    template<typename allocator_type>
    void save_override(const std::vector<protoc::int8_t, allocator_type>& data, int)
    {
        output.put(data);
    }

    // std::map
    template<typename key_type, typename mapped_type, typename key_compare, typename allocator_type>
    void save_override(const std::map<key_type, mapped_type, key_compare, allocator_type>& data, int version)
    {
        output.put_map_begin();
        for (typename std::map<key_type, mapped_type>::const_iterator it = data.begin();
             it != data.end();
             ++it)
        {
            this->save_override(*it, version);
        }
        output.put_map_end();
    }

    // boost::serialization::nvp
    template<typename value_type>
    void save_override(const boost::serialization::nvp<value_type>& data, int)
    {
        *this << data.value();
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
    encoder output;
};

// base_from_member is needed because we want to add a member that must be
// initialized before oarchive because it is passed as an argument to its
// constructor
class stream_oarchive
    : private boost::base_from_member< protoc::output_stream<protoc::uint8_t> >,
      public oarchive
{
    typedef protoc::output_stream<protoc::uint8_t> member_type;
    typedef oarchive base_type;

public:
    stream_oarchive(std::ostream& stream);
};

}
}

BOOST_SERIALIZATION_REGISTER_ARCHIVE(protoc::transenc::oarchive);

#endif /* PROTOC_TRANSENC_OARCHIVE_HPP */
