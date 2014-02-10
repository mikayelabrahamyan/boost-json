///////////////////////////////////////////////////////////////////////////////
//
// http://protoc.sourceforge.net/
//
// Copyright (C) 2013 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <boost/test/unit_test.hpp>

#include <sstream>
#include <limits>
#include <protoc/exceptions.hpp>
#include <protoc/json/stream_oarchive.hpp>
#include <protoc/json/pair.hpp>
#include <protoc/json/string.hpp>
#include <protoc/json/vector.hpp>
#include <protoc/json/set.hpp>
#include <protoc/json/map.hpp>
#include <protoc/json/optional.hpp>
#include <protoc/serialization/nvp.hpp>

using namespace protoc;

BOOST_AUTO_TEST_SUITE(json_oarchive_suite)

//-----------------------------------------------------------------------------
// Basic types
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_empty)
{
    std::ostringstream result;
    json::stream_oarchive ar(result);
    BOOST_REQUIRE_EQUAL(result.str().data(), "");
}

BOOST_AUTO_TEST_CASE(test_false)
{
    std::ostringstream result;
    json::stream_oarchive out(result);
    bool value = false;
    out << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "false");
}

BOOST_AUTO_TEST_CASE(test_const_false)
{
    std::ostringstream result;
    json::stream_oarchive out(result);
    const bool value = false;
    out << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "false");
}

BOOST_AUTO_TEST_CASE(test_true)
{
    std::ostringstream result;
    json::stream_oarchive out(result);
    bool value = true;
    out << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "true");
}

BOOST_AUTO_TEST_CASE(test_const_true)
{
    std::ostringstream result;
    json::stream_oarchive out(result);
    const bool value = true;
    out << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "true");
}

//-----------------------------------------------------------------------------
// Integers
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_int_zero)
{
    std::ostringstream result;
    json::stream_oarchive ar(result);
    int value = 0;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "0");
}

BOOST_AUTO_TEST_CASE(test_const_int_zero)
{
    std::ostringstream result;
    json::stream_oarchive ar(result);
    const int value = 0;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "0");
}

BOOST_AUTO_TEST_CASE(test_int_one)
{
    std::ostringstream result;
    json::stream_oarchive ar(result);
    int value = 1;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "1");
}

BOOST_AUTO_TEST_CASE(test_int_minus_one)
{
    std::ostringstream result;
    json::stream_oarchive ar(result);
    int value = -1;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "-1");
}

//-----------------------------------------------------------------------------
// Float
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_double_one)
{
    std::ostringstream result;
    json::stream_oarchive ar(result);
    double value = 1.0;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "1");
}

BOOST_AUTO_TEST_CASE(test_const_double_one)
{
    std::ostringstream result;
    json::stream_oarchive ar(result);
    const double value = 1.0;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "1");
}

BOOST_AUTO_TEST_CASE(test_double_half)
{
    std::ostringstream result;
    json::stream_oarchive ar(result);
    double value = 0.5;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "0.5");
}

BOOST_AUTO_TEST_CASE(test_double_max)
{
    std::ostringstream result;
    json::stream_oarchive ar(result);
    double value = std::numeric_limits<double>::max();
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "1.7976931348623157e+308");
}

BOOST_AUTO_TEST_CASE(test_double_min)
{
    std::ostringstream result;
    json::stream_oarchive ar(result);
    double value = std::numeric_limits<double>::min();
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "2.2250738585072014e-308");
}

BOOST_AUTO_TEST_CASE(test_double_infinity)
{
    std::ostringstream result;
    json::stream_oarchive ar(result);
    double value = std::numeric_limits<double>::infinity();
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "null");
}

BOOST_AUTO_TEST_CASE(test_double_minus_infinity)
{
    std::ostringstream result;
    json::stream_oarchive ar(result);
    double value = -std::numeric_limits<double>::infinity();
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "null");
}

BOOST_AUTO_TEST_CASE(test_double_nan)
{
    std::ostringstream result;
    json::stream_oarchive ar(result);
    double value = std::numeric_limits<double>::quiet_NaN();
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "null");
}

//-----------------------------------------------------------------------------
// String
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_string_empty)
{
    std::ostringstream result;
    json::stream_oarchive ar(result);
    std::string value("");
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\"\"");
}

