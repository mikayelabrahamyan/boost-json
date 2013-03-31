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
#include <protoc/json/oarchive.hpp>

using namespace protoc;

BOOST_AUTO_TEST_SUITE(json_oarchive_suite)

//-----------------------------------------------------------------------------
// Basic types
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_empty)
{
    std::ostringstream result;
    json::oarchive ar(result);
    BOOST_REQUIRE_EQUAL(result.str().data(), "");
}

BOOST_AUTO_TEST_CASE(test_false)
{
    std::ostringstream result;
    json::oarchive out(result);
    bool value = false;
    out << boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(result.str().data(), "false");
}

BOOST_AUTO_TEST_CASE(test_const_false)
{
    std::ostringstream result;
    json::oarchive out(result);
    const bool value = false;
    out << boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(result.str().data(), "false");
}

BOOST_AUTO_TEST_CASE(test_true)
{
    std::ostringstream result;
    json::oarchive out(result);
    bool value = true;
    out << boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(result.str().data(), "true");
}

BOOST_AUTO_TEST_CASE(test_const_true)
{
    std::ostringstream result;
    json::oarchive out(result);
    const bool value = true;
    out << boost::serialization::make_nvp("value", value);
    BOOST_REQUIRE_EQUAL(result.str().data(), "true");
}

BOOST_AUTO_TEST_SUITE_END()
