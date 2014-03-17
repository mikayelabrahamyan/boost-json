#ifndef PROTOC_MSGPACK_WRITER_HPP
#define PROTOC_MSGPACK_WRITER_HPP

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

#include <stack>
#include <protoc/writer.hpp>
#include <protoc/token.hpp>
#include <protoc/msgpack/detail/encoder.hpp>

namespace protoc
{
namespace msgpack
{

class writer : public protoc::writer
{
    typedef msgpack::detail::encoder encoder_type;

public:
    typedef encoder_type::output_type output_type;

    writer(output_type&);

    virtual size_type size();

    virtual size_type write(); // Null
    virtual size_type write(bool);
    virtual size_type write(int);
    virtual size_type write(long long);
    virtual size_type write(double);
    virtual size_type write(const char *);
    virtual size_type write(const std::string&);
    virtual size_type write(const_pointer, size_type);

    virtual size_type array_begin();
    virtual size_type array_begin(size_type count);
    virtual size_type array_end();

    virtual size_type map_begin();
    virtual size_type map_begin(size_type count);
    virtual size_type map_end();

private:
    size_type track(size_type);

private:
    encoder_type encoder;
    struct element
    {
        element(protoc::token::value token, size_type count)
            : token(token),
              count(count)
        {}

        protoc::token::value token;
        size_type count;
    };
    typedef std::stack<element> stack_type;
    stack_type stack;
};

} // namespace msgpack
} // namespace protoc

#include <cassert>
#include <protoc/exceptions.hpp>

namespace protoc
{
namespace msgpack
{

inline writer::writer(output_type& out)
    : encoder(out)
{
}

inline writer::size_type writer::size()
{
    return stack.size();
}

inline writer::size_type writer::write()
{
    return track(encoder.put());
}

inline writer::size_type writer::write(bool value)
{
    return track(encoder.put(value));
}

inline writer::size_type writer::write(int value)
{
    return track(encoder.put(value));
}

inline writer::size_type writer::write(long long value)
{
    return track(encoder.put(protoc::int64_t(value)));
}

inline writer::size_type writer::write(double value)
{
    return track(encoder.put(value));
}

inline writer::size_type writer::write(const char *value)
{
    return track(encoder.put(value));
}

inline writer::size_type writer::write(const std::string& value)
{
    return track(encoder.put(value));
}

inline writer::size_type writer::write(const_pointer data, size_type size)
{
    return track(encoder.put(data, size));
}

inline writer::size_type writer::array_begin()
{
    throw invalid_value("Array count must be specified");
}

inline writer::size_type writer::array_begin(size_type count)
{
    stack.push(element(protoc::token::token_array_begin, count));
    return encoder.put_array_begin(count);
}

inline writer::size_type writer::array_end()
{
    assert(!stack.empty());
    if (stack.empty())
        throw invalid_scope("Stack empty");

    element& top = stack.top();
    if (top.count != 0)
        throw invalid_scope("Writing too few elements");

    stack.pop();

    return 0;
}

inline writer::size_type writer::map_begin()
{
    throw invalid_value("Map count must be specified");
}

inline writer::size_type writer::map_begin(size_type count)
{
    stack.push(element(protoc::token::token_map_begin, 2 * count));
    return encoder.put_map_begin(count);
}

inline writer::size_type writer::map_end()
{
    assert(!stack.empty());
    if (stack.empty())
        throw invalid_scope("Stack empty");

    element& top = stack.top();
    if (top.count != 0)
        throw invalid_scope("Writing too few elements");

    stack.pop();

    return 0;
}

inline writer::size_type writer::track(size_type size)
{
    if (stack.empty())
        return size;

    element& top = stack.top();
    if (top.count == 0)
        throw invalid_scope("Writing too many elements");
    --top.count;
    return size;
}

} // namespace msgpack
} // namespace protoc

#endif // PROTOC_MSGPACK_WRITER_HPP
