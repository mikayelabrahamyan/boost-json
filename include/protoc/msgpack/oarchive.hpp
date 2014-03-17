#ifndef PROTOC_MSGPACK_OARCHIVE_HPP
#define PROTOC_MSGPACK_OARCHIVE_HPP

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

#include <boost/archive/detail/common_oarchive.hpp>
#include <string>
#include <protoc/msgpack/writer.hpp>

namespace protoc
{
namespace msgpack
{

class oarchive
    : public boost::archive::detail::common_oarchive<oarchive>
{
    friend class boost::archive::save_access;

public:
    oarchive(msgpack::writer&);

    void save();
    void save(bool);
    void save(int);
    void save(long long);
    void save(float);
    void save(double);
    void save(const char *);
    void save(const std::string&);
    void save(const unsigned char *, std::size_t);

    void save_record_begin();
    void save_record_end();
    void save_array_begin();
    void save_array_begin(std::size_t);
    void save_array_end();
    void save_map_begin();
    void save_map_begin(std::size_t);
    void save_map_end();

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

protected:
    msgpack::writer& writer;
};

} // namespace msgpack
} // namespace protoc

#include <boost/archive/detail/register_archive.hpp>

BOOST_SERIALIZATION_REGISTER_ARCHIVE(protoc::msgpack::oarchive);

namespace protoc
{
namespace msgpack
{

inline oarchive::oarchive(msgpack::writer& writer)
    : writer(writer)
{
}

inline void oarchive::save()
{
    writer.write();
}

inline void oarchive::save(bool value)
{
    writer.write(value);
}

inline void oarchive::save(int value)
{
    writer.write(value);
}

inline void oarchive::save(long long value)
{
    writer.write(value);
}

inline void oarchive::save(float value)
{
    writer.write(value);
}

inline void oarchive::save(double value)
{
    writer.write(value);
}

inline void oarchive::save(const char *value)
{
    writer.write(value);
}

inline void oarchive::save(const std::string& value)
{
    writer.write(value);
}

} // namespace msgpack
} // namespace protoc

#endif // PROTOC_MSGPACK_OARCHIVE_HPP
