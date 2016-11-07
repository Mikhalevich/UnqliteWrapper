#pragma once

#include <string>

extern "C" {
    #include "unqlite.h"
}

class UnqliteDatabase
{
public:
    ~UnqliteDatabase();
    bool open(const std::string &databaseFile,
              unsigned int mode = UNQLITE_OPEN_CREATE);
    bool isOpen() const;
    bool close();
    std::string lastError() const;
    unqlite* handler() const;
    bool enableThreadSafe(bool enable);

private:
    unqlite *m_database = nullptr;
};
