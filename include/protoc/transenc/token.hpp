#ifndef PROTOC_TRANSENC_TOKEN_HPP
#define PROTOC_TRANSENC_TOKEN_HPP

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

namespace protoc
{
namespace transenc
{

enum token
{
    token_eof,
    token_error,

    token_null,
    token_true,
    token_false,

    token_int8,
    token_int16,
    token_int32,
    token_int64,
    token_int128,

    token_float32,
    token_float64,

    token_string
};

}
}

#endif /* PROTOC_TRANSENC_TOKEN_HPP */
