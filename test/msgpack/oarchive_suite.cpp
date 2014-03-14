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
    BOOST_REQUIRE_EQUAL(result.str().data(), "");
}

BOOST_AUTO_TEST_CASE(test_false)
{
    std::ostringstream result;
    format::stream_oarchive out(result);
    bool value = false;
    out << value;
    char expected[] = { detail::code_false, 0x00 };
    BOOST_REQUIRE_EQUAL(result.str().data(), expected);
}

BOOST_AUTO_TEST_CASE(test_true)
{
    std::ostringstream result;
    format::stream_oarchive ar(result);
    bool value = true;
    ar << value;
    char expected[] = { detail::code_true, 0x00 };
    BOOST_REQUIRE_EQUAL(result.str().data(), expected);
}

BOOST_AUTO_TEST_SUITE_END()
