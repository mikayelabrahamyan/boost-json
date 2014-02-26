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

#include <protoc/types.hpp>

namespace protoc
{
namespace transenc
{

typedef unsigned char value_type;

const value_type code_false = 0x80;
const value_type code_true = 0x81;
const value_type code_null = 0x82;
const value_type code_record_begin = 0x90;
const value_type code_record_end = 0x91;
const value_type code_array_begin = 0x92;
const value_type code_array_end = 0x93;
const value_type code_map_begin = 0x9C;
const value_type code_map_end = 0x9D;
const value_type code_int8 = 0xA0;
const value_type code_string_int8 = 0xA9;
const value_type code_binary_int8 = 0xAB;
const value_type code_int16 = 0xB0;
const value_type code_string_int16 = 0xB9;
const value_type code_binary_int16 = 0xBB;
const value_type code_int32 = 0xC0;
const value_type code_float32 = 0xC2;
const value_type code_string_int32 = 0xC9;
const value_type code_binary_int32 = 0xCB;
const value_type code_int64 = 0xD0;
const value_type code_float64 = 0xD2;
const value_type code_string_int64 = 0xD9;
const value_type code_binary_int64 = 0xDB;

} // namespace transenc
} // namespace protoc

#endif // PROTOC_TRANSENC_CODES_HPP
