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

#include <protoc/output_vector.hpp>
#include <protoc/output_array.hpp>
#include <protoc/msgpack/writer.hpp>
#include <protoc/msgpack/detail/codes.hpp>

namespace format = protoc::msgpack;
namespace detail = format::detail;

template<std::size_t N>
struct test_array : public protoc::output_array<unsigned char, N>
{
};

struct test_vector : public protoc::output_vector<unsigned char>
{
};

BOOST_AUTO_TEST_SUITE(msgpack_writer_suite)

//-----------------------------------------------------------------------------
// Basic types
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_empty)
{
    test_vector buffer;
    format::writer writer(buffer);
    BOOST_REQUIRE_EQUAL(writer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_null)
{
    test_vector buffer;
    format::writer writer(buffer);
    BOOST_REQUIRE_EQUAL(writer.write(), 1);

    format::writer::value_type expected[] = { detail::code_null };
    BOOST_REQUIRE_EQUAL_COLLECTIONS(buffer.begin(), buffer.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_true)
{
    test_vector buffer;
    format::writer writer(buffer);
    BOOST_REQUIRE_EQUAL(writer.write(true), 1);

    format::writer::value_type expected[] = { detail::code_true };
    BOOST_REQUIRE_EQUAL_COLLECTIONS(buffer.begin(), buffer.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_false)
{
    test_vector buffer;
    format::writer writer(buffer);
    BOOST_REQUIRE_EQUAL(writer.write(false), 1);

    format::writer::value_type expected[] = { detail::code_false };
    BOOST_REQUIRE_EQUAL_COLLECTIONS(buffer.begin(), buffer.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_integer_zero)
{
    test_vector buffer;
    format::writer writer(buffer);
    BOOST_REQUIRE_EQUAL(writer.write(0), 1);

    format::writer::value_type expected[] = { 0x00 };
    BOOST_REQUIRE_EQUAL_COLLECTIONS(buffer.begin(), buffer.end(),
                                    expected, expected + sizeof(expected));
}

//-----------------------------------------------------------------------------
// Container
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_array_empty)
{
    test_vector buffer;
    format::writer writer(buffer);
    BOOST_REQUIRE_EQUAL(writer.array_begin(0), 1);
    BOOST_REQUIRE_EQUAL(writer.array_end(), 0);

    format::writer::value_type expected[] = { detail::code_fixarray_0 };
    BOOST_REQUIRE_EQUAL_COLLECTIONS(buffer.begin(), buffer.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(test_array_one)
{
    test_vector buffer;
    format::writer writer(buffer);
    BOOST_REQUIRE_EQUAL(writer.array_begin(1), 1);
    BOOST_REQUIRE_EQUAL(writer.write(), 1);
    BOOST_REQUIRE_EQUAL(writer.array_end(), 0);

    format::writer::value_type expected[] = { detail::code_fixarray_1, detail::code_null };
    BOOST_REQUIRE_EQUAL_COLLECTIONS(buffer.begin(), buffer.end(),
                                    expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(fail_array_count_too_small)
{
    test_vector buffer;
    format::writer writer(buffer);
    BOOST_REQUIRE_EQUAL(writer.array_begin(0), 1);
    BOOST_REQUIRE_THROW(writer.write(), protoc::invalid_scope);
}

BOOST_AUTO_TEST_CASE(fail_array_count_too_big)
{
    test_vector buffer;
    format::writer writer(buffer);
    BOOST_REQUIRE_EQUAL(writer.array_begin(2), 1);
    BOOST_REQUIRE_EQUAL(writer.write(), 1);
    BOOST_REQUIRE_THROW(writer.array_end(), protoc::invalid_scope);
}

BOOST_AUTO_TEST_SUITE_END()
