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
#include <protoc/transenc/detail/codes.hpp>
#include <protoc/transenc/stream_oarchive.hpp>
#include <protoc/transenc/vector_oarchive.hpp>
#include <protoc/transenc/string.hpp>
#include <protoc/transenc/vector.hpp>
#include <protoc/transenc/set.hpp>
#include <protoc/transenc/map.hpp>
#include <protoc/transenc/optional.hpp>
#include <protoc/serialization/nvp.hpp>

namespace format = protoc::transenc;
namespace detail = format::detail;

BOOST_AUTO_TEST_SUITE(transenc_oarchive_suite)

//-----------------------------------------------------------------------------
// Archive types
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_vector_oarchive)
{
    std::vector<unsigned char> result;
    format::vector_oarchive ar(result);

    bool value = false;
    ar << value;

    unsigned char expected[] = { detail::code_false };
    BOOST_REQUIRE_EQUAL_COLLECTIONS(result.begin(), result.end(),
                                    expected, expected + sizeof(expected));
}

//-----------------------------------------------------------------------------
// Basic types
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_empty)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);

    char expected[] = { };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_false)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    bool value = false;
    ar << value;

    char expected[] = { detail::code_false };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_true)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    bool value = true;
    ar << value;

    char expected[] = { detail::code_true };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_const_false)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    const bool value = false;
    ar << value;

    char expected[] = { detail::code_false };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_const_true)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    const bool value = true;
    ar << value;

    char expected[] = { detail::code_true };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

//-----------------------------------------------------------------------------
// Integers
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_int_zero)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    int value = 0;
    ar << value;

    char expected[] = { 0x00 };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_const_int_zero)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    const int value = 0;
    ar << value;

    char expected[] = { 0x00 };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_int_one)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    int value = 1;
    ar << value;

    char expected[] = { 0x01 };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_int_minus_one)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    int value = -1;
    ar << value;

    char expected[] = { 0xFF };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_int_minus_128)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    int value = -128;
    ar << value;

    char expected[] = { detail::code_int8, 0x80 };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_int16)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    int value = 1 << 8;
    ar << value;

    char expected[] = { detail::code_int16, 0x00, 0x01 };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_const_int16)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    const int value = 1 << 8;
    ar << value;

    char expected[] = { detail::code_int16, 0x00, 0x01 };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_int32)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    int value = 1 << 16;
    ar << value;

    char expected[] = { detail::code_int32, 0x00, 0x00, 0x01, 0x00 };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_const_int32)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    const int value = 1 << 16;
    ar << value;

    char expected[] = { detail::code_int32, 0x00, 0x00, 0x01, 0x00 };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_int64)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    long long value = 1LL << 32;
    ar << value;

    char expected[] = { detail::code_int64, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00 };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_const_int64)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    const long long value = 1LL << 32;
    ar << value;

    char expected[] = { detail::code_int64, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00 };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_int_all_types)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    int alpha = 1;
    int bravo = 0x0100;
    int charlie = 0x010000;
    long long delta = 0x0100000000LL;
    ar << alpha << bravo << charlie << delta;


    char expected[] = { 0x01,
                        detail::code_int16, 0x00, 0x01,
                        detail::code_int32, 0x00, 0x00, 0x01, 0x00,
                        detail::code_int64, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00 };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

//-----------------------------------------------------------------------------
// Floating-point
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_float32_one)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    protoc::float32_t value = 1.0f;
    ar << value;

    char expected[] = { detail::code_float32, 0x00, 0x00, 0x80, 0x3F };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_const_float32_one)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    const protoc::float32_t value = 1.0f;
    ar << value;

    char expected[] = { detail::code_float32, 0x00, 0x00, 0x80, 0x3F };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_float64_one)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    protoc::float64_t value = 1.0;
    ar << value;

    char expected[] = { detail::code_float64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x3F };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_const_float64_one)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    const protoc::float64_t value = 1.0;
    ar << value;

    char expected[] = { detail::code_float64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x3F };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

//-----------------------------------------------------------------------------
// String
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_string_empty)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    std::string value("");
    ar << value;

    char expected[] = { detail::code_string_int8, 0x00 };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_const_string_empty)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    const std::string value("");
    ar << value;

    char expected[] = { detail::code_string_int8, 0x00 };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_string_a)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    std::string value("A");
    ar << value;

    char expected[] = { detail::code_string_int8, 0x01, 0x41 };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_string_alpha)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    std::string value("ALPHA");
    ar << value;

    char expected[] = { detail::code_string_int8, 0x05, 0x41, 0x4C, 0x50, 0x48, 0x41 };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_literal_alpha)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    const char *value = "ALPHA";
    ar << value;

    char expected[] = { detail::code_string_int8, 0x05, 0x41, 0x4C, 0x50, 0x48, 0x41 };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_literal_alpha_2)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    ar << "ALPHA";

    char expected[] = { detail::code_string_int8, 0x05, 0x41, 0x4C, 0x50, 0x48, 0x41 };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

