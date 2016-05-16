#ifndef REDISWRAPPER_HPP
#define REDISWRAPPER_HPP

#include <hiredis/hiredis.h>
#include <string>
#include <map>
#include <memory>
#include <vector>

#include "serializers/serializers.hpp"
#include "helpers.hpp"


class RedisInterface
{
public:

    /*
     * Connect to a redis instance given a server, port and optional timeout.
     * If timeout is 0, will block until function returns.
     * Timeout is in milliseconds
     *
     * Raise exception if failed
     * */
    RedisInterface(std::string hostname, int port, unsigned int timeout = 0);

    /*
     * Clean the redis object.
     * */
    ~RedisInterface();

    /*
     * Get a value from redis database given a key
     * Get will return a string
     * */
    std::string get(std::string key);

    /*
    * Select the database (usually between 0 and 16)
    */
    bool select(unsigned int database_nb);

    /*
     * Set the key with the given value.
     * Set will return true if ok
     * */
    bool set(std::string key, std::string value);

    /*
     * Push a value at the left of a list.
     * return true if success
     * */
    bool lpush(std::string key, std::string value);

    /*
     * Now we can use the syntax lpush(key, value1, value2, value3);
     * */
    template<typename ... Args>
    bool lpush(std::string key, Args ... values)
    {
        return lpush(key, concat(' ', values ...));
    }

    /*
     * Push a value to the right side of a list.
     * (+varadic version)
     * */
    bool rpush(std::string key, std::string value);

    template <typename ... Args>
    bool rpush(std::string key, Args ... values)
    {
        return rpush(key, concat(' ', values ...));
    }
    /*
     * Right pop.
     * Return right value of a list. If empty, return empty string.
     * */
    std::string rpop(std::string key);

    /*
     * Left pop
     * return left value of a list. If empty, return empty string
     * */
    std::string lpop(std::string key);

    /*
     * Blocking left pop.
     * Input, list keys and eventually a timeout in second.
     * return key -> value (for the first lsit non empty in order)
     * */
    std::pair<std::string, std::string> blpop(std::string key, unsigned int timeout_sec = 0);

    // Here the syntax changes a little to allow varadic template AND timeout
    template <typename ... Args>
    std::pair<std::string, std::string> blpop(unsigned int timeout_sec, Args ... keys)
    {
        return blpop(concat(' ', keys...), timeout_sec);
    }

    /*
     * Blocking right pop. See above
     * */
    std::pair<std::string, std::string> brpop(std::string key, unsigned int timeout_sec = 0);

    // Here the syntax changes a little to allow varadic template AND timeout
    template <typename ... Args>
    std::pair<std::string, std::string> brpop(unsigned int timeout_sec, Args ... keys)
    {
        return brpop(concat(' ', keys...), timeout_sec);
    }

    /*
        Get values from a list
        If you do not precise begin and end, it will give you the entire list.

        Return a vector of string which contains the values in order. Empty vector
        if list does not exist or values not in given range
    */
    std::vector<std::string> lrange(std::string key, int begin = 0, int end = -1);

    /*
        Will return the redisContext associated to the object.
        DO NOT call redisFree with the returned context, otherwise the other commands
        will raise segfault.
    */
    redisContext* get_context();

private:

    /*
     * Object used to interact with redis. is initialized in constructor and
     * memory is freed in destructor
     * */
    redisContext* context_;


};
#endif
