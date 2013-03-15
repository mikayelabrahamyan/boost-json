#ifndef PROTOC_EXCEPTIONS_HPP
#define PROTOC_EXCEPTIONS_HPP

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

#include <stdexcept>

namespace protoc
{

class unexpected_token : public std::runtime_error
{
public:
    explicit unexpected_token(const std::string& message)
        : std::runtime_error(message)
    {}

    ~unexpected_token() throw()
    {}
};

}

#endif // PROTOC_EXCEPTIONS_HPP
