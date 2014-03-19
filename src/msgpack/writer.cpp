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

#include <protoc/msgpack/writer.hpp>

#include <cassert>
#include <protoc/exceptions.hpp>

namespace protoc
{
namespace msgpack
{

writer::writer(output_type& out)
    : encoder(out)
{
}

writer::size_type writer::size()
{
    return stack.size();
}

writer::size_type writer::write()
{
    return track(encoder.put());
}

writer::size_type writer::write(bool value)
{
    return track(encoder.put(value));
}

writer::size_type writer::write(int value)
{
    return track(encoder.put(value));
}

writer::size_type writer::write(long long value)
{
    return track(encoder.put(protoc::int64_t(value)));
}

writer::size_type writer::write(float value)
{
    return track(encoder.put(protoc::float32_t(value)));
}

writer::size_type writer::write(double value)
{
    return track(encoder.put(protoc::float64_t(value)));
}

writer::size_type writer::write(const char *value)
{
    return track(encoder.put(value));
}

writer::size_type writer::write(const std::string& value)
{
    return track(encoder.put(value));
}

writer::size_type writer::write(const value_type *data, size_type size)
{
    return track(encoder.put(data, size));
}

writer::size_type writer::record_begin()
{
    return 0;
}

writer::size_type writer::record_end()
{
    return 0;
}

writer::size_type writer::array_begin()
{
    throw invalid_value("Array count must be specified");
}

writer::size_type writer::array_begin(size_type count)
{
    stack.push(frame(protoc::token::token_array_begin, count));
    return encoder.put_array_begin(count);
}

writer::size_type writer::array_end()
{
    assert(!stack.empty());
    if (stack.empty())
        throw invalid_scope("Stack empty");

    frame& top = stack.top();
    if (top.count != 0)
        throw invalid_scope("Writing too few elements");

    stack.pop();

    return 0;
}

writer::size_type writer::map_begin()
{
    throw invalid_value("Map count must be specified");
}

writer::size_type writer::map_begin(size_type count)
{
    stack.push(frame(protoc::token::token_map_begin, 2 * count));
    return encoder.put_map_begin(count);
}

writer::size_type writer::map_end()
{
    assert(!stack.empty());
    if (stack.empty())
        throw invalid_scope("Stack empty");

    frame& top = stack.top();
    if (top.count != 0)
        throw invalid_scope("Writing too few elements");

    stack.pop();

    return 0;
}

writer::size_type writer::track(size_type size)
{
    if (stack.empty())
        return size;

    frame& top = stack.top();
    if (top.count == 0)
        throw invalid_scope("Writing too many elements");
    --top.count;
    return size;
}

writer::frame::frame(protoc::token::value token,
                     size_type count)
    : token(token),
      count(count)
{
}

} // namespace msgpack
} // namespace protoc
