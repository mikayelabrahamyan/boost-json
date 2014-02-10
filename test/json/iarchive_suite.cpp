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
#include <protoc/exceptions.hpp>
#include <protoc/json/iarchive.hpp>
#include <protoc/json/pair.hpp>
#include <protoc/json/string.hpp>
#include <protoc/json/vector.hpp>
#include <protoc/json/set.hpp>
#include <protoc/json/map.hpp>
#include <protoc/json/optional.hpp>
#include <protoc/serialization/nvp.hpp>

using namespace protoc;

BOOST_AUTO_TEST_SUITE(json_iarchive_suite)

//-----------------------------------------------------------------------------
// Basic types
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_false)
{
    const char input[] = "false";
    json::iarchive in(input, input + sizeof(input) - 1);
    bool value = true;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value, false);
}

BOOST_AUTO_TEST_CASE(test_fail_false)
{
    const char input[] = "fals";
    json::iarchive in(input, input + sizeof(input) - 1);
    bool value = true;
    BOOST_REQUIRE_THROW(in >> value,
                        invalid_value);
}

BOOST_AUTO_TEST_CASE(test_true)
{
    const char input[] = "true";
    json::iarchive in(input, input + sizeof(input) - 1);
    bool value = false;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value, true);
}

BOOST_AUTO_TEST_CASE(test_fail_true)
{
    const char input[] = "tru";
    json::iarchive in(input, input + sizeof(input) - 1);
    bool value = false;
    BOOST_REQUIRE_THROW(in >> value,
                        invalid_value);
}

//-----------------------------------------------------------------------------
// Integer
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_integer_zero)
{
    const char input[] = "0";
    json::iarchive in(input, input + sizeof(input) - 1);
    protoc::int64_t value = 99;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value, 0);
}

BOOST_AUTO_TEST_CASE(test_integer_hundred)
{
    const char input[] = "100";
    json::iarchive in(input, input + sizeof(input) - 1);
    protoc::int64_t value = 99;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value, 100);
}

BOOST_AUTO_TEST_CASE(test_integer_minus_hundred)
{
    const char input[] = "-100";
    json::iarchive in(input, input + sizeof(input) - 1);
    protoc::int64_t value = 99;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value, -100);
}

//-----------------------------------------------------------------------------
// Float
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_float_half)
{
    const char input[] = "0.5";
    json::iarchive in(input, input + sizeof(input) - 1);
    protoc::float64_t value = 99;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value, 0.5);
}

//-----------------------------------------------------------------------------
// String
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_string_empty)
{
    const char input[] = "\"\"";
    json::iarchive in(input, input + sizeof(input) - 1);
    std::string value("EMPTY");
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value, "");
}

BOOST_AUTO_TEST_CASE(test_string_alpha)
{
    const char input[] = "\"alpha\"";
    json::iarchive in(input, input + sizeof(input) - 1);
    std::string value("EMPTY");
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value, "alpha");
}

BOOST_AUTO_TEST_CASE(test_string_escape_quote)
{
    const char input[] = "\"\\\"\"";
    json::iarchive in(input, input + sizeof(input) - 1);
    std::string value("EMPTY");
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value, "\"");
}

BOOST_AUTO_TEST_CASE(test_string_escape_reverse_solidus)
{
    const char input[] = "\"\\\\\"";
    json::iarchive in(input, input + sizeof(input) - 1);
    std::string value("EMPTY");
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value, "\\");
}

BOOST_AUTO_TEST_CASE(test_string_escape_solidus)
{
    const char input[] = "\"\\/\"";
    json::iarchive in(input, input + sizeof(input) - 1);
    std::string value("EMPTY");
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value, "/");
}

//-----------------------------------------------------------------------------
// Pair
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_pair)
{
    const char input[] = "[42,true]";
    json::iarchive in(input, input + sizeof(input) - 1);
    std::pair<protoc::int64_t, bool> value(42, true);
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value.first, 42);
    BOOST_REQUIRE_EQUAL(value.second, true);
}

BOOST_AUTO_TEST_CASE(test_pair_object)
{
    const char input[] = "[\"alpha\",true]";
    json::iarchive in(input, input + sizeof(input) - 1);
    std::pair<std::string, bool> value("alpha", true);
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value.first, "alpha");
    BOOST_REQUIRE_EQUAL(value.second, true);
}

//-----------------------------------------------------------------------------
// Optional
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_optional)
{
    const char input[] = "\"alpha\"";
    json::iarchive in(input, input + sizeof(input) - 1);
    boost::optional<std::string> value;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE(value);
    BOOST_REQUIRE_EQUAL(*value, "alpha");
}

