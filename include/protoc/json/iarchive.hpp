#ifndef PROTOC_JSON_IARCHIVE_HPP
#define PROTOC_JSON_IARCHIVE_HPP

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

#include <boost/serialization/nvp.hpp>
#include <boost/archive/detail/common_iarchive.hpp>
#include <boost/archive/detail/register_archive.hpp>
#include <protoc/types.hpp>
#include <protoc/json/decoder.hpp>

namespace protoc
{
namespace json
{

class iarchive : public boost::archive::detail::common_iarchive<iarchive>
{
    friend class boost::archive::load_access;

public:
    iarchive(const char *begin, const char *end);
    ~iarchive();

    void load_override(boost::serialization::nvp<bool>, int);
    void load_override(boost::serialization::nvp<protoc::int8_t>, int);
    void load_override(boost::serialization::nvp<protoc::int16_t>, int);
    void load_override(boost::serialization::nvp<protoc::int32_t>, int);
    void load_override(boost::serialization::nvp<protoc::int64_t>, int);
    void load_override(boost::serialization::nvp<protoc::float32_t>, int);
    void load_override(boost::serialization::nvp<protoc::float64_t>, int);
    void load_override(boost::serialization::nvp<std::string>, int);

    // Ignore these
    void load_override(boost::archive::version_type, int) {}
    void load_override(boost::archive::object_id_type, int) {}
    void load_override(boost::archive::object_reference_type, int) {}
    void load_override(boost::archive::class_id_type, int) {}
    void load_override(boost::archive::class_id_optional_type, int) {}
    void load_override(boost::archive::class_id_reference_type, int) {}
    void load_override(boost::archive::tracking_type, int) {}
    void load_override(boost::archive::class_name_type&, int) {}

    void load_binary(void *, std::size_t) {}

private:
    decoder input;
};

} // namespace json
} // namespace protoc

BOOST_SERIALIZATION_REGISTER_ARCHIVE(protoc::json::iarchive);


#endif // PROTOC_JSON_IARCHIVE_HPP
