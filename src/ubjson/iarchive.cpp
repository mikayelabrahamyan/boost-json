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

#include <sstream>
#include <protoc/exceptions.hpp>
#include <protoc/ubjson/iarchive.hpp>

namespace protoc
{
namespace ubjson
{

iarchive::iarchive(const std::string& buffer)
    : buffer(buffer),
      input(buffer.data(), buffer.data() + buffer.size())
{
}

iarchive::~iarchive()
{
}

void iarchive::load_override(boost::serialization::nvp<bool> data, int)
{
    ubjson::token type = input.next();
    if (type == token_true)
    {
        data.value() = true;
    }
    else if (type == token_false)
    {
        data.value() = false;
    }
    else
    {
        std::ostringstream error;
        error << type;
        throw unexpected_token(error.str());
    }
}

}
}
