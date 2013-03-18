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
    transenc::oarchive ar(result);
    BOOST_REQUIRE_EQUAL(result.str().data(), "");
}

BOOST_AUTO_TEST_CASE(test_false)
{
    std::ostringstream result;
    transenc::oarchive out(result);
    bool value = false;
    out << boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x80");
}

BOOST_AUTO_TEST_CASE(test_true)
{
    std::ostringstream result;
    transenc::oarchive ar(result);
    bool value = true;
    ar << boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x81");
}

BOOST_AUTO_TEST_CASE(test_const_false)
{
    std::ostringstream result;
    transenc::oarchive ar(result);
    const bool value = false;
    ar << boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x80");
}

BOOST_AUTO_TEST_CASE(test_const_true)
{
    std::ostringstream result;
    transenc::oarchive ar(result);
    const bool value = true;
    ar << boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x81");
}

//-----------------------------------------------------------------------------
// Integers
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_int8_zero)
{
    std::ostringstream result;
    transenc::oarchive ar(result);
    protoc::int8_t value = 0;
    ar << boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x00");
}

BOOST_AUTO_TEST_CASE(test_const_int8_zero)
{
    std::ostringstream result;
    transenc::oarchive ar(result);
    const protoc::int8_t value = 0;
    ar << boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x00");
}

BOOST_AUTO_TEST_CASE(test_int8_one)
{
    std::ostringstream result;
    transenc::oarchive ar(result);
    protoc::int8_t value = 1;
    ar << boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x01");
}

BOOST_AUTO_TEST_CASE(test_int8_minus_one)
{
    std::ostringstream result;
    transenc::oarchive ar(result);
    protoc::int8_t value = -1;
    ar << boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xFF");
}

BOOST_AUTO_TEST_CASE(test_int8_minus_128)
{
    std::ostringstream result;
    transenc::oarchive ar(result);
    protoc::int8_t value = -128;
    ar << boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xA0\x80");
}

BOOST_AUTO_TEST_CASE(test_int16_one)
{
    std::ostringstream result;
    transenc::oarchive ar(result);
    protoc::int16_t value = 1;
    ar << boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xB0\x00\x00");
}

BOOST_AUTO_TEST_CASE(test_const_int16_one)
{
    std::ostringstream result;
    transenc::oarchive ar(result);
    const protoc::int16_t value = 1;
    ar << boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xB0\x00\x00");
}

BOOST_AUTO_TEST_CASE(test_int32_one)
{
    std::ostringstream result;
    transenc::oarchive ar(result);
    protoc::int32_t value = 1;
    ar << boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xC0\x00\x00\x00\x00");
}

BOOST_AUTO_TEST_CASE(test_const_int32_one)
{
    std::ostringstream result;
    transenc::oarchive ar(result);
    const protoc::int32_t value = 1;
    ar << boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xC0\x00\x00\x00\x00");
}

BOOST_AUTO_TEST_CASE(test_int64_one)
{
    std::ostringstream result;
    transenc::oarchive ar(result);
    protoc::int64_t value = 1;
    ar << boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xD0\x00\x00\x00\x00\x00\x00\x00\x00");
}

BOOST_AUTO_TEST_CASE(test_const_int64_one)
{
    std::ostringstream result;
    transenc::oarchive ar(result);
    const protoc::int64_t value = 1;
    ar << boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xD0\x00\x00\x00\x00\x00\x00\x00\x00");
}

BOOST_AUTO_TEST_CASE(test_int_all_types)
{
    std::ostringstream result;
    transenc::oarchive ar(result);
    protoc::int8_t alpha = 1;
    protoc::int16_t bravo = 2;
    protoc::int32_t charlie = 3;
    protoc::int64_t delta = 4;
    ar << boost::serialization::make_nvp("alpha", alpha);
    ar << boost::serialization::make_nvp("bravo", bravo);
    ar << boost::serialization::make_nvp("charlie", charlie);
    ar << boost::serialization::make_nvp("delta", delta);
    BOOST_REQUIRE_EQUAL(result.str().data(),
                        "\x01"
                        "\xB0\x00\x02"
                        "\xC0\x00\x00\x00\x03"
                        "\xD0\x00\x00\x00\x00\x00\x00\x00\x04");
}

