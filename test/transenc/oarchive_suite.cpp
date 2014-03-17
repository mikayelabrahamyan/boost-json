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
#include <boost/serialization/split_member.hpp>
#include <protoc/exceptions.hpp>
#include <protoc/transenc/codes.hpp>
#include <protoc/transenc/stream_oarchive.hpp>
#include <protoc/transenc/string.hpp>
#include <protoc/transenc/vector.hpp>
#include <protoc/transenc/set.hpp>
#include <protoc/transenc/map.hpp>
#include <protoc/transenc/optional.hpp>
#include <protoc/serialization/nvp.hpp>

using namespace protoc; // FIXME: Remove
namespace format = protoc::transenc;
namespace detail = format; // FIXME ::detail;

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

BOOST_AUTO_TEST_CASE(test_int_zero)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    int value = 0;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x00");
}

BOOST_AUTO_TEST_CASE(test_const_int_zero)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    const int value = 0;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x00");
}

BOOST_AUTO_TEST_CASE(test_int_one)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    int value = 1;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x01");
}

BOOST_AUTO_TEST_CASE(test_int_minus_one)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    int value = -1;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xFF");
}

BOOST_AUTO_TEST_CASE(test_int_minus_128)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    int value = -128;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xA0\x80");
}

BOOST_AUTO_TEST_CASE(test_int16)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    int value = 1 << 8;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xB0\x00\x01");
}

BOOST_AUTO_TEST_CASE(test_const_int16)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    const int value = 1 << 8;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xB0\x00\x01");
}

BOOST_AUTO_TEST_CASE(test_int32)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    int value = 1 << 16;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xC0\x00\x00\x00\x01");
}

BOOST_AUTO_TEST_CASE(test_const_int32)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    const int value = 1 << 16;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xC0\x00\x00\x00\x01");
}

BOOST_AUTO_TEST_CASE(test_int64)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    long long value = 1LL << 32;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xD0\x00\x00\x00\x00\x00\x00\x00\x01");
}

BOOST_AUTO_TEST_CASE(test_const_int64)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    const long long value = 1LL << 32;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xD0\x00\x00\x00\x00\x00\x00\x00\x01");
}

BOOST_AUTO_TEST_CASE(test_int_all_types)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    int alpha = 1;
    int bravo = 0x0100;
    int charlie = 0x010000;
    int delta = 0x01000000;
    ar << alpha << bravo << charlie << delta;
    BOOST_REQUIRE_EQUAL(result.str().data(),
                        "\x01"
                        "\xB0\x00\x01"
                        "\xC0\x00\x01\x00\x00"
                        "\xD0\x00\x00\x00\x01\x00\x00\x00\x00");
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
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x90" "\xA9\x01" "A" "\x81" "\x91");
}

BOOST_AUTO_TEST_CASE(test_const_pair)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    const std::pair<std::string, bool> value("A", true);
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x90" "\xA9\x01" "A" "\x81" "\x91");
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
// Named value pair
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_nvp)
{
    std::ostringstream result;
    transenc::stream_oarchive out(result);
    bool value = false;
    out << boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x80");
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

    char expected[] = { detail::code_array_begin, 0x00, detail::code_array_end };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_vector_bool_one)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    std::vector<bool> value;
    value.push_back(true);
    ar << value;

    char expected[] = { detail::code_array_begin, 0x01, detail::code_true, detail::code_array_end };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_vector_bool_two)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    std::vector<bool> value;
    value.push_back(true);
    value.push_back(false);
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x92\x02\x81\x80\x93");
}

BOOST_AUTO_TEST_CASE(test_set_int_empty)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    std::set<int> value;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x92\x82\x93");
}

BOOST_AUTO_TEST_CASE(test_set_int_one)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    std::set<int> value;
    value.insert(1);
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x92\x82\x01\x93");
}

BOOST_AUTO_TEST_CASE(test_set_int_two)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    std::set<int> value;
    value.insert(1);
    value.insert(2);
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x92\x82\x01\x02\x93");
}

BOOST_AUTO_TEST_CASE(test_map_bool_empty)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    std::map<std::string, bool> value;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x9C\x82\x9D");
}

BOOST_AUTO_TEST_CASE(test_map_bool_one)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    std::map<std::string, bool> value;
    value["A"] = true;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x9C\x82\x90" "\xA9\x01" "A" "\x81" "\x91\x9D");
}

BOOST_AUTO_TEST_CASE(test_map_bool_two)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    std::map<std::string, bool> value;
    value["A"] = true;
    value["B"] = false;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x9C\x82\x90" "\xA9\x01" "A" "\x81" "\x91\x90" "\xA9\x01" "B" "\x80" "\x91\x9D");
}

//-----------------------------------------------------------------------------
// Enum
//-----------------------------------------------------------------------------

#if 0 // FIXME
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
#endif

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
    int age;
};

struct split_person
{
    split_person(const std::string& name, int age)
        : name(name),
          age(age)
    {}

    template<typename T>
    void load(T& archive, const unsigned int)
    {
        archive >> name;
        archive >> age;
    }

    template<typename T>
    void save(T& archive, const unsigned int) const
    {
        archive << name;
        archive << age;
    }

    std::string name;
    int age;

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

BOOST_AUTO_TEST_CASE(test_struct_person)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    person value("Kant", 127);
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x90" "\xA9\x04" "Kant" "\x7F" "\x91");
}

BOOST_AUTO_TEST_CASE(test_struct_split_person)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    split_person value("Kant", 127);
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\x90" "\xA9\x04" "Kant" "\x7F" "\x91");
}

//-----------------------------------------------------------------------------
// Binary
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_binary_empty)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    std::vector<unsigned char> value;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xAB\x00");
}

BOOST_AUTO_TEST_CASE(test_binary_one)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    std::vector<unsigned char> value(1, 0xFF);
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xAB\x01\xFF");
}

BOOST_AUTO_TEST_CASE(test_binary_two)
{
    std::ostringstream result;
    transenc::stream_oarchive ar(result);
    std::vector<unsigned char> value(2, 0xFF);
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\xAB\x02\xFF\xFF");
}

BOOST_AUTO_TEST_SUITE_END()
