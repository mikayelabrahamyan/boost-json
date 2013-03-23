#ifndef PROTOC_TRANSENC_CODES_HPP
#define PROTOC_TRANSENC_CODES_HPP

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
const char code_false = '\x80';
const char code_true = '\x81';
const char code_null = '\x82';
const char code_tuple_begin = '\x90';
const char code_array_begin = '\x91';
const char code_tuple_end = '\x98';
const char code_array_end = '\x99';
const char code_int8 = '\xA0';
const char code_binary_int8 = '\xA8';
const char code_string_int8 = '\xA9';
const char code_int16 = '\xB0';
const char code_binary_int16 = '\xB8';
const char code_string_int16 = '\xB9';
const char code_int32 = '\xC0';
const char code_float32 = '\xC2';
const char code_binary_int32 = '\xC8';
const char code_string_int32 = '\xC9';
const char code_int64 = '\xD0';
const char code_float64 = '\xD2';
const char code_binary_int64 = '\xD8';
const char code_string_int64 = '\xD9';

} // namespace transenc
} // namespace protoc

#endif // PROTOC_TRANSENC_CODES_HPP
