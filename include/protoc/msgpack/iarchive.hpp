#ifndef PROTOC_MSGPACK_IARCHIVE_HPP
#define PROTOC_MSGPACK_IARCHIVE_HPP

///////////////////////////////////////////////////////////////////////////////
//
// http://protoc.sourceforge.net/
//
// Copyright (C) 2014 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <cstddef> // std::size_t
#include <string>
#include <boost/archive/detail/common_iarchive.hpp>
#include <protoc/msgpack/reader.hpp>

namespace protoc
{
namespace msgpack
{

class iarchive
    : public boost::archive::detail::common_iarchive<iarchive>
{
    friend class boost::archive::load_access;

public:
    typedef msgpack::reader::value_type value_type;

    iarchive(const msgpack::reader&);
    template <typename Iterator>
    iarchive(Iterator begin, Iterator end);

    template<typename value_type>
    void load_override(value_type& data, long /*version*/)
    {
        boost::archive::load(*this->This(), data);
    }

    void load();
    void load(bool&);
    void load(int&);
    void load(long long&);
    void load(float&);
    void load(double&);
    void load(std::string&);

    std::size_t load_binary_begin();
    void load(void *, std::size_t);

    void load_record_begin();
    void load_record_end();

    std::size_t load_array_begin();
    void load_array_end();
    bool at_array_end() const;

    std::size_t load_map_begin();
    void load_map_end();
    bool at_map_end() const;

    protoc::token::value type() const;

    // Ignore these
    void load(boost::archive::version_type&) {}
    void load(boost::archive::object_id_type) {}
    void load(boost::archive::object_reference_type) {}
    void load(boost::archive::class_id_type) {}
    void load(boost::archive::class_id_optional_type) {}
    void load(boost::archive::class_id_reference_type) {}
    void load(boost::archive::tracking_type) {}
    void load(boost::archive::class_name_type&) {}

private:
    msgpack::reader reader;
};

} // namespace msgpack
} // namespace protoc

#include <boost/archive/detail/register_archive.hpp>

BOOST_SERIALIZATION_REGISTER_ARCHIVE(protoc::msgpack::iarchive);

namespace protoc
{
namespace msgpack
{

inline iarchive::iarchive(const msgpack::reader& reader)
    : reader(reader)
{
}

template <typename Iterator>
inline iarchive::iarchive(Iterator begin, Iterator end)
    : reader(begin, end)
{
}

inline void iarchive::load(bool& value)
{
    value = reader.get_bool();
    reader.next();
}

inline void iarchive::load(int& value)
{
    value = reader.get_int();
    reader.next();
}

inline void iarchive::load(long long& value)
{
    value = reader.get_long_long();
    reader.next();
}

inline void iarchive::load(float& value)
{
    value = reader.get_double();
    reader.next();
}

inline void iarchive::load(double& value)
{
    value = reader.get_double();
    reader.next();
}

inline void iarchive::load(std::string& value)
{
    value = reader.get_string();
    reader.next();
}

} // namespace msgpack
} // namespace protoc

#endif // PROTOC_MSGPACK_IARCHIVE_HPP
