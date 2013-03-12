///////////////////////////////////////////////////////////////////////////////
//
// http://protoc.sourceforge.net/
//
// Copyright (C) 2013 Bjorn Reese <breese@users.sourceforge.net>
//
// Permission to use, copy, modify, and distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
// MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE AUTHORS AND
// CONTRIBUTORS ACCEPT NO RESPONSIBILITY IN ANY CONCEIVABLE MANNER.
//
///////////////////////////////////////////////////////////////////////////////

#include <protoc/input_range.hpp>

namespace protoc
{

input_range::input_range()
    : current(0),
      last(0)
{
}

input_range::~input_range()
{
}

input_range::input_range(const input_range& other)
    : current(other.current),
      last(other.last)
{
}

input_range::input_range(const_iterator first, const_iterator last)
    : current(const_cast<iterator>(first)),
      last(const_cast<iterator>(last))
{
}

input_range& input_range::operator = (const input_range& other)
{
    current = other.current;
    last = other.last;
    return *this;
}

input_range& input_range::operator += (size_type delta)
{
    if (delta < size())
    {
        current += delta;
    }
    else
    {
        current = last;
    }
    return *this;
}

bool input_range::empty() const
{
    return (current == last);
}

const input_range::value_type& input_range::operator * () const
{
    return *current;
}

input_range input_range::operator ++ ()
{
    ++current;
    return *this;
}

input_range input_range::operator ++ (int)
{
    input_range result(*this);
    ++current;
    return result;
}

input_range::const_iterator input_range::begin() const
{
    return current;
}

input_range::const_iterator input_range::end() const
{
    return last;
}

input_range::size_type input_range::size() const
{
    return (last - current);
}

}
