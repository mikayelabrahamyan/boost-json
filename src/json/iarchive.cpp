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

#include <exception>
#include <sstream>
#include <protoc/exceptions.hpp>
#include <protoc/json/iarchive.hpp>

namespace protoc
{
namespace json
{

//-----------------------------------------------------------------------------
// frame
//-----------------------------------------------------------------------------

iarchive::frame::frame(detail::decoder& input)
    : input(input)
{
}

iarchive::frame::~frame()
{
}

void iarchive::frame::throw_unexpected_token(const detail::token type)
{
    if (std::uncaught_exception())
        return;
    std::ostringstream message;
    message << type;
    throw unexpected_token(message.str());
}

iarchive::array_frame::array_frame(detail::decoder& input)
    : frame(input)
{
    const detail::token type = input.type();
    if (type != detail::token_array_begin)
        throw_unexpected_token(type);
    input.next();
}

iarchive::array_frame::~array_frame()
{
    if (!at_end())
        throw_unexpected_token(input.type());
    input.next();
}

void iarchive::array_frame::get_separator()
{
    const detail::token type = input.type();
    switch (type)
    {
    case detail::token_comma:
        input.next();
        break;

    case detail::token_array_end:
        break;

    default:
        throw_unexpected_token(type);
    }
}

bool iarchive::array_frame::at_end() const
{
    return (input.type() == detail::token_array_end);
}

iarchive::object_frame::object_frame(detail::decoder& input)
    : frame(input),
      counter(0)
{
    const detail::token type = input.type();
    if (type != detail::token_object_begin)
        throw_unexpected_token(type);
    input.next();
}

iarchive::object_frame::~object_frame()
{
    if (!at_end())
        throw_unexpected_token(input.type());
    input.next();
}

void iarchive::object_frame::get_separator()
{
    const detail::token type = input.type();
    switch (type)
    {
    case detail::token_colon:
        if (counter % 2 == 0)
        {
            input.next();
        }
        else
        {
            throw_unexpected_token(type);
        }
        break;

    case detail::token_comma:
        if (counter % 2 == 1)
        {
            input.next();
        }
        else
        {
            throw_unexpected_token(type);
        }
        break;

    case detail::token_object_end:
        break;

    default:
        throw_unexpected_token(type);
    }
    ++counter;
}

bool iarchive::object_frame::at_end() const
{
    return (input.type() == detail::token_object_end);
}

//-----------------------------------------------------------------------------
// iarchive
//-----------------------------------------------------------------------------

iarchive::iarchive(const char *begin, const char * end)
    : input(begin, end)
{
}

iarchive::~iarchive()
{
}

void iarchive::load_override(boost::serialization::nvp<bool> data, int)
{
    const detail::token type = input.type();
    switch (type)
    {
    case detail::token_true:
        data.value() = true;
        input.next();
        break;

    case detail::token_false:
        data.value() = false;
        input.next();
        break;

    default:
        std::ostringstream error;
        error << type;
        throw unexpected_token(error.str());
    }
}

void iarchive::load_override(boost::serialization::nvp<protoc::int64_t> data, int)
{
    const detail::token type = input.type();
    switch (type)
    {
    case detail::token_integer:
        data.value() = input.get_integer();
        input.next();
        break;

    default:
        std::ostringstream error;
        error << type;
        throw unexpected_token(error.str());
    }
}

void iarchive::load_override(boost::serialization::nvp<protoc::float64_t> data, int)
{
    const detail::token type = input.type();
    switch (type)
    {
    case detail::token_float:
        data.value() = input.get_float();
        input.next();
        break;

    default:
        std::ostringstream error;
        error << type;
        throw unexpected_token(error.str());
    }
}

void iarchive::load_override(boost::serialization::nvp<std::string> data, int)
{
    const detail::token type = input.type();
    switch (type)
    {
    case detail::token_string:
        data.value() = input.get_string();
        input.next();
        break;

    default:
        std::ostringstream error;
        error << type;
        throw unexpected_token(error.str());
    }
}

} // namespace json
} // namespace protoc
