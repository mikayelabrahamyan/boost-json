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

#include <sstream>
#include <protoc/exceptions.hpp>
#include <protoc/json/iarchive.hpp>

namespace protoc
{
namespace json
{

iarchive::iarchive(const char *begin, const char * end)
    : input(begin, end)
{
}

iarchive::~iarchive()
{
}

void iarchive::load_override(boost::serialization::nvp<bool> data, int)
{
    const token type = input.type();
    switch (type)
    {
    case token_true:
        data.value() = true;
        input.next();
        break;

    case token_false:
        data.value() = false;
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
