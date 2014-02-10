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
#include <protoc/transenc/iarchive.hpp>
#include <protoc/transenc/string.hpp>
#include <protoc/transenc/vector.hpp>
#include <protoc/transenc/set.hpp>
#include <protoc/transenc/map.hpp>
#include <protoc/transenc/optional.hpp>
#include <protoc/serialization/nvp.hpp>

using namespace protoc;

BOOST_AUTO_TEST_SUITE(transenc_iarchive_suite)

//-----------------------------------------------------------------------------
// Basic types
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_false)
{
    const protoc::uint8_t input[] = { transenc::code_false };
    transenc::iarchive in(input, input + sizeof(input));
    bool value = true;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value, false);
}

BOOST_AUTO_TEST_CASE(test_true)
{
    const protoc::uint8_t input[] = { transenc::code_true };
    transenc::iarchive in(input, input + sizeof(input));
    bool value = false;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value, true);
}

BOOST_AUTO_TEST_CASE(test_bool_junk)
{
    const protoc::uint8_t input[] = { transenc::code_null }; // Null cannot be deserialized as bool (only as optional<bool>)
    transenc::iarchive in(input, input + sizeof(input));
    bool value = true;
    BOOST_REQUIRE_THROW(in >> value,
                        invalid_value);
}

//-----------------------------------------------------------------------------
// Integers
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_int_zero)
{
    const protoc::uint8_t input[] = { 0x00 };
    transenc::iarchive in(input, input + sizeof(input));
    int value = 99;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value, 0);
}

BOOST_AUTO_TEST_CASE(test_int_one)
{
    const protoc::uint8_t input[] = { 0x01 };
    transenc::iarchive in(input, input + sizeof(input));
    int value = 99;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value, 1);
}

BOOST_AUTO_TEST_CASE(test_int_minus_one)
{
    const protoc::uint8_t input[] = { '\xFF' };
    transenc::iarchive in(input, input + sizeof(input));
    int value = 99;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value, -1);
}

BOOST_AUTO_TEST_CASE(test_int_one_int8)
{
    const protoc::uint8_t input[] = { transenc::code_int8, 0x01 };
    transenc::iarchive in(input, input + sizeof(input));
    int value = 99;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value, 1);
}

BOOST_AUTO_TEST_CASE(test_int_minus_one_int8)
{
    const protoc::uint8_t input[] = { transenc::code_int8, '\xFF' };
    transenc::iarchive in(input, input + sizeof(input));
    int value = 99;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value, -1);
}

BOOST_AUTO_TEST_CASE(test_int_one_int16)
{
    const protoc::uint8_t input[] = { transenc::code_int16, 0x01, 0x00 };
    transenc::iarchive in(input, input + sizeof(input));
    int value = 99;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value, 1);
}

BOOST_AUTO_TEST_CASE(test_int_minus_one_int16)
{
    const protoc::uint8_t input[] = { transenc::code_int16, '\xFF', '\xFF' };
    transenc::iarchive in(input, input + sizeof(input));
    int value = 99;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value, -1);
}

BOOST_AUTO_TEST_CASE(test_int_one_int32)
{
    const protoc::uint8_t input[] = { transenc::code_int32, 0x01, 0x00, 0x00, 0x00 };
    transenc::iarchive in(input, input + sizeof(input));
    int value = 99;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value, 1);
}

BOOST_AUTO_TEST_CASE(test_int_minus_one_int32)
{
    const protoc::uint8_t input[] = { transenc::code_int32, '\xFF', '\xFF', '\xFF', '\xFF' };
    transenc::iarchive in(input, input + sizeof(input));
    int value = 99;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value, -1);
}

BOOST_AUTO_TEST_CASE(test_longlong_one_int8_small)
{
    const protoc::uint8_t input[] = { 0x01 };
    transenc::iarchive in(input, input + sizeof(input));
    long long value = 99;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value, 1);
}

BOOST_AUTO_TEST_CASE(test_longlong_one_int8)
{
    const protoc::uint8_t input[] = { transenc::code_int8, 0x01 };
    transenc::iarchive in(input, input + sizeof(input));
    long long value = 99;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value, 1);
}

