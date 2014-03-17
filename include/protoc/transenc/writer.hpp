#ifndef PROTOC_TRANSENC_WRITER_HPP
#define PROTOC_TRANSENC_WRITER_HPP

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
#include <boost/optional.hpp>
#include <protoc/writer.hpp>
#include <protoc/token.hpp>
#include <protoc/transenc/encoder.hpp>

namespace protoc
{
namespace transenc
{

class writer : public protoc::writer
{
    typedef transenc::encoder encoder_type;

public:
    typedef encoder_type::output_type output_type;

    writer(output_type&);

    virtual size_type size();

    virtual size_type write(); // Null
    virtual size_type write(bool);
    virtual size_type write(int);
    virtual size_type write(long long);
    virtual size_type write(float);
    virtual size_type write(double);
    virtual size_type write(const char *);
    virtual size_type write(const std::string&);
    virtual size_type write(const value_type *, size_type);

    virtual size_type record_begin();
    virtual size_type record_end();

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
        element(protoc::token::value token);
        element(protoc::token::value token, size_type counter);

        void decrease();
        void verify_end(protoc::token::value);

        protoc::token::value token;
        boost::optional<size_type> count;
    };
    typedef std::stack<element> stack_type;
    stack_type stack;
};

} // namespace transenc
} // namespace protoc

#include <cassert>
#include <protoc/exceptions.hpp>

namespace protoc
{
namespace transenc
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

inline writer::size_type writer::write(float value)
{
    return track(encoder.put(protoc::float32_t(value)));
}

inline writer::size_type writer::write(double value)
{
    return track(encoder.put(protoc::float64_t(value)));
}

inline writer::size_type writer::write(const char *value)
{
    return track(encoder.put(value));
}

inline writer::size_type writer::write(const std::string& value)
{
    return track(encoder.put(value));
}

inline writer::size_type writer::write(const value_type *data, size_type size)
{
    return track(encoder.put(data, size));
}

inline writer::size_type writer::record_begin()
{
    return encoder.put_record_begin();
}

inline writer::size_type writer::record_end()
{
    return encoder.put_record_end();
}

inline writer::size_type writer::array_begin()
{
    stack.push(element(protoc::token::token_array_begin));
    return encoder.put_array_begin();
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
    top.verify_end(protoc::token::token_array_begin);

    stack.pop();

    return encoder.put_array_end();
}

inline writer::size_type writer::map_begin()
{
    stack.push(element(protoc::token::token_map_begin));
    return encoder.put_map_begin();
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
    top.verify_end(protoc::token::token_map_begin);

    stack.pop();

    return encoder.put_map_end();
}

inline writer::size_type writer::track(size_type size)
{
    if (stack.empty())
        return size;

    element& top = stack.top();
    top.decrease();
    return size;
}

inline writer::element::element(protoc::token::value token)
    : token(token)
{
}

inline writer::element::element(protoc::token::value token,
                                size_type counter)
    : token(token),
      count(counter)
{
}

inline void writer::element::decrease()
{
    if (!count)
        return;

    if (*count == 0)
        throw invalid_scope("Writing too many elements");
    --(*count);
}

inline void writer::element::verify_end(protoc::token::value expected_token)
{
    if (expected_token != token)
        throw invalid_scope("Unexpected scope token");

    if (!count)
        return;

    size_type s = *count;
    if (s != 0)
        throw invalid_scope("Writing too few elements");
}

} // namespace transenc
} // namespace protoc


#endif // PROTOC_TRANSENC_WRITER_HPP
