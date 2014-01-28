#ifndef PROTOC_JSON_IARCHIVE_HPP
#define PROTOC_JSON_IARCHIVE_HPP

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

#include <vector>
#include <map>
#include <utility> // std::pair
#include <stack>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/ref.hpp>
#include <boost/optional.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/archive/detail/common_iarchive.hpp>
#include <boost/archive/detail/register_archive.hpp>
#include <protoc/types.hpp>
#include <protoc/json/decoder.hpp>

namespace protoc
{
namespace json
{

class iarchive : public boost::archive::detail::common_iarchive<iarchive>
{
    friend class boost::archive::load_access;

public:
    iarchive(const char *begin, const char *end);
    ~iarchive();

    void load_override(boost::serialization::nvp<bool>, int);
    void load_override(boost::serialization::nvp<protoc::int64_t>, int);
    void load_override(boost::serialization::nvp<protoc::float64_t>, int);
    void load_override(boost::serialization::nvp<std::string>, int);

    // std::pair
    template<typename first_type, typename second_type>
    void load_override(const boost::serialization::nvp< std::pair<first_type, second_type> >& data, int)
    {
        scope<array_frame> current(this);

        *this >> boost::serialization::make_nvp("first", data.value().first);
        current.separator();
        *this >> boost::serialization::make_nvp("second", data.value().second);
    }

    // boost::optional
    template<typename value_type>
    void load_override(const boost::serialization::nvp< boost::optional<value_type> >& data, int)
    {
        const detail::token type = input.type();
        if (type == detail::token_null)
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
        scope<array_frame> current(this);

        while (!current.at_end())
        {
            value_type item;
            *this >> boost::serialization::make_nvp(data.name(), item);
            data.value().push_back(item);
            current.separator();
        }
    }

    // std::map
    template<typename key_type, typename mapped_type, typename key_compare, typename allocator_type>
    void load_override(const boost::serialization::nvp< std::map<key_type, mapped_type, key_compare, allocator_type> > data, int)
    {
        scope<array_frame> current(this);

        while (!current.at_end())
        {
            std::pair<key_type, mapped_type> value;
            *this >> boost::serialization::make_nvp(data.name(), value);
            data.value().insert(value);
            current.separator();
        }
    }

    // Specialization for std::map<std::string, T>
    template<typename mapped_type, typename key_compare, typename allocator_type>
    void load_override(const boost::serialization::nvp< std::map<std::string, mapped_type, key_compare, allocator_type> > data, int)
    {
        scope<object_frame> current(this);

        while (!current.at_end())
        {
            std::string key;
            *this >> boost::serialization::make_nvp(data.name(), key);
            current.separator();
            mapped_type value;
            *this >> boost::serialization::make_nvp(data.name(), value);
            data.value().insert(std::make_pair(key, value));
            current.separator();
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
    class frame
    {
    public:
        frame(detail::decoder&);
        virtual ~frame();

        virtual void get_separator() = 0;
        virtual bool at_end() const = 0;

        void throw_unexpected_token(const detail::token);

    protected:
        detail::decoder& input;
    };

    class array_frame : public frame
    {
    public:
        array_frame(detail::decoder&);
        ~array_frame();

        void get_separator();
        bool at_end() const;
    };

    class object_frame : public frame
    {
    public:
        object_frame(detail::decoder&);
        ~object_frame();

        void get_separator();
        bool at_end() const;

    private:
        std::size_t counter;
    };

    template<typename T> friend class scope;
    template<typename T>
    class scope
    {
    public:
        scope(iarchive * self)
            : self(self)
        {
            self->stack.push(boost::make_shared<T>(boost::ref(self->input)));
        }

        ~scope()
        {
            self->stack.pop();
        }

        void separator()
        {
            self->stack.top()->get_separator();
        }

        bool at_end() const
        {
            return self->stack.top()->at_end();
        }

    private:
        iarchive * self;
    };

private:
    detail::decoder input;
    std::stack< boost::shared_ptr<frame> > stack;
};

} // namespace json
} // namespace protoc

BOOST_SERIALIZATION_REGISTER_ARCHIVE(protoc::json::iarchive);


#endif // PROTOC_JSON_IARCHIVE_HPP