BOOST_AUTO_TEST_CASE(test_longlong_one_int16)
{
    const protoc::uint8_t input[] = { transenc::code_int16, 0x01, 0x00 };
    transenc::iarchive in(input, input + sizeof(input));
    long long value = 99;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value, 1);
}

BOOST_AUTO_TEST_CASE(test_longlong_one_int32)
{
    const protoc::uint8_t input[] = { transenc::code_int32, 0x01, 0x00, 0x00, 0x00 };
    transenc::iarchive in(input, input + sizeof(input));
    long long value = 99;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value, 1);
}

BOOST_AUTO_TEST_CASE(test_longlong_one_int64)
{
    const protoc::uint8_t input[] = { transenc::code_int64, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    transenc::iarchive in(input, input + sizeof(input));
    long long value = 99;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value, 1);
}

//-----------------------------------------------------------------------------
// Floating-point
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_float_one)
{
    const protoc::uint8_t input[] = { transenc::code_float32, 0x00, 0x00, 0x80, 0x3F };
    transenc::iarchive in(input, input + sizeof(input));
    protoc::float32_t value = 0.0f;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value, 1.0f);
}

BOOST_AUTO_TEST_CASE(test_double_one)
{
    const protoc::uint8_t input[] = { transenc::code_float64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x3F };
    transenc::iarchive in(input, input + sizeof(input));
    protoc::float64_t value = 0.0;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value, 1.0);
}

BOOST_AUTO_TEST_CASE(test_double_one_float)
{
    const protoc::uint8_t input[] = { transenc::code_float32, 0x00, 0x00, 0x80, 0x3F };
    transenc::iarchive in(input, input + sizeof(input));
    protoc::float64_t value = 0.0;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value, 1.0);
}

//-----------------------------------------------------------------------------
// String
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_string_empty)
{
    const protoc::uint8_t input[] = { transenc::code_string_int8, 0x00 };
    transenc::iarchive in(input, input + sizeof(input));
    std::string value("replace");
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value, "");
}

BOOST_AUTO_TEST_CASE(test_string_alpha)
{
    const protoc::uint8_t input[] = { transenc::code_string_int8, 0x05, 'a', 'l', 'p', 'h', 'a' };
    transenc::iarchive in(input, input + sizeof(input));
    std::string value;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value, "alpha");
}

//-----------------------------------------------------------------------------
// Pair
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_pair)
{
    const protoc::uint8_t input[] = { transenc::code_record_begin, transenc::code_string_int8, 0x01, 'A', transenc::code_true, transenc::code_record_end };
    transenc::iarchive in(input, input + sizeof(input));
    std::pair<std::string, bool> value("replace", false);
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value.first, "A");
    BOOST_REQUIRE_EQUAL(value.second, true);
}

BOOST_AUTO_TEST_CASE(test_pair_too_short)
{
    const protoc::uint8_t input[] = { transenc::code_record_begin, transenc::code_string_int8, 0x01, 'A', transenc::code_record_end };
    transenc::iarchive in(input, input + sizeof(input));
    std::pair<std::string, bool> value;
    BOOST_REQUIRE_THROW(in >> value,
                        invalid_value);
}

BOOST_AUTO_TEST_CASE(test_pair_too_long)
{
    const protoc::uint8_t input[] = { transenc::code_record_begin, transenc::code_string_int8, 0x01, 'A', transenc::code_true, transenc::code_false, transenc::code_record_end };
    transenc::iarchive in(input, input + sizeof(input));
    std::pair<std::string, bool> value;
    BOOST_REQUIRE_THROW(in >> value,
                        unexpected_token);
}

BOOST_AUTO_TEST_CASE(test_pair_missing_end)
{
    const protoc::uint8_t input[] = { transenc::code_record_begin, transenc::code_string_int8, 0x01, 'A', transenc::code_true };
    transenc::iarchive in(input, input + sizeof(input));
    std::pair<std::string, bool> value;
    BOOST_REQUIRE_THROW(in >> value,
                        unexpected_token);
}

