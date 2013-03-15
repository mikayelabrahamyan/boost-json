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

#include <boost/test/unit_test.hpp>

#include <sstream>
#include <protoc/exceptions.hpp>
#include <protoc/ubjson/iarchive.hpp>

using namespace protoc;

BOOST_AUTO_TEST_SUITE(ubjson_iarchive_suite)

//-----------------------------------------------------------------------------
// Basic types
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_false)
{
    const char input[] = "F";
    ubjson::iarchive in(input, input + sizeof(input));
    bool value = true;
    in >> boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(value, false);
}

BOOST_AUTO_TEST_CASE(test_true)
{
    const char input[] = "T";
    ubjson::iarchive in(input, input + sizeof(input));
    bool value = false;
    in >> boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(value, true);
}

BOOST_AUTO_TEST_CASE(test_bool_junk)
{
    const char input[] = "Z";
    ubjson::iarchive in(input, input + sizeof(input));
    bool value = true;
    BOOST_REQUIRE_THROW(in >> boost::serialization::make_nvp("value", value),
                        unexpected_token);
}

//-----------------------------------------------------------------------------
// Integers
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_int8_one)
{
    const char input[] = "B\x01";
    ubjson::iarchive in(input, input + sizeof(input));
    protoc::int8_t value = 99;
    in >> boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(value, 1);
}

BOOST_AUTO_TEST_CASE(test_int16_one)
{
    const char input[] = "i\x00\x01";
    ubjson::iarchive in(input, input + sizeof(input));
    protoc::int16_t value = 99;
    in >> boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(value, 1);
}

BOOST_AUTO_TEST_CASE(test_int16_one_int8)
{
    const char input[] = "B\x01";
    ubjson::iarchive in(input, input + sizeof(input));
    protoc::int16_t value = 99;
    in >> boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(value, 1);
}

BOOST_AUTO_TEST_CASE(test_int32_one)
{
    const char input[] = "I\x00\x00\x00\x01";
    ubjson::iarchive in(input, input + sizeof(input));
    protoc::int32_t value = 99;
    in >> boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(value, 1);
}

BOOST_AUTO_TEST_CASE(test_int32_one_int16)
{
    const char input[] = "i\x00\x01";
    ubjson::iarchive in(input, input + sizeof(input));
    protoc::int32_t value = 99;
    in >> boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(value, 1);
}

BOOST_AUTO_TEST_CASE(test_int32_one_int8)
{
    const char input[] = "B\x01";
    ubjson::iarchive in(input, input + sizeof(input));
    protoc::int32_t value = 99;
    in >> boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(value, 1);
}

BOOST_AUTO_TEST_CASE(test_int64_one)
{
    const char input[] = "L\x00\x00\x00\x00\x00\x00\x00\x01";
    ubjson::iarchive in(input, input + sizeof(input));
    protoc::int64_t value = 99;
    in >> boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(value, 1);
}

BOOST_AUTO_TEST_CASE(test_int64_one_int32)
{
    const char input[] = "I\x00\x00\x00\x01";
    ubjson::iarchive in(input, input + sizeof(input));
    protoc::int64_t value = 99;
    in >> boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(value, 1);
}

BOOST_AUTO_TEST_CASE(test_int64_one_int16)
{
    const char input[] = "i\x00\x01";
    ubjson::iarchive in(input, input + sizeof(input));
    protoc::int64_t value = 99;
    in >> boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(value, 1);
}

BOOST_AUTO_TEST_CASE(test_int64_one_int8)
{
    const char input[] = "B\x01";
    ubjson::iarchive in(input, input + sizeof(input));
    protoc::int64_t value = 99;
    in >> boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(value, 1);
}

//-----------------------------------------------------------------------------
// Floating-point
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_float_one)
{
    const char input[] = "d\x3F\x80\x00\x00";
    ubjson::iarchive in(input, input + sizeof(input));
    protoc::float32_t value = 0.0f;
    in >> boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(value, 1.0f);
}

BOOST_AUTO_TEST_CASE(test_double_one)
{
    const char input[] = "D\x3F\xF0\x00\x00\x00\x00\x00\x00";
    ubjson::iarchive in(input, input + sizeof(input));
    protoc::float64_t value = 0.0;
    in >> boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(value, 1.0);
}

BOOST_AUTO_TEST_CASE(test_double_one_float)
{
    const char input[] = "d\x3F\x80\x00\x00";
    ubjson::iarchive in(input, input + sizeof(input));
    protoc::float64_t value = 0.0;
    in >> boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(value, 1.0);
}

BOOST_AUTO_TEST_SUITE_END()
