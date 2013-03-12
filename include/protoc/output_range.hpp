#ifndef PROTOC_OUTPUT_RANGE_HPP
#define PROTOC_OUTPUT_RANGE_HPP

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

class output_range : public input_range
{
public:
    output_range();
    virtual ~output_range();
    output_range(iterator first, iterator last);

    value_type& operator * ();

    iterator begin();
    iterator end();
};

}

#endif /* PROTOC_OUTPUT_RANGE_HPP */