BOOST_AUTO_TEST_CASE(test_pair_missing_end_2)
{
    const protoc::uint8_t input[] = { transenc::code_record_begin, transenc::code_string_int8, 0x01, 'A' };
    transenc::iarchive in(input, input + sizeof(input));
    std::pair<std::string, bool> value;
    BOOST_REQUIRE_THROW(in >> value,
                        invalid_value);
}

BOOST_AUTO_TEST_CASE(test_pair_missing_end_3)
{
    const protoc::uint8_t input[] = { transenc::code_record_begin };
    transenc::iarchive in(input, input + sizeof(input));
    std::pair<std::string, bool> value;
    BOOST_REQUIRE_THROW(in >> value,
                        invalid_value);
}

BOOST_AUTO_TEST_CASE(test_pair_missing_begin)
{
    const protoc::uint8_t input[] = { transenc::code_string_int8, 0x01, 'A', transenc::code_true, transenc::code_record_end };
    transenc::iarchive in(input, input + sizeof(input));
    std::pair<std::string, bool> value;
    BOOST_REQUIRE_THROW(in >> value,
                        unexpected_token);
}

//-----------------------------------------------------------------------------
// Optional
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_optional)
{
    const protoc::uint8_t input[] = { transenc::code_string_int8, 0x01, 'A' };
    transenc::iarchive in(input, input + sizeof(input));
    boost::optional<std::string> value;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE(value);
    BOOST_REQUIRE_EQUAL(*value, "A");
}

BOOST_AUTO_TEST_CASE(test_optional_null)
{
    const protoc::uint8_t input[] = { transenc::code_null };
    transenc::iarchive in(input, input + sizeof(input));
    boost::optional<std::string> value;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE(!value);
}

BOOST_AUTO_TEST_CASE(test_optional_null_value)
{
    const protoc::uint8_t input[] = { transenc::code_null, transenc::code_true };
    transenc::iarchive in(input, input + sizeof(input));
    boost::optional<bool> value;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE(!value);
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE(value);
    BOOST_REQUIRE_EQUAL(*value, true);
}

BOOST_AUTO_TEST_CASE(test_optional_wrong_type)
{
    const protoc::uint8_t input[] = { transenc::code_true };
    transenc::iarchive in(input, input + sizeof(input));
    boost::optional<std::string> value;
    BOOST_REQUIRE_THROW(in >> value,
                        invalid_value);
}

//-----------------------------------------------------------------------------
// Named value pair
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_nvp)
{
    const protoc::uint8_t input[] = { transenc::code_false };
    transenc::iarchive in(input, input + sizeof(input));
    bool value = true;
    BOOST_REQUIRE_NO_THROW(in >> boost::serialization::make_nvp("value", value));
    BOOST_REQUIRE_EQUAL(value, false);
}

//-----------------------------------------------------------------------------
// Container
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_vector_bool_empty)
{
    const protoc::uint8_t input[] = { transenc::code_array_begin, 0x00, transenc::code_array_end };
    transenc::iarchive in(input, input + sizeof(input));
    std::vector<bool> value;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_vector_bool_one)
{
    const protoc::uint8_t input[] = { transenc::code_array_begin, 0x01, transenc::code_true, transenc::code_array_end };
    transenc::iarchive in(input, input + sizeof(input));
    std::vector<bool> value;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value.size(), 1);
    BOOST_REQUIRE_EQUAL(value[0], true);
}

BOOST_AUTO_TEST_CASE(test_vector_bool_two)
{
    const protoc::uint8_t input[] = { transenc::code_array_begin, 0x02, transenc::code_true, transenc::code_false, transenc::code_array_end };
    transenc::iarchive in(input, input + sizeof(input));
    std::vector<bool> value;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value.size(), 2);
    BOOST_REQUIRE_EQUAL(value[0], true);
    BOOST_REQUIRE_EQUAL(value[1], false);
}

BOOST_AUTO_TEST_CASE(test_vector_mixed)
{
    const protoc::uint8_t input[] = { transenc::code_array_begin, 0x02, transenc::code_true, 0x00, transenc::code_array_end };
    transenc::iarchive in(input, input + sizeof(input));
    std::vector<bool> value;
    BOOST_REQUIRE_THROW(in >> value,
                        invalid_value);
}

