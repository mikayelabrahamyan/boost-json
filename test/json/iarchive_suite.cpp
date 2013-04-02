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
    BOOST_REQUIRE_NO_THROW(in >> boost::serialization::make_nvp("value", value));
    BOOST_REQUIRE_EQUAL(value, false);
}

BOOST_AUTO_TEST_CASE(test_fail_false)
{
    const char input[] = "fals";
    json::iarchive in(input, input + sizeof(input) - 1);
    bool value = true;
    BOOST_REQUIRE_THROW(in >> boost::serialization::make_nvp("value", value),
                        unexpected_token);
}

BOOST_AUTO_TEST_CASE(test_true)
{
    const char input[] = "true";
    json::iarchive in(input, input + sizeof(input) - 1);
    bool value = false;
    BOOST_REQUIRE_NO_THROW(in >> boost::serialization::make_nvp("value", value));
    BOOST_REQUIRE_EQUAL(value, true);
}

BOOST_AUTO_TEST_CASE(test_fail_true)
{
    const char input[] = "tru";
    json::iarchive in(input, input + sizeof(input) - 1);
    bool value = false;
    BOOST_REQUIRE_THROW(in >> boost::serialization::make_nvp("value", value),
                        unexpected_token);
}

BOOST_AUTO_TEST_SUITE_END()
