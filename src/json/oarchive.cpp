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

#include <protoc/json/oarchive.hpp>

namespace protoc
{
namespace json
{

oarchive::oarchive(std::ostream& stream)
    : boost::archive::detail::common_oarchive<oarchive>(),
      buffer(stream),
      output(buffer)
{
}

oarchive::~oarchive()
{
}

void oarchive::save_override(const boost::serialization::nvp<bool>& data, int)
{
    output.put(data.value());
}

void oarchive::save_override(const boost::serialization::nvp<const bool>& data, int)
{
    output.put(data.value());
}

void oarchive::save_override(const boost::serialization::nvp<protoc::int8_t>& data, int)
{
    output.put(static_cast<protoc::int64_t>(data.value()));
}

void oarchive::save_override(const boost::serialization::nvp<const protoc::int8_t>& data, int)
{
    output.put(static_cast<protoc::int64_t>(data.value()));
}

void oarchive::save_override(const boost::serialization::nvp<protoc::int16_t>& data, int)
{
    output.put(static_cast<protoc::int64_t>(data.value()));
}

void oarchive::save_override(const boost::serialization::nvp<const protoc::int16_t>& data, int)
{
    output.put(static_cast<protoc::int64_t>(data.value()));
}

void oarchive::save_override(const boost::serialization::nvp<protoc::int32_t>& data, int)
{
    output.put(static_cast<protoc::int64_t>(data.value()));
}

void oarchive::save_override(const boost::serialization::nvp<const protoc::int32_t>& data, int)
{
    output.put(static_cast<protoc::int64_t>(data.value()));
}

void oarchive::save_override(const boost::serialization::nvp<protoc::int64_t>& data, int)
{
    output.put(data.value());
}

void oarchive::save_override(const boost::serialization::nvp<const protoc::int64_t>& data, int)
{
    output.put(data.value());
}

void oarchive::save_override(const boost::serialization::nvp<protoc::float32_t>& data, int)
{
    output.put(static_cast<protoc::float64_t>(data.value()));
}

void oarchive::save_override(const boost::serialization::nvp<const protoc::float32_t>& data, int)
{
    output.put(static_cast<protoc::float64_t>(data.value()));
}

void oarchive::save_override(const boost::serialization::nvp<protoc::float64_t>& data, int)
{
    output.put(data.value());
}

void oarchive::save_override(const boost::serialization::nvp<const protoc::float64_t>& data, int)
{
    output.put(data.value());
}

void oarchive::save_override(const boost::serialization::nvp<std::string>& data, int)
{
    output.put(data.value());
}

void oarchive::save_override(const boost::serialization::nvp<const std::string>& data, int)
{
    output.put(data.value());
}

} // namespace json
} // namespace protoc
