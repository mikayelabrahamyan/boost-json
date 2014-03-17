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
#include <boost/archive/detail/common_oarchive.hpp>
#include <boost/archive/detail/register_archive.hpp>
#include <protoc/types.hpp>
#include <protoc/transenc/writer.hpp>

namespace protoc
{
namespace transenc
{

class oarchive
    : public boost::archive::detail::common_oarchive<oarchive>
{
    friend class boost::archive::save_access;

public:
    oarchive(transenc::writer&);

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
    transenc::writer& writer;
};

} // namespace transenc
} // namespace protoc

BOOST_SERIALIZATION_REGISTER_ARCHIVE(protoc::transenc::oarchive);

namespace protoc
{
namespace transenc
{

inline oarchive::oarchive(transenc::writer& writer)
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

inline void oarchive::save(const unsigned char *data, std::size_t size)
{
    writer.write(data, size);
}

inline void oarchive::save_record_begin()
{
    writer.record_begin();
}

inline void oarchive::save_record_end()
{
    writer.record_end();
}

inline void oarchive::save_array_begin()
{
    writer.array_begin();
}

inline void oarchive::save_array_begin(std::size_t size)
{
    writer.array_begin(size);
}

inline void oarchive::save_array_end()
{
    writer.array_end();
}

inline void oarchive::save_map_begin()
{
    writer.map_begin();
}

inline void oarchive::save_map_begin(std::size_t size)
{
    writer.map_begin(size);
}

inline void oarchive::save_map_end()
{
    writer.map_end();
}

} // namespace transenc
} // namespace protoc

#endif /* PROTOC_TRANSENC_OARCHIVE_HPP */
