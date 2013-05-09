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
#include <protoc/transenc/oarchive.hpp>

using namespace protoc;

BOOST_AUTO_TEST_SUITE(transenc_oarchive_suite)

//-----------------------------------------------------------------------------
// Basic types
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_empty)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    BOOST_REQUIRE_EQUAL(result.str().data(), "");
}

BOOST_AUTO_TEST_CASE(test_false)
{
    std::ostringstream result;
    transenc::stream_oarchive out(result);
    bool value = false;
    out << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x80");
}

BOOST_AUTO_TEST_CASE(test_true)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    bool value = true;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x81");
}

BOOST_AUTO_TEST_CASE(test_const_false)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    const bool value = false;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x80");
}

BOOST_AUTO_TEST_CASE(test_const_true)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    const bool value = true;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x81");
}

//-----------------------------------------------------------------------------
// Integers
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_int8_zero)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    protoc::int8_t value = 0;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x00");
}

BOOST_AUTO_TEST_CASE(test_const_int8_zero)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    const protoc::int8_t value = 0;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x00");
}

BOOST_AUTO_TEST_CASE(test_int8_one)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    protoc::int8_t value = 1;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x01");
}

BOOST_AUTO_TEST_CASE(test_int8_minus_one)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    protoc::int8_t value = -1;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xFF");
}

BOOST_AUTO_TEST_CASE(test_int8_minus_128)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    protoc::int8_t value = -128;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xA0\x80");
}

BOOST_AUTO_TEST_CASE(test_int16_one)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    protoc::int16_t value = 1;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xB0\x01\x00");
}

BOOST_AUTO_TEST_CASE(test_const_int16_one)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    const protoc::int16_t value = 1;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xB0\x01\x00");
}

BOOST_AUTO_TEST_CASE(test_int32_one)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    protoc::int32_t value = 1;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xC0\x01\x00\x00\x00");
}

BOOST_AUTO_TEST_CASE(test_const_int32_one)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    const protoc::int32_t value = 1;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xC0\x01\x00\x00\x00");
}

BOOST_AUTO_TEST_CASE(test_int64_one)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    protoc::int64_t value = 1;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xD0\x01\x00\x00\x00\x00\x00\x00\x00");
}

BOOST_AUTO_TEST_CASE(test_const_int64_one)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    const protoc::int64_t value = 1;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xD0\x01\x00\x00\x00\x00\x00\x00\x00");
}

BOOST_AUTO_TEST_CASE(test_int_all_types)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    protoc::int8_t alpha = 1;
    protoc::int16_t bravo = 2;
    protoc::int32_t charlie = 3;
    protoc::int64_t delta = 4;
    ar << alpha << bravo << charlie << delta;
    BOOST_REQUIRE_EQUAL(result.str().data(),
                        "\x01"
                        "\xB0\x02\x00"
                        "\xC0\x03\x00\x00\x00"
                        "\xD0\x04\x00\x00\x00\x00\x00\x00\x00");
}

//-----------------------------------------------------------------------------
// Floating-point
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_float32_one)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    protoc::float32_t value = 1.0f;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xC2\x00\x00\x80\x3F");
}

BOOST_AUTO_TEST_CASE(test_const_float32_one)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    const protoc::float32_t value = 1.0f;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xC2\x00\x00\x80\x3F");
}

BOOST_AUTO_TEST_CASE(test_float64_one)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    protoc::float64_t value = 1.0;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xD2\x00\x00\x00\x00\x00\x00\xF0\x3F");
}

BOOST_AUTO_TEST_CASE(test_const_float64_one)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    const protoc::float64_t value = 1.0;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xD2\x00\x00\x00\x00\x00\x00\xF0\x3F");
}

//-----------------------------------------------------------------------------
// String
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_string_empty)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    std::string value("");
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xA9\x00");
}

BOOST_AUTO_TEST_CASE(test_const_string_empty)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    const std::string value("");
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xA9\x00");
}

BOOST_AUTO_TEST_CASE(test_string_a)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    std::string value("a");
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xA9\x01" "a");
}

BOOST_AUTO_TEST_CASE(test_string_alpha)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    std::string value("alpha");
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xA9\x05" "alpha");
}

BOOST_AUTO_TEST_CASE(test_literal_alpha)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    const char *value = "alpha";
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xA9\x05" "alpha");
}

BOOST_AUTO_TEST_CASE(test_literal_alpha_2)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    ar << "alpha";
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xA9\x05" "alpha");
}

//-----------------------------------------------------------------------------
// Pair
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_pair)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    std::pair<std::string, bool> value("A", true);
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x90" "\xA9\x01" "A" "\x81" "\x98");
}

BOOST_AUTO_TEST_CASE(test_const_pair)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    const std::pair<std::string, bool> value("A", true);
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x90" "\xA9\x01" "A" "\x81" "\x98");
}

//-----------------------------------------------------------------------------
// Optional
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_optional)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    boost::optional<std::string> value("A");
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xA9\x01" "A");
}

BOOST_AUTO_TEST_CASE(test_optional_null)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    boost::optional<std::string> value;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x82");
}

BOOST_AUTO_TEST_CASE(test_const_optional)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    const boost::optional<std::string> value("A");
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xA9\x01" "A");
}

BOOST_AUTO_TEST_CASE(test_const_optional_null)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    const boost::optional<std::string> value;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x82");
}

//-----------------------------------------------------------------------------
// Container
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_vector_bool_empty)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    std::vector<bool> value;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x91\x99");
}

BOOST_AUTO_TEST_CASE(test_vector_bool_one)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    std::vector<bool> value;
    value.push_back(true);
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x91\x81\x99");
}

BOOST_AUTO_TEST_CASE(test_vector_bool_two)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    std::vector<bool> value;
    value.push_back(true);
    value.push_back(false);
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x91\x81\x80\x99");
}

BOOST_AUTO_TEST_CASE(test_map_bool_empty)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    std::map<std::string, bool> value;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x91\x99");
}

BOOST_AUTO_TEST_CASE(test_map_bool_one)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    std::map<std::string, bool> value;
    value["A"] = true;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x91\x90" "\xA9\x01" "A" "\x81" "\x98\x99");
}

BOOST_AUTO_TEST_CASE(test_map_bool_two)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    std::map<std::string, bool> value;
    value["A"] = true;
    value["B"] = false;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x91\x90" "\xA9\x01" "A" "\x81" "\x98\x90" "\xA9\x01" "B" "\x80" "\x98\x99");
}

//-----------------------------------------------------------------------------
// Enum
//-----------------------------------------------------------------------------

enum Number
{
    one = 1
};

BOOST_AUTO_TEST_CASE(test_enum_one)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    enum Number value = one;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xA3\x01");
}

//-----------------------------------------------------------------------------
// Struct
//-----------------------------------------------------------------------------

struct person
{
    person(const std::string& name, int age)
        : name(name),
          age(age)
    {}

    template<typename T>
    void serialize(T& archive, const unsigned int)
    {
        archive & name;
        archive & age;
    }

    std::string name;
    protoc::int16_t age;
};

BOOST_AUTO_TEST_CASE(test_struct_person)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    person value("Kant", 127);
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x90" "\xA9\x04" "Kant" "\xB0\x7F\x00" "\x98");
}

//-----------------------------------------------------------------------------
// Binary
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_binary_empty)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    std::vector<protoc::int8_t> value;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xA8\x00");
}

BOOST_AUTO_TEST_SUITE_END()
