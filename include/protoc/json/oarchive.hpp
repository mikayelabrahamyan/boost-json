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
#include <utility> // std::pair
#include <stack>
#include <ostream>
#include <boost/make_shared.hpp>
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
        scope.push(boost::make_shared<array_frame>(output));

        boost::archive::save(*this->This(), const_cast<const value_type&>(data.value()));

        scope.pop();
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
        scope.push(boost::make_shared<array_frame>(output));
        *this << boost::serialization::make_nvp("first", data.value().first);
        *this << boost::serialization::make_nvp("second", data.value().second);
        scope.pop();
    }

    template<typename first_type, typename second_type>
    void save_override(const boost::serialization::nvp< std::pair<first_type, second_type> >& data, int version)
    {
        this->save_override(boost::serialization::make_nvp(data.name(), const_cast<const std::pair<first_type, second_type>&>(data.value())), version);
    }

    // std::vector
    template<typename value_type, typename allocator_type>
    void save_override(const boost::serialization::nvp< const std::vector<value_type, allocator_type> >& data, int)
    {
        scope.push(boost::make_shared<array_frame>(output));

        for (typename std::vector<value_type, allocator_type>::const_iterator it = data.value().begin();
             it != data.value().end();
             ++it)
        {
            value_type value = *it;
            *this << boost::serialization::make_nvp(data.name(), value);
        }

        scope.pop();
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
        scope.push(boost::make_shared<array_frame>(output));

        for (typename std::map<key_type, mapped_type>::const_iterator it = data.value().begin();
             it != data.value().end();
             ++it)
        {
            scope.top()->put_separator();
            *this << boost::serialization::make_nvp(data.name(), *it);
        }

        scope.pop();
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
        scope.push(boost::make_shared<object_frame>(output));

        for (typename std::map<std::string, mapped_type>::const_iterator it = data.value().begin();
             it != data.value().end();
             ++it)
        {
            *this << boost::serialization::make_nvp(data.name(), it->first);
            *this << boost::serialization::make_nvp(data.name(), it->second);
        }

        scope.pop();
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
    class frame
    {
    public:
        frame(const encoder&);
        virtual ~frame() {}

        virtual void put_separator() = 0;

        template<typename T>
        void put(const T& value)
        {
            put_separator();
            output.put(value);
        }

    protected:
        encoder& output;
        std::size_t counter;
    };

    class top_frame : public frame
    {
    public:
        top_frame(const encoder&);
        void put_separator();
    };

    class array_frame : public frame
    {
    public:
        array_frame(const encoder&);
        ~array_frame();
        void put_separator();
    };

    class object_frame : public frame
    {
    public:
        object_frame(const encoder&);
        ~object_frame();
        void put_separator();
    };

    protoc::output_stream buffer;
    encoder output;
    std::stack< boost::shared_ptr<frame> > scope;
};

} // namespace json
} // namespace protoc

BOOST_SERIALIZATION_REGISTER_ARCHIVE(protoc::json::oarchive);

#endif // PROTOC_JSON_OARCHIVE_HPP
