///////////////////////////////////////////////////////////////////////////////
//
// http://protoc.sourceforge.net/
//
// Copyright (C) 2014 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <boost/test/unit_test.hpp>

#include <protoc/msgpack/detail/codes.hpp>
#include <protoc/msgpack/stream_oarchive.hpp>
#include <protoc/msgpack/string.hpp>
#include <protoc/msgpack/vector.hpp>
#include <protoc/msgpack/map.hpp>

namespace format = protoc::msgpack;
namespace detail = protoc::msgpack::detail;

BOOST_AUTO_TEST_SUITE(msgpack_oarchive_suite)

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
    format::stream_oarchive out(result);
    bool value = false;
    out << value;
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

//-----------------------------------------------------------------------------
// Floating-point
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_float32_one)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    protoc::float32_t value = 1.0f;
    ar << value;
    char expected[] = { detail::code_float32, 0x3F, 0x80, 0x00, 0x00 };
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
    char expected[] = { detail::code_float32, 0x3F, 0x80, 0x00, 0x00 };
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
    char expected[] = { detail::code_float64, 0x3F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
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
    char expected[] = { detail::code_float64, 0x3F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
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
    char expected[] = { detail::code_bin8, 0x00 };
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
    char expected[] = { detail::code_bin8, 0x01, 0xFF };
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

    char expected[] = { detail::code_fixarray_0 };
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

    char expected[] = { detail::code_fixarray_1, detail::code_true };
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

    char expected[] = { detail::code_fixmap_0 };
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

    char expected[] = { detail::code_fixmap_1, detail::code_fixstr_1, 0x41, detail::code_true };
    std::string got = result.str();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(got.begin(), got.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_SUITE_END()
