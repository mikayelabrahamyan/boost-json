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

#include <protoc/output_range.hpp>

namespace protoc
{

output_range::output_range()
    : input_range()
{
}

output_range::~output_range()
{
}

output_range::output_range(iterator first, iterator last)
    : input_range(const_cast<const_iterator>(first),
                  const_cast<const_iterator>(last))
{
}

output_range::value_type& output_range::operator * ()
{
    return const_cast<value_type&>(*current);
}

output_range::iterator output_range::begin()
{
    return const_cast<iterator>(current);
}

output_range::iterator output_range::end()
{
    return const_cast<iterator>(last);
}

}
