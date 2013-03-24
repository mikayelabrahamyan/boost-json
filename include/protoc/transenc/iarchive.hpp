#ifndef PROTOC_TRANSENC_IARCHIVE_HPP
#define PROTOC_TRANSENC_IARCHIVE_HPP

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

#include <string>
#include <vector>
#include <map>
#include <stack>
#include <sstream>
#include <boost/optional.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/archive/detail/common_iarchive.hpp>
#include <boost/archive/detail/register_archive.hpp>
#include <protoc/types.hpp>
#include <protoc/exceptions.hpp>
#include <protoc/transenc/decoder.hpp>

namespace protoc
{
namespace transenc
{

class iarchive : public boost::archive::detail::common_iarchive<iarchive>
{
    friend class boost::archive::load_access;

    struct scope
    {
        scope(token group) : group(group) {}

        token group;
    };

public:
    iarchive(const char *begin, const char *end);
    ~iarchive();

    void load_override(boost::serialization::nvp<bool>, int);
    void load_override(boost::serialization::nvp<protoc::int8_t>, int);
    void load_override(boost::serialization::nvp<protoc::int16_t>, int);
    void load_override(boost::serialization::nvp<protoc::int32_t>, int);
    void load_override(boost::serialization::nvp<protoc::int64_t>, int);
    void load_override(boost::serialization::nvp<protoc::float32_t>, int);
    void load_override(boost::serialization::nvp<protoc::float64_t>, int);
    void load_override(boost::serialization::nvp<std::string>, int);

    // std::pair
    template<typename first_type, typename second_type>
    void load_override(const boost::serialization::nvp< std::pair<first_type, second_type> >& data, int)
    {
        token type = input.type();
        if (type == token_tuple_begin)
        {
            input.next();
            *this >> boost::serialization::make_nvp("first", data.value().first);
            *this >> boost::serialization::make_nvp("second", data.value().second);
            type = input.type();
            if (type != token_tuple_end)
            {
                goto error;
            }
        }
        else
        {
        error:
            std::ostringstream error;
            error << type;
            throw unexpected_token(error.str());
        }
    }

    // boost::optional

    template<typename value_type>
    void load_override(const boost::serialization::nvp< boost::optional<value_type> >& data, int)
    {
        const token type = input.type();
        if (type == token_null)
        {
            data.value() = boost::optional<value_type>();
        }
        else
        {
            value_type item;
            *this >> boost::serialization::make_nvp(data.name(), item);
            data.value() = boost::optional<value_type>(item);
        }
    }

    // std::vector
    template<typename value_type, typename allocator_type>
    void load_override(const boost::serialization::nvp< std::vector<value_type, allocator_type> > data, int)
    {
        token type = input.type();
        if (type == token_array_begin)
        {
            scope_stack.push(scope(type)); // FIXME: Not popped in error situations
            input.next();
            while (true)
            {
                type = input.type();
                if (type == token_array_end)
                {
                    if (scope_stack.top().group == token_array_begin)
                    {
                        scope_stack.pop();
                    }
                    else
                    {
                        goto error;
                    }
                    break;
                }
                else if ((type == token_eof) || (type == token_error))
                {
                    goto error;
                }
                else
                {
                    value_type item;
                    *this >> boost::serialization::make_nvp(data.name(), item);
                    data.value().push_back(item);
                }
            }
        }
        else
        {
        error:
            std::ostringstream error;
            error << type;
            throw unexpected_token(error.str());
        }
    }

    // std::map
    template<typename key_type, typename mapped_type, typename key_compare, typename allocator_type>
    void load_override(const boost::serialization::nvp< std::map<key_type, mapped_type, key_compare, allocator_type> > data, int)
    {
        token type = input.type();
        if (type == token_array_begin)
        {
            scope_stack.push(scope(type));
            input.next();
            while (true)
            {
                assert(!scope_stack.empty());

                type = input.type();
                if (type == token_array_end)
                {
                    if (scope_stack.top().group == token_array_begin)
                    {
                        scope_stack.pop();
                    }
                    else
                    {
                        goto error;
                    }
                    break;
                }
                else if ((type == token_eof) || (type == token_error))
                {
                    goto error;
                }
                else
                {
                    std::pair<key_type, mapped_type> value;
                    *this >> boost::serialization::make_nvp(data.name(), value);
                    data.value().insert(value);
                    if (input.type() != token_tuple_end)
                    {
                        goto error;
                    }
                }
                input.next();
            }
        }
        else
        {
        error:
            std::ostringstream error;
            error << type;
            throw unexpected_token(error.str());
        }
    }

    // Ignore these
    void load_override(boost::archive::version_type, int) {}
    void load_override(boost::archive::object_id_type, int) {}
    void load_override(boost::archive::object_reference_type, int) {}
    void load_override(boost::archive::class_id_type, int) {}
    void load_override(boost::archive::class_id_optional_type, int) {}
    void load_override(boost::archive::class_id_reference_type, int) {}
    void load_override(boost::archive::tracking_type, int) {}
    void load_override(boost::archive::class_name_type&, int) {}

    void load_binary(void *, std::size_t) {}

private:
    decoder input;
    std::stack<scope> scope_stack;
};

}
}

BOOST_SERIALIZATION_REGISTER_ARCHIVE(protoc::transenc::iarchive);

#endif /* PROTOC_TRANSENC_ARCHIVE_HPP */
