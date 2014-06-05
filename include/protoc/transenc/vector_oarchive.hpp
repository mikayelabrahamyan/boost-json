#ifndef PROTOC_TRANSENC_VECTOR_OARCHIVE_HPP
#define PROTOC_TRANSENC_VECTOR_OARCHIVE_HPP

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

#include <vector>
#include <boost/ref.hpp>
#include <protoc/output_container.hpp>
#include <protoc/transenc/writer.hpp>
#include <protoc/transenc/oarchive.hpp>

namespace protoc
{
namespace transenc
{

class vector_oarchive
    : private boost::base_from_member< protoc::output_container<transenc::writer::value_type, std::vector> >,
      private boost::base_from_member<transenc::writer>,
      public oarchive
{
    typedef protoc::output_container<protoc::writer::value_type, std::vector> output_type;
    typedef transenc::writer writer_type;
    typedef boost::base_from_member<output_type> base_output_type;
    typedef boost::base_from_member<writer_type> base_writer_type;

public:
    vector_oarchive(std::vector<protoc::writer::value_type>& container)
        : base_output_type(boost::ref(container)),
          base_writer_type(boost::ref(base_output_type::member)),
          oarchive(base_writer_type::member)
    {}
};

} // namespace transenc
} // namespace protoc

BOOST_SERIALIZATION_REGISTER_ARCHIVE(protoc::transenc::vector_oarchive);

#endif // PROTOC_TRANSENC_VECTOR_OARCHIVE_HPP