BOOST_AUTO_TEST_CASE(test_vector_missing_end)
{
    const protoc::uint8_t input[] = { transenc::code_array_begin, 0x01, transenc::code_true };
    transenc::iarchive in(input, input + sizeof(input));
    std::vector<bool> value;
    BOOST_REQUIRE_THROW(in >> value,
                        invalid_value);
}

BOOST_AUTO_TEST_CASE(test_vector_missing_begin)
{
    const protoc::uint8_t input[] = { transenc::code_true, transenc::code_array_end };
    transenc::iarchive in(input, input + sizeof(input));
    std::vector<bool> value;
    BOOST_REQUIRE_THROW(in >> value,
                        unexpected_token);
}

BOOST_AUTO_TEST_CASE(test_set_int_empty)
{
    const protoc::uint8_t input[] = { transenc::code_array_begin, transenc::code_null, transenc::code_array_end };
    transenc::iarchive in(input, input + sizeof(input));
    std::set<int> value;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_set_int_one)
{
    const protoc::uint8_t input[] = { transenc::code_array_begin, transenc::code_null, 0x11, transenc::code_array_end };
    transenc::iarchive in(input, input + sizeof(input));
    std::set<int> value;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value.size(), 1);
    BOOST_REQUIRE_EQUAL(value.count(0x11), 1);
}

BOOST_AUTO_TEST_CASE(test_set_int_two)
{
    const protoc::uint8_t input[] = { transenc::code_array_begin, transenc::code_null, 0x11, 0x22, transenc::code_array_end };
    transenc::iarchive in(input, input + sizeof(input));
    std::set<int> value;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value.size(), 2);
    BOOST_REQUIRE_EQUAL(value.count(0x11), 1);
    BOOST_REQUIRE_EQUAL(value.count(0x22), 1);
}

BOOST_AUTO_TEST_CASE(test_map_bool_empty)
{
    const protoc::uint8_t input[] = { transenc::code_map_begin, 0x00, transenc::code_map_end };
    transenc::iarchive in(input, input + sizeof(input));
    std::map<std::string, bool> value;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_map_bool_empty_null)
{
    const protoc::uint8_t input[] = { transenc::code_map_begin, transenc::code_null, transenc::code_map_end };
    transenc::iarchive in(input, input + sizeof(input));
    std::map<std::string, bool> value;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_map_bool_one)
{
    const protoc::uint8_t input[] = { transenc::code_map_begin, 0x01, transenc::code_record_begin, transenc::code_string_int8, 0x01, 'A', transenc::code_true, transenc::code_record_end, transenc::code_map_end };
    transenc::iarchive in(input, input + sizeof(input));
    std::map<std::string, bool> value;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value.size(), 1);
    BOOST_REQUIRE_EQUAL(value["A"], true);
}

BOOST_AUTO_TEST_CASE(test_map_bool_two)
{
    const protoc::uint8_t input[] = { transenc::code_map_begin, 0x02, transenc::code_record_begin, transenc::code_string_int8, 0x01, 'A', transenc::code_true, transenc::code_record_end, transenc::code_record_begin, transenc::code_string_int8, 0x01, 'B', transenc::code_false, transenc::code_record_end, transenc::code_map_end };
    transenc::iarchive in(input, input + sizeof(input));
    std::map<std::string, bool> value;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value.size(), 2);
    BOOST_REQUIRE_EQUAL(value["A"], true);
    BOOST_REQUIRE_EQUAL(value["B"], false);
}

BOOST_AUTO_TEST_CASE(test_map_missing_end)
{
    const protoc::uint8_t input[] = { transenc::code_map_begin, 0x01, transenc::code_record_begin, transenc::code_string_int8, 0x01, 'A', transenc::code_true, transenc::code_record_end };
    transenc::iarchive in(input, input + sizeof(input));
    std::map<std::string, bool> value;
    BOOST_REQUIRE_THROW(in >> value,
                        unexpected_token);
}

BOOST_AUTO_TEST_CASE(test_map_missing_pair_end)
{
    const protoc::uint8_t input[] = { transenc::code_map_begin, 0x01, transenc::code_record_begin, transenc::code_string_int8, 0x01, 'A', transenc::code_true, transenc::code_map_end };
    transenc::iarchive in(input, input + sizeof(input));
    std::map<std::string, bool> value;
    BOOST_REQUIRE_THROW(in >> value,
                        unexpected_token);
}

