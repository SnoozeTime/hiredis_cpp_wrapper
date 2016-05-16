#include "RedisWrapper.hpp"
#include <iostream>
#include <stdexcept>

RedisInterface::RedisInterface(std::string hostname, int port, unsigned int timeout)
{
    struct timeval timeout_struct;
    timeout_struct.tv_sec = timeout / 1000;;
    timeout_struct.tv_usec = (timeout % 1000) * 1000;
    context_ = redisConnectWithTimeout(hostname.c_str(), port, timeout_struct);

    if (context_ == nullptr || context_->err) {
        if (context_) {
            std::string error = "Connection error: " + std::string(context_->errstr);
            throw std::runtime_error(error);
        } else {
            throw std::runtime_error("Connection error: cannot allocate redis context");
        }
    }
}

RedisInterface::~RedisInterface()
{
    if (context_ != NULL) {
        redisFree(context_);
    }
}

bool RedisInterface::select(unsigned int database_nb)
{
    bool ret = true;
    redisReply *reply = static_cast<redisReply*>(redisCommand(context_, "SELECT %d", database_nb));
    if (reply->type == REDIS_REPLY_ERROR) {
        ret = false;
    }
    freeReplyObject(reply);

    return ret;
}

std::string RedisInterface::get(std::string key)
{
    std::string ret;
    redisReply* reply = static_cast<redisReply*>(redisCommand(context_, "GET %s", key.c_str()));
    if (reply->type == REDIS_REPLY_STRING) {
        ret = std::string(reply->str, reply->len);
    }

    // Clean reply object
    freeReplyObject(reply);

    return ret;
}

bool RedisInterface::set(std::string key, std::string value)
{
    bool ret = true;
    redisReply *reply = static_cast<redisReply*>(redisCommand(context_, "SET %s %s", key.c_str(), value.c_str()));
    if (reply->type == REDIS_REPLY_ERROR) {
        ret = false;
    }

    //Dont forget to free the memory.
    freeReplyObject(reply);

    return ret;
}

bool RedisInterface::lpush(std::string key, std::string value)
{
    bool ret = true;
    redisReply *reply = static_cast<redisReply*>(redisCommand(context_, "LPUSH %s %s", key.c_str(), value.c_str()));
    if (reply->type == REDIS_REPLY_ERROR) {
        std::cout << "ERRRRROOOOOORRR\n";
        ret = false;
    }
    freeReplyObject(reply);
    return ret;
}

bool RedisInterface::rpush(std::string key, std::string value)
{
    bool ret = true;
    redisReply *reply = static_cast<redisReply*>(redisCommand(context_, "RPUSH %s %s", key.c_str(), value.c_str()));
    if (reply->type == REDIS_REPLY_ERROR) {
        ret = false;
    }
    freeReplyObject(reply);
    return ret;
}

std::string RedisInterface::rpop(std::string key)
{
    std::string ret;
    redisReply *reply = static_cast<redisReply*>(redisCommand(context_, "RPOP %s", key.c_str()));
    if (reply->type == REDIS_REPLY_STRING) {
        ret = std::string(reply->str, reply->len);
    }
    freeReplyObject(reply);
    return ret;
}

std::string RedisInterface::lpop(std::string key)
{
    std::string ret;
    redisReply *reply = static_cast<redisReply*>(redisCommand(context_, "LPOP %s", key.c_str()));
    if (reply->type == REDIS_REPLY_STRING) {
        ret = std::string(reply->str, reply->len);
    }
    freeReplyObject(reply);
    return ret;
}

std::pair<std::string, std::string> RedisInterface::blpop(std::string key, unsigned int timeout_sec)
{
    std::pair<std::string, std::string> ret_tuple;
    std::string command = "BLPOP " + key + " " + std::to_string(timeout_sec);
    redisReply *reply = static_cast<redisReply*>(redisCommand(context_, command.c_str()));
    if (reply->type == REDIS_REPLY_ARRAY && reply->elements == 2 &&
            reply->element[0]->type == REDIS_REPLY_STRING &&
            reply->element[1]->type == REDIS_REPLY_STRING) {
        ret_tuple = std::make_pair(std::string(reply->element[0]->str, reply->element[0]->len),
                                   std::string(reply->element[1]->str, reply->element[1]->len));
    }
    freeReplyObject(reply);
    return ret_tuple;
}

std::pair<std::string, std::string> RedisInterface::brpop(std::string key, unsigned int timeout_sec)
{
    std::pair<std::string, std::string> ret_tuple;
    std::string command = "BRPOP " + key + " " + std::to_string(timeout_sec);
    redisReply *reply = static_cast<redisReply*>(redisCommand(context_, command.c_str()));

    if (reply->type == REDIS_REPLY_ARRAY && reply->elements == 2 &&
            reply->element[0]->type == REDIS_REPLY_STRING &&
            reply->element[1]->type == REDIS_REPLY_STRING) {
        ret_tuple = std::make_pair(std::string(reply->element[0]->str, reply->element[0]->len),
                                   std::string(reply->element[1]->str, reply->element[1]->len));
    }
    freeReplyObject(reply);
    return ret_tuple;
}

std::vector<std::string> RedisInterface::lrange(std::string key, int begin, int end)
{
    std::vector<std::string> values;
    redisReply *reply = static_cast<redisReply*>(redisCommand(context_, "LRANGE %s %d %d", key.c_str(), begin, end));

    // We get either an array, a nil or an error here
    if (reply->type == REDIS_REPLY_ARRAY) {
        for (int i = 0; i < reply->elements; i++) {
            if (reply->element[i]->type == REDIS_REPLY_STRING) {
                values.push_back(std::string(reply->element[i]->str, reply->element[i]->len));
            }
        }
    }
    freeReplyObject(reply);
    return values;
}


redisContext* RedisInterface::get_context()
{
    return context_;
}
