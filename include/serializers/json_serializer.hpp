#ifndef JSON_SERIALIZER_HPP
#define JSON_SERIALIZER_HPP

#include "base_serializer.hpp"
#include <json/json.h>


class JsonSerializer : public BaseSerializer<Json::Value>
{
public:

    std::string serialize(Json::Value object)
    {
        Json::StreamWriterBuilder builder;
        builder.settings_["indentation"] = "";
        return Json::writeString(builder, object);
    }

    Json::Value deserialize(std::string str_value)
    {
        Json::Value root;
        Json::Reader reader;
        bool parsingSuccessful = reader.parse( str_value.c_str(), root );     //parse process
        if ( !parsingSuccessful )
        {
            std::string error_message = "Failed to parse the json string from redis: " + str_value;
            throw SerializerException(error_message);
        }
        return root;
    }

};

#endif