//-----------------------------------------------------------------------------
// Floating-point
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_float32_one)
{
    std::ostringstream result;
    transenc::oarchive ar(result);
    protoc::float32_t value = 1.0f;
    ar << boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xC2\x3F\x80\x00\x00");
}

BOOST_AUTO_TEST_CASE(test_const_float32_one)
{
    std::ostringstream result;
    transenc::oarchive ar(result);
    const protoc::float32_t value = 1.0f;
    ar << boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xC2\x3F\x80\x00\x00");
}

BOOST_AUTO_TEST_CASE(test_float64_one)
{
    std::ostringstream result;
    transenc::oarchive ar(result);
    protoc::float64_t value = 1.0;
    ar << boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xD2\x3F\xF0\x00\x00\x00\x00\x00\x00");
}

BOOST_AUTO_TEST_CASE(test_const_float64_one)
{
    std::ostringstream result;
    transenc::oarchive ar(result);
    const protoc::float64_t value = 1.0;
    ar << boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xD2\x3F\xF0\x00\x00\x00\x00\x00\x00");
}

//-----------------------------------------------------------------------------
// String
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_string_empty)
{
    std::ostringstream result;
    transenc::oarchive ar(result);
    std::string value("");
    ar << boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xA9\x00");
}

BOOST_AUTO_TEST_CASE(test_const_string_empty)
{
    std::ostringstream result;
    transenc::oarchive ar(result);
    const std::string value("");
    ar << boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xA9\x00");
}

BOOST_AUTO_TEST_CASE(test_string_a)
{
    std::ostringstream result;
    transenc::oarchive ar(result);
    std::string value("a");
    ar << boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xA9\x01" "a");
}

BOOST_AUTO_TEST_CASE(test_string_alpha)
{
    std::ostringstream result;
    transenc::oarchive ar(result);
    std::string value("alpha");
    ar << boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xA9\x05" "alpha");
}

//-----------------------------------------------------------------------------
// Container
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_vector_bool_empty)
{
    std::ostringstream result;
    transenc::oarchive ar(result);
    std::vector<bool> value;
    ar << boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x92\x93");
}

BOOST_AUTO_TEST_CASE(test_vector_bool_one)
{
    std::ostringstream result;
    transenc::oarchive ar(result);
    std::vector<bool> value;
    value.push_back(true);
    ar << boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x92\x81\x93");
}

BOOST_AUTO_TEST_CASE(test_vector_bool_two)
{
    std::ostringstream result;
    transenc::oarchive ar(result);
    std::vector<bool> value;
    value.push_back(true);
    value.push_back(false);
    ar << boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x92\x81\x80\x93");
}

BOOST_AUTO_TEST_CASE(test_map_bool_empty)
{
    std::ostringstream result;
    transenc::oarchive ar(result);
    std::map<std::string, bool> value;
    ar << boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x92\x93");
}

BOOST_AUTO_TEST_CASE(test_map_bool_one)
{
    std::ostringstream result;
    transenc::oarchive ar(result);
    std::map<std::string, bool> value;
    value["A"] = true;
    ar << boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x92\x90" "\xA9\x01" "A" "\x81" "\x91\x93");
}

BOOST_AUTO_TEST_CASE(test_map_bool_two)
{
    std::ostringstream result;
    transenc::oarchive ar(result);
    std::map<std::string, bool> value;
    value["A"] = true;
    value["B"] = false;
    ar << boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x92\x90" "\xA9\x01" "A" "\x81" "\x91\x90" "\xA9\x01" "B" "\x80" "\x91\x93");
}

struct person
{
    person(const std::string& name, int age)
        : name(name),
          age(age)
    {}

    template<typename T>
    void serialize(T& archive, const unsigned int)
    {
        archive & boost::serialization::make_nvp("name", name);
        archive & boost::serialization::make_nvp("age", age);
    }

    std::string name;
    protoc::int16_t age;
};

BOOST_AUTO_TEST_CASE(test_struct_person)
{
    std::ostringstream result;
    transenc::oarchive ar(result);
    person value("Kant", 127);
    ar << boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x90" "\xA9\x04" "Kant" "\xB0\x00\x7F" "\x91");
}

BOOST_AUTO_TEST_SUITE_END()