BOOST_AUTO_TEST_CASE(test_map_missing_pair_end_2)
{
    const protoc::uint8_t input[] = { transenc::code_map_begin, 0x01, transenc::code_record_begin, transenc::code_string_int8, 0x01, 'A', transenc::code_true };
    transenc::iarchive in(input, input + sizeof(input));
    std::map<std::string, bool> value;
    BOOST_REQUIRE_THROW(in >> value,
                        unexpected_token);
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
    const protoc::uint8_t input[] = { transenc::code_record_begin, transenc::code_string_int8, 0x04, 'K', 'A', 'N', 'T', transenc::code_int16, 0x7F, 0x00, transenc::code_record_end };
    transenc::iarchive in(input, input + sizeof(input));
    person value("", 99);
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value.name, "KANT");
    BOOST_REQUIRE_EQUAL(value.age, 127);
}

BOOST_AUTO_TEST_CASE(test_struct_split_person)
{
    const protoc::uint8_t input[] = { transenc::code_record_begin, transenc::code_string_int8, 0x04, 'K', 'A', 'N', 'T', transenc::code_int16, 0x7F, 0x00, transenc::code_record_end };
    transenc::iarchive in(input, input + sizeof(input));
    split_person value("", 99);
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value.name, "KANT");
    BOOST_REQUIRE_EQUAL(value.age, 127);
}

template <typename Type>
struct type_struct
{
    template<typename T>
    void serialize(T& archive, const unsigned int)
    {
        archive & data;
    }

    Type data;
};

BOOST_AUTO_TEST_CASE(test_struct_vector_empty)
{
    const protoc::uint8_t input[] = { transenc::code_record_begin, transenc::code_array_begin, 0x00, transenc::code_array_end, transenc::code_record_end };
    transenc::iarchive in(input, input + sizeof(input));
    type_struct< std::vector<bool> > value;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value.data.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_struct_vector_one)
{
    const protoc::uint8_t input[] = { transenc::code_record_begin, transenc::code_array_begin, 0x01, transenc::code_true, transenc::code_array_end, transenc::code_record_end };
    transenc::iarchive in(input, input + sizeof(input));
    type_struct< std::vector<bool> > value;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value.data.size(), 1);
    BOOST_REQUIRE_EQUAL(value.data[0], true);
}

BOOST_AUTO_TEST_CASE(test_struct_set_empty)
{
    const protoc::uint8_t input[] = { transenc::code_record_begin, transenc::code_array_begin, transenc::code_null, transenc::code_array_end, transenc::code_record_end };
    transenc::iarchive in(input, input + sizeof(input));
    type_struct< std::set<int> > value;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value.data.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_struct_set_one)
{
    const protoc::uint8_t input[] = { transenc::code_record_begin, transenc::code_array_begin, transenc::code_null, 0x11, transenc::code_array_end, transenc::code_record_end };
    transenc::iarchive in(input, input + sizeof(input));
    type_struct< std::set<int> > value;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value.data.size(), 1);
    BOOST_REQUIRE_EQUAL(value.data.count(0x11), 1);
}

BOOST_AUTO_TEST_CASE(test_struct_map_empty)
{
    const protoc::uint8_t input[] = { transenc::code_record_begin, transenc::code_map_begin, transenc::code_null, transenc::code_map_end, transenc::code_record_end };
    transenc::iarchive in(input, input + sizeof(input));
    type_struct< std::map<std::string, bool> > value;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value.data.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_struct_map_one)
{
    const protoc::uint8_t input[] = { transenc::code_record_begin, transenc::code_map_begin, transenc::code_null, transenc::code_record_begin, transenc::code_string_int8, 0x01, 'A', transenc::code_true, transenc::code_record_end, transenc::code_map_end, transenc::code_record_end };
    transenc::iarchive in(input, input + sizeof(input));
    type_struct< std::map<std::string, bool> > value;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value.data.size(), 1);
    BOOST_REQUIRE_EQUAL(value.data["A"], true);
}

BOOST_AUTO_TEST_SUITE_END()