//-----------------------------------------------------------------------------
// Pair
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_pair)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    std::pair<std::string, bool> value("A", true);
    ar << value;

    char expected[] = { detail::code_record_begin, detail::code_string_int8, 0x01, 0x41, detail::code_true, detail::code_record_end };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_const_pair)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    const std::pair<std::string, bool> value("A", true);
    ar << value;

    char expected[] = { detail::code_record_begin, detail::code_string_int8, 0x01, 0x41, detail::code_true, detail::code_record_end };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

//-----------------------------------------------------------------------------
// Optional
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_optional)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    boost::optional<std::string> value("A");
    ar << value;

    char expected[] = { detail::code_string_int8, 0x01, 0x41 };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_optional_null)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    boost::optional<std::string> value;
    ar << value;

    char expected[] = { detail::code_null };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_const_optional)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    const boost::optional<std::string> value("A");
    ar << value;

    char expected[] = { detail::code_string_int8, 0x01, 0x41 };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_const_optional_null)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    const boost::optional<std::string> value;
    ar << value;

    char expected[] = { detail::code_null };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

//-----------------------------------------------------------------------------
// Named value pair
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_nvp)
{
    std::ostringstream result;
    format::stream_oarchive out(result);
    bool value = false;
    out << boost::serialization::make_nvp("value", value);

    char expected[] = { detail::code_false };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

//-----------------------------------------------------------------------------
// Container
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_vector_bool_empty)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
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
    format::stream_oarchive ar(result);
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
    format::stream_oarchive ar(result);
    std::vector<bool> value;
    value.push_back(true);
    value.push_back(false);
    ar << value;

    char expected[] = { detail::code_array_begin, 0x02, detail::code_true, detail::code_false, detail::code_array_end };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_set_int_empty)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    std::set<int> value;
    ar << value;

    char expected[] = { detail::code_array_begin, detail::code_null, detail::code_array_end };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_set_int_one)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    std::set<int> value;
    value.insert(1);
    ar << value;

    char expected[] = { detail::code_array_begin, detail::code_null, 0x01, detail::code_array_end };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_set_int_two)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    std::set<int> value;
    value.insert(1);
    value.insert(2);
    ar << value;

    char expected[] = { detail::code_array_begin, detail::code_null, 0x01, 0x02, detail::code_array_end };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_map_bool_empty)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    std::map<std::string, bool> value;
    ar << value;

    char expected[] = { detail::code_map_begin, detail::code_null, detail::code_map_end };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_map_bool_one)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    std::map<std::string, bool> value;
    value["A"] = true;
    ar << value;

    char expected[] = { detail::code_map_begin, detail::code_null, detail::code_record_begin, detail::code_string_int8, 0x01, 0x41, detail::code_true, detail::code_record_end, detail::code_map_end };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_map_bool_two)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    std::map<std::string, bool> value;
    value["A"] = true;
    value["B"] = false;
    ar << value;

    char expected[] = { detail::code_map_begin, detail::code_null, detail::code_record_begin, detail::code_string_int8, 0x01, 0x41, detail::code_true, detail::code_record_end, detail::code_record_begin, detail::code_string_int8, 0x01, 0x42, detail::code_false, detail::code_record_end, detail::code_map_end };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
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
    format::stream_oarchive ar(result);
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
    format::stream_oarchive ar(result);
    person value("KANT", 127);
    ar << value;

    char expected[] = { detail::code_record_begin, detail::code_string_int8, 0x04, 0x4B, 0x41, 0x4E, 0x54, 0x7F, detail::code_record_end };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_struct_split_person)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    split_person value("KANT", 127);
    ar << value;

    char expected[] = { detail::code_record_begin, detail::code_string_int8, 0x04, 0x4B, 0x41, 0x4E, 0x54, 0x7F, detail::code_record_end };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_vector_of_struct_person)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    std::vector<person> persons;
    persons.push_back(person("KANT", 127));
    ar << persons;

    char expected[] = { detail::code_array_begin, 0x01, detail::code_record_begin, detail::code_string_int8, 0x04, 0x4B, 0x41, 0x4E, 0x54, 0x7F, detail::code_record_end, detail::code_array_end };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}
//-----------------------------------------------------------------------------
// Binary
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_binary_empty)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    std::vector<unsigned char> value;
    ar << value;

    char expected[] = { detail::code_binary_int8, 0x00 };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_binary_one)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    std::vector<unsigned char> value(1, 0xFF);
    ar << value;

    char expected[] = { detail::code_binary_int8, 0x01, 0xFF };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_binary_two)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    std::vector<unsigned char> value(2, 0xFF);
    ar << value;

    char expected[] = { detail::code_binary_int8, 0x02, 0xFF, 0xFF };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_SUITE_END()