BOOST_AUTO_TEST_CASE(test_optional_null)
{
    const char input[] = "null";
    json::iarchive in(input, input + sizeof(input) - 1);
    boost::optional<std::string> value;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE(!value);
}

BOOST_AUTO_TEST_CASE(test_optional_wrong_type)
{
    const char input[] = "true";
    json::iarchive in(input, input + sizeof(input) - 1);
    boost::optional<std::string> value;
    BOOST_REQUIRE_THROW(in >> value,
                        invalid_value);
}

//-----------------------------------------------------------------------------
// Named value pair
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_nvp)
{
    const char input[] = "false";
    json::iarchive in(input, input + sizeof(input) - 1);
    bool value = true;
    BOOST_REQUIRE_NO_THROW(in >> boost::serialization::make_nvp("value", value));
    BOOST_REQUIRE_EQUAL(value, false);
}

//-----------------------------------------------------------------------------
// Container
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_vector_bool_empty)
{
    const char input[] = "[]";
    json::iarchive in(input, input + sizeof(input) - 1);
    std::vector<bool> value;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_vector_bool_one)
{
    const char input[] = "[true]";
    json::iarchive in(input, input + sizeof(input) - 1);
    std::vector<bool> value;
    BOOST_REQUIRE_NO_THROW(in >> boost::serialization::make_nvp("value", value));
    BOOST_REQUIRE_EQUAL(value.size(), 1);
    BOOST_REQUIRE_EQUAL(value[0], true);
}

BOOST_AUTO_TEST_CASE(test_vector_bool_two)
{
    const char input[] = "[true,false]";
    json::iarchive in(input, input + sizeof(input) - 1);
    std::vector<bool> value;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value.size(), 2);
    BOOST_REQUIRE_EQUAL(value[0], true);
    BOOST_REQUIRE_EQUAL(value[1], false);
}

BOOST_AUTO_TEST_CASE(test_vector_mixed)
{
    const char input[] = "[true,0]"; // Although this is legal JSON, we cannot deserialize it into a vector<bool>
    json::iarchive in(input, input + sizeof(input) - 1);
    std::vector<bool> value;
    BOOST_REQUIRE_THROW(in >> value,
                        invalid_value);
}

BOOST_AUTO_TEST_CASE(test_vector_missing_end)
{
    const char input[] = "[true";
    json::iarchive in(input, input + sizeof(input) - 1);
    std::vector<bool> value;
    BOOST_REQUIRE_THROW(in >> value,
                        unexpected_token);
}

BOOST_AUTO_TEST_CASE(test_vector_missing_begin)
{
    const char input[] = "true]";
    json::iarchive in(input, input + sizeof(input) - 1);
    std::vector<bool> value;
    BOOST_REQUIRE_THROW(in >> value,
                        unexpected_token);
}

BOOST_AUTO_TEST_CASE(test_intmap_bool_empty)
{
    const char input[] = "[]";
    json::iarchive in(input, input + sizeof(input) - 1);
    std::map<protoc::int64_t, bool> value;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_intmap_bool_one)
{
    const char input[] = "[[2,true]]";
    json::iarchive in(input, input + sizeof(input) - 1);
    std::map<protoc::int64_t, bool> value;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value.size(), 1);
    BOOST_REQUIRE_EQUAL(value[2], true);
}

BOOST_AUTO_TEST_CASE(test_intmap_bool_two)
{
    const char input[] = "[[2,true],[4,false]]";
    json::iarchive in(input, input + sizeof(input) - 1);
    std::map<protoc::int64_t, bool> value;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value.size(), 2);
    BOOST_REQUIRE_EQUAL(value[2], true);
    BOOST_REQUIRE_EQUAL(value[4], false);
}

BOOST_AUTO_TEST_CASE(test_map_bool_empty)
{
    const char input[] = "{}";
    json::iarchive in(input, input + sizeof(input) - 1);
    std::map<std::string, bool> value;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_map_bool_one)
{
    const char input[] = "{\"alpha\":true}";
    json::iarchive in(input, input + sizeof(input) - 1);
    std::map<std::string, bool> value;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value.size(), 1);
    BOOST_REQUIRE_EQUAL(value["alpha"], true);
}

BOOST_AUTO_TEST_CASE(test_map_bool_two)
{
    const char input[] = "{\"alpha\":true,\"bravo\":false}";
    json::iarchive in(input, input + sizeof(input) - 1);
    std::map<std::string, bool> value;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value.size(), 2);
    BOOST_REQUIRE_EQUAL(value["alpha"], true);
    BOOST_REQUIRE_EQUAL(value["bravo"], false);
}

BOOST_AUTO_TEST_SUITE_END()
