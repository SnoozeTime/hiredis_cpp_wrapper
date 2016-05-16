Introduction
------------

This repository provides a wrapper around the C library hiredis. Instead of using
the C API, you can use an wrapper object instead which will automatically clean
the C objects, and provides simple function for the most used commands.

This code is designed for one of my project so there is not all the features you
would expect from a real C++ redis client. However, the C++ wrapper still provides
a way to use directly the C object so you will have access to all the features from
hiredis.

Dependencies
------------
The C library hiredis is used. Because currently there are compilation errors with
sdc and g++ (see https://github.com/redis/hiredis/pull/428, https://github.com/redis/hiredis/pull/412). I forked the repo
with a fix in the meantime.
https://github.com/SnoozeTime/hiredis-cpp

Json cpp is also used for serialization in the examples but it is not necessary
for the core library.

To do
-----

* Add unit tests
* More features

Usage
-----
First, create a RedisInterface object. It will take the hostname, port of the redis
server, and a timeout (= 0 for no timeout) for the connection to the server.

```c++
RedisInterface redis("127.0.0.1", 6379, 1500);
```

Then, you can do the following basic operations:

* Get a key
* Set a key
* push value(s) to a list (left or right)
* pop a value from a list (left or right)
* pop or block if the list is empty.
* Select a database

```c++
/*
    Return "" if no value
*/
std::string value = redis.get("key");

bool success = redis.set("key", "value");

/*
    You can push 1 to n values to a list
*/
bool success = redis.lpush("mylist", "1");
success = redis.rpush("mylist", "1", "abcv", "another value");

/*
    lpop for non blocking left pop and blpop for blocking left pop
*/
unsigned int timeout = 1; // 1 second
std::string value = redis.blpop("thekey", timeout);

// If using multiple keys, timeout comes first
std::string value = redis.blpop(0, "list1", "list2");

// and so on
```