BOOST_AUTO_TEST_CASE(test_const_string_empty)
{
    std::ostringstream result;
    json::stream_oarchive ar(result);
    const std::string value("");
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\"\"");
}

BOOST_AUTO_TEST_CASE(test_string_alpha)
{
    std::ostringstream result;
    json::stream_oarchive ar(result);
    std::string value("alpha");
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\"alpha\"");
}

//-----------------------------------------------------------------------------
// Pair
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_pair)
{
    std::ostringstream result;
    json::stream_oarchive ar(result);
    std::pair<std::string, bool> value("alpha", true);
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "[\"alpha\",true]");
}

BOOST_AUTO_TEST_CASE(test_const_pair)
{
    std::ostringstream result;
    json::stream_oarchive ar(result);
    const std::pair<std::string, bool> value("alpha", true);
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "[\"alpha\",true]");
}

//-----------------------------------------------------------------------------
// Optional
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_optional)
{
    std::ostringstream result;
    json::stream_oarchive ar(result);
    boost::optional<std::string> value("alpha");
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\"alpha\"");
}

BOOST_AUTO_TEST_CASE(test_optional_null)
{
    std::ostringstream result;
    json::stream_oarchive ar(result);
    boost::optional<std::string> value;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "null");
}

BOOST_AUTO_TEST_CASE(test_const_optional)
{
    std::ostringstream result;
    json::stream_oarchive ar(result);
    const boost::optional<std::string> value("alpha");
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "\"alpha\"");
}

BOOST_AUTO_TEST_CASE(test_const_optional_null)
{
    std::ostringstream result;
    json::stream_oarchive ar(result);
    const boost::optional<std::string> value;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "null");
}

//-----------------------------------------------------------------------------
// Named value pair
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_nvp)
{
    std::ostringstream result;
    json::stream_oarchive out(result);
    bool value = false;
    out << boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(result.str().data(), "false");
}
//-----------------------------------------------------------------------------
// Container
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_array_bool_empty)
{
    std::ostringstream result;
    json::stream_oarchive ar(result);
    std::vector<bool> value;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "[]");
}

BOOST_AUTO_TEST_CASE(test_array_bool_one)
{
    std::ostringstream result;
    json::stream_oarchive ar(result);
    std::vector<bool> value;
    value.push_back(true);
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "[true]");
}

BOOST_AUTO_TEST_CASE(test_array_bool_two)
{
    std::ostringstream result;
    json::stream_oarchive ar(result);
    std::vector<bool> value;
    value.push_back(true);
    value.push_back(false);
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "[true,false]");
}

BOOST_AUTO_TEST_CASE(test_object_bool_empty)
{
    std::ostringstream result;
    json::stream_oarchive ar(result);
    std::map<std::string, bool> value;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "{}");
}

BOOST_AUTO_TEST_CASE(test_object_bool_one)
{
    std::ostringstream result;
    json::stream_oarchive ar(result);
    std::map<std::string, bool> value;
    value["A"] = true;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "{\"A\":true}");
}

BOOST_AUTO_TEST_CASE(test_object_bool_two)
{
    std::ostringstream result;
    json::stream_oarchive ar(result);
    std::map<std::string, bool> value;
    value["A"] = true;
    value["B"] = false;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "{\"A\":true,\"B\":false}");
}

BOOST_AUTO_TEST_CASE(test_nonobject_bool_empty)
{
    std::ostringstream result;
    json::stream_oarchive ar(result);
    std::map<int, bool> value;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "[]");
}

BOOST_AUTO_TEST_CASE(test_nonobject_bool_one)
{
    std::ostringstream result;
    json::stream_oarchive ar(result);
    std::map<int, bool> value;
    value[2] = true;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "[[2,true]]");
}

BOOST_AUTO_TEST_CASE(test_nonobject_bool_two)
{
    std::ostringstream result;
    json::stream_oarchive ar(result);
    std::map<int, bool> value;
    value[2] = true;
    value[4] = false;
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "[[2,true],[4,false]]");
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

BOOST_AUTO_TEST_CASE(test_array_struct)
{
    std::ostringstream result;
    json::stream_oarchive ar(result);
    person value("Kant", 127);
    ar << value;
    BOOST_REQUIRE_EQUAL(result.str().data(), "[\"Kant\",127]");
}

BOOST_AUTO_TEST_SUITE_END()
