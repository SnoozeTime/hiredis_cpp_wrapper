#ifndef JSON_SERIALIZER_HPP
#define JSON_SERIALIZER_HPP

#include "base_serializer.hpp"

/*
    Just return value as a string
*/

class StringSerializer : public BaseSerializer<std::string>
{
public:

    std::string serialize(std::string object)
    {
        return object;
    }

    std::string deserialize(std::string str_value)
    {
        return str_value;
    }

};

#endif
