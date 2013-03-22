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

#include <protoc/output.hpp>

namespace protoc
{
namespace transenc
{
const protoc::output::value_type code_false = '\x80';
const protoc::output::value_type code_true = '\x81';
const protoc::output::value_type code_null = '\x82';
const protoc::output::value_type code_tuple_begin = '\x90';
const protoc::output::value_type code_array_begin = '\x91';
const protoc::output::value_type code_tuple_end = '\x98';
const protoc::output::value_type code_array_end = '\x99';
const protoc::output::value_type code_int8 = '\xA0';
const protoc::output::value_type code_binary_int8 = '\xA8';
const protoc::output::value_type code_string_int8 = '\xA9';
const protoc::output::value_type code_int16 = '\xB0';
const protoc::output::value_type code_binary_int16 = '\xB8';
const protoc::output::value_type code_string_int16 = '\xB9';
const protoc::output::value_type code_int32 = '\xC0';
const protoc::output::value_type code_float32 = '\xC2';
const protoc::output::value_type code_binary_int32 = '\xC8';
const protoc::output::value_type code_string_int32 = '\xC9';
const protoc::output::value_type code_int64 = '\xD0';
const protoc::output::value_type code_float64 = '\xD2';
const protoc::output::value_type code_binary_int64 = '\xD8';
const protoc::output::value_type code_string_int64 = '\xD9';

} // namespace transenc
} // namespace protoc

#endif // PROTOC_TRANSENC_CODES_HPP
