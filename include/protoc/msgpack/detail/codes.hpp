#ifndef PROTOC_MSGPACK_DETAIL_CODES_HPP
#define PROTOC_MSGPACK_DETAIL_CODES_HPP

///////////////////////////////////////////////////////////////////////////////
//
// http://protoc.sourceforge.net/
//
// Copyright (C) 2014 Bjorn Reese <breese@users.sourceforge.net>
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
namespace msgpack
{
namespace detail
{

typedef unsigned char value_type;

const value_type code_fixarray_0 = 0x90;
const value_type code_fixarray_1 = 0x91;
const value_type code_fixarray_2 = 0x92;
const value_type code_fixarray_3 = 0x93;
const value_type code_fixarray_4 = 0x94;
const value_type code_fixarray_5 = 0x95;
const value_type code_fixarray_6 = 0x96;
const value_type code_fixarray_7 = 0x97;
const value_type code_fixarray_8 = 0x98;
const value_type code_fixarray_9 = 0x99;
const value_type code_fixarray_10 = 0x9A;
const value_type code_fixarray_11 = 0x9B;
const value_type code_fixarray_12 = 0x9C;
const value_type code_fixarray_13 = 0x9D;
const value_type code_fixarray_14 = 0x9E;
const value_type code_fixarray_15 = 0x9F;
const value_type code_fixstr_0 = 0xA0;
const value_type code_fixstr_1 = 0xA1;
const value_type code_fixstr_2 = 0xA2;
const value_type code_fixstr_3 = 0xA3;
const value_type code_fixstr_4 = 0xA4;
const value_type code_fixstr_5 = 0xA5;
const value_type code_fixstr_6 = 0xA6;
const value_type code_fixstr_7 = 0xA7;
const value_type code_fixstr_8 = 0xA8;
const value_type code_fixstr_9 = 0xA9;
const value_type code_fixstr_10 = 0xAA;
const value_type code_fixstr_11 = 0xAB;
const value_type code_fixstr_12 = 0xAC;
const value_type code_fixstr_13 = 0xAD;
const value_type code_fixstr_14 = 0xAE;
const value_type code_fixstr_15 = 0xAF;
const value_type code_fixstr_16 = 0xB0;
const value_type code_fixstr_17 = 0xB1;
const value_type code_fixstr_18 = 0xB2;
const value_type code_fixstr_19 = 0xB3;
const value_type code_fixstr_20 = 0xB4;
const value_type code_fixstr_21 = 0xB5;
const value_type code_fixstr_22 = 0xB6;
const value_type code_fixstr_23 = 0xB7;
const value_type code_fixstr_24 = 0xB8;
const value_type code_fixstr_25 = 0xB9;
const value_type code_fixstr_26 = 0xBA;
const value_type code_fixstr_27 = 0xBB;
const value_type code_fixstr_28 = 0xBC;
const value_type code_fixstr_29 = 0xBD;
const value_type code_fixstr_30 = 0xBE;
const value_type code_fixstr_31 = 0xBF;
const value_type code_null = 0xC0;
const value_type code_false = 0xC2;
const value_type code_true = 0xC3;
const value_type code_bin8 = 0xC4;
const value_type code_bin16 = 0xC5;
const value_type code_bin32 = 0xC6;
const value_type code_float32 = 0xCA;
const value_type code_float64 = 0xCB;
const value_type code_uint8 = 0xCC;
const value_type code_uint16 = 0xCD;
const value_type code_uint32 = 0xCE;
const value_type code_uint64 = 0xCF;
const value_type code_int8 = 0xD0;
const value_type code_int16 = 0xD1;
const value_type code_int32 = 0xD2;
const value_type code_int64 = 0xD3;
const value_type code_str8 = 0xD9;
const value_type code_str16 = 0xDA;
const value_type code_str32 = 0xDB;
const value_type code_array16 = 0xDC;
const value_type code_array32 = 0xDD;

} // namespace detail
} // namespace msgpack
} // namespace protoc

#endif // PROTOC_MSGPACK_DETAIL_CODES_HPP
