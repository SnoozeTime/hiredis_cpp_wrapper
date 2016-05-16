#include "RedisWrapper.hpp"
#include <iostream>
#include "serializers/json_serializer.hpp"

int main()
{
    JsonSerializer json_serializer;
    RedisInterface redis("127.0.0.1", 6379, 1500);
    bool select_ret = redis.select(1);

    if (select_ret) {
        while (true) {
            // template function, cannot pass stirng literals
            std::pair<std::string, std::string> result = redis.blpop(0, std::string("mylist1"),
                                                                     std::string("mylist2"),
                                                                     std::string("mylist3"));
            std::cout << result.first << " -> " << result.second << std::endl;
            try {
                Json::Value root = json_serializer.deserialize(result.second);
                std::cout << root.get("command_name", "Default value").asString() << std::endl;
            } catch (SerializerException &e) {
                std::cerr << "json serializer raised an exception: " << e.what() << std::endl;
            }
        }

    } else {
        std::cerr << "Couldnt connect to database " << 41 << std::endl;
    }
}
