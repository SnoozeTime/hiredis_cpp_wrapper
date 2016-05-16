#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

#include <string>

#include <stdexcept>

/*
 * Exception used in Serializer classes
 * */
class SerializerException: public std::runtime_error
{
public:
    SerializerException(std::string reason):
        std::runtime_error(reason)
    {
    }

    SerializerException(const char *reason):
        std::runtime_error(reason)
    {
    }
};

/*
 * Base class for the serializer used in our redis interface.
 * */

template <typename T>
class BaseSerializer
{
public:
    virtual ~BaseSerializer() {};
    /*
     * Return the object T as a serialized string
     * */
    virtual std::string serialize(T object) = 0;

    /*
     * Return the string as an object T
     * Will raise SerializerException if wrong string format
     * */
    virtual T deserialize(std::string str_value) = 0;
};


#endif
