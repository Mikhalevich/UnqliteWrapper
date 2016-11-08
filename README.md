# simple c++ wrapper on unqlite api
```
#include "UnqliteDatabase.h"
#include "UnqliteQuery.h"

#include <iostream>

int main(int argc, char **argv)
{
    UnqliteDatabase db;
    if (!db.open("example.db"))
    {
        std::cout << "Unable to open database: " << db.lastError() << std::endl;
        return 1;
    }

    UnqliteQuery query(db);

    // key value
    if (!query.storeInt("IntKey", 777))
    {
        std::cout << "Unable store int key/value: " << query.lastError() << std::endl;
        return 1;
    }

    if (!query.storeString("StringKey", "Hello World!!!"))
    {
        std::cout << "Unable store string key/value: " << query.lastError() << std::endl;
        return 1;
    }

    auto intResult = query.fetchInt("IntKey");
    if (!intResult.second)
    {
        std::cout << "Unable fetch int value:" << query.lastError() << std::endl;
        return 1;
    }
    std::cout << "Fetched int value: " << intResult.first << std::endl;

    auto stringResult = query.fetchString("StringKey");
    if (!stringResult.second)
    {
        std::cout << "Unable fetch string valud: " << query.lastError() << std::endl;
        return 1;
    }
    std::cout << "Fetched string value: " << stringResult.first << std::endl;

    // document
    if (!query.storeDocument("example_collection", "{ first: 70, second: 'Hello'}"))
    {
        std::cout << "Unable store document: " << query.lastError() << std::endl;
        return 1;
    }

    if (!query.storeDocument("example_collection", "{ first: 40, third: 'world'}"))
    {
        std::cout << "Unable store document: " << query.lastError() << std::endl;
        return 1;
    }

    auto docResult = query.fetchDocument("example_collection", "function($rec) { if ($rec.first > 50) { return TRUE; } return FALSE;}");
    if (!docResult.second)
    {
        std::cout << "Unable fetch document: " << query.lastError() << std::endl;
        return 1;
    }
    std::cout << "Fetched document: " << docResult.first << std::endl;

    std::cout << "Done..." << std::endl;
    return 0;
}

```
