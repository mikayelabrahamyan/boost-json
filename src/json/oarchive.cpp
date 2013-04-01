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

//-----------------------------------------------------------------------------
// frame
//-----------------------------------------------------------------------------

oarchive::frame::frame(const encoder& output)
    : output(const_cast<encoder&>(output)),
      counter(0)
{
}

oarchive::top_frame::top_frame(const encoder& out)
    : frame(out)
{
}

void oarchive::top_frame::put_separator()
{
}

oarchive::array_frame::array_frame(const encoder& out)
    : frame(out)
{
    output.put_array_begin();
}

oarchive::array_frame::~array_frame()
{
    output.put_array_end();
}

void oarchive::array_frame::put_separator()
{
    if (counter > 0)
    {
        output.put_comma();
    }
    ++counter;
}

oarchive::object_frame::object_frame(const encoder& out)
    : frame(out)
{
    output.put_object_begin();
}

oarchive::object_frame::~object_frame()
{
    output.put_object_end();
}

void oarchive::object_frame::put_separator()
{
    if (counter > 0)
    {
        if (counter % 2)
        {
            output.put_colon();
        }
        else
        {
            output.put_comma();
        }
    }
    ++counter;
}

//-----------------------------------------------------------------------------
// oarchive
//-----------------------------------------------------------------------------

oarchive::oarchive(std::ostream& stream)
    : boost::archive::detail::common_oarchive<oarchive>(),
      buffer(stream),
      output(buffer)
{
    scope.push(boost::make_shared<top_frame>(output));
}

oarchive::~oarchive()
{
    assert(!scope.empty());
    scope.pop();
}

void oarchive::save_override(const boost::serialization::nvp<bool>& data, int)
{
    assert(!scope.empty());
    scope.top()->put(data.value());
}

void oarchive::save_override(const boost::serialization::nvp<const bool>& data, int)
{
    assert(!scope.empty());
    scope.top()->put(data.value());
}

void oarchive::save_override(const boost::serialization::nvp<protoc::int8_t>& data, int)
{
    assert(!scope.empty());
    scope.top()->put(static_cast<protoc::int64_t>(data.value()));
}

void oarchive::save_override(const boost::serialization::nvp<const protoc::int8_t>& data, int)
{
    assert(!scope.empty());
    scope.top()->put(static_cast<protoc::int64_t>(data.value()));
}

void oarchive::save_override(const boost::serialization::nvp<protoc::int16_t>& data, int)
{
    assert(!scope.empty());
    scope.top()->put(static_cast<protoc::int64_t>(data.value()));
}

void oarchive::save_override(const boost::serialization::nvp<const protoc::int16_t>& data, int)
{
    assert(!scope.empty());
    scope.top()->put(static_cast<protoc::int64_t>(data.value()));
}

void oarchive::save_override(const boost::serialization::nvp<protoc::int32_t>& data, int)
{
    assert(!scope.empty());
    scope.top()->put(static_cast<protoc::int64_t>(data.value()));
}

void oarchive::save_override(const boost::serialization::nvp<const protoc::int32_t>& data, int)
{
    assert(!scope.empty());
    scope.top()->put(static_cast<protoc::int64_t>(data.value()));
}

void oarchive::save_override(const boost::serialization::nvp<protoc::int64_t>& data, int)
{
    assert(!scope.empty());
    scope.top()->put(data.value());
}

void oarchive::save_override(const boost::serialization::nvp<const protoc::int64_t>& data, int)
{
    assert(!scope.empty());
    scope.top()->put(data.value());
}

void oarchive::save_override(const boost::serialization::nvp<protoc::float32_t>& data, int)
{
    assert(!scope.empty());
    scope.top()->put(static_cast<protoc::float64_t>(data.value()));
}

void oarchive::save_override(const boost::serialization::nvp<const protoc::float32_t>& data, int)
{
    assert(!scope.empty());
    scope.top()->put(static_cast<protoc::float64_t>(data.value()));
}

void oarchive::save_override(const boost::serialization::nvp<protoc::float64_t>& data, int)
{
    assert(!scope.empty());
    scope.top()->put(data.value());
}

void oarchive::save_override(const boost::serialization::nvp<const protoc::float64_t>& data, int)
{
    assert(!scope.empty());
    scope.top()->put(data.value());
}

void oarchive::save_override(const boost::serialization::nvp<std::string>& data, int)
{
    assert(!scope.empty());
    scope.top()->put(data.value());
}

void oarchive::save_override(const boost::serialization::nvp<const std::string>& data, int)
{
    assert(!scope.empty());
    scope.top()->put(data.value());
}

} // namespace json
} // namespace protoc
