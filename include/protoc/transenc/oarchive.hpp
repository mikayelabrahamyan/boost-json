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
#include <limits>
#include <ostream>
#include <istream>
#include <boost/optional.hpp>
#include <boost/ref.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/archive/detail/common_oarchive.hpp>
#include <boost/archive/detail/register_archive.hpp>
#include <boost/utility/base_from_member.hpp>
#include <protoc/types.hpp>
#include <protoc/output_stream.hpp>
#include <protoc/transenc/encoder.hpp>
#include <protoc/basic_oarchive.hpp>

namespace protoc
{
namespace transenc
{

class basic_oarchive
    : public protoc::basic_oarchive
{
public:
    basic_oarchive(transenc::encoder&);

    virtual void save();
    virtual void save(bool);
    virtual void save(int);
    virtual void save(long long);
    virtual void save(float);
    virtual void save(double);
    virtual void save(const char *);
    virtual void save(const std::string&);
    virtual void save_binary(const char *, std::size_t);

    virtual void save_record_begin();
    virtual void save_record_end();
    virtual void save_array_begin();
    virtual void save_array_begin(std::size_t);
    virtual void save_array_end();
    virtual void save_map_begin();
    virtual void save_map_begin(std::size_t);
    virtual void save_map_end();

protected:
    transenc::encoder& encoder;
};

// base_from_member is needed because we want to add a member that must be
// initialized before oarchive because it is passed as an argument to its
// constructor
class stream_oarchive
    : private boost::base_from_member< protoc::output_stream<protoc::uint8_t> >,
      private boost::base_from_member<transenc::encoder>,
      public basic_oarchive
{
    typedef protoc::output_stream<protoc::uint8_t> member1_type;
    typedef transenc::encoder member2_type;
    typedef boost::base_from_member<member1_type> base_member1_type;
    typedef boost::base_from_member<member2_type> base_member2_type;

public:
    stream_oarchive(std::ostream& stream)
        : base_member1_type(member1_type(stream)),
          base_member2_type(boost::ref(base_member1_type::member)),
          basic_oarchive(base_member2_type::member)
    {}
};

} // namespace transenc
} // namespace protoc

BOOST_SERIALIZATION_REGISTER_ARCHIVE(protoc::transenc::basic_oarchive);
BOOST_SERIALIZATION_REGISTER_ARCHIVE(protoc::transenc::stream_oarchive);

namespace protoc
{
namespace transenc
{

inline basic_oarchive::basic_oarchive(transenc::encoder& encoder)
    : protoc::basic_oarchive(),
      encoder(encoder)
{
}

inline void basic_oarchive::save()
{
    encoder.put();
}

inline void basic_oarchive::save(bool value)
{
    encoder.put(value);
}

inline void basic_oarchive::save(int value)
{
    encoder.put(value);
}

inline void basic_oarchive::save(long long value)
{
    encoder.put(value);
}

inline void basic_oarchive::save(float value)
{
    encoder.put(value);
}

inline void basic_oarchive::save(double value)
{
    encoder.put(value);
}

inline void basic_oarchive::save(const char *value)
{
    encoder.put(value);
}

inline void basic_oarchive::save(const std::string& value)
{
    encoder.put(value);
}

inline void basic_oarchive::save_binary(const char *data, std::size_t size)
{
    encoder.put(data, size);
}

inline void basic_oarchive::save_record_begin()
{
    encoder.put_record_begin();
}

inline void basic_oarchive::save_record_end()
{
    encoder.put_record_end();
}

inline void basic_oarchive::save_array_begin()
{
    encoder.put_array_begin();
}

inline void basic_oarchive::save_array_begin(std::size_t size)
{
    encoder.put_array_begin(size);
}

inline void basic_oarchive::save_array_end()
{
    encoder.put_array_end();
}

inline void basic_oarchive::save_map_begin()
{
    encoder.put_map_begin();
}

inline void basic_oarchive::save_map_begin(std::size_t size)
{
    encoder.put_map_begin(size);
}

inline void basic_oarchive::save_map_end()
{
    encoder.put_map_end();
}

} // namespace transenc
} // namespace protoc

#endif /* PROTOC_TRANSENC_OARCHIVE_HPP */
