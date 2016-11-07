#include "UnqliteDatabase.h"

UnqliteDatabase::~UnqliteDatabase()
{
    close();
}

bool UnqliteDatabase::open(const std::string &databaseFile,
                           unsigned int mode /* = UNQLITE_OPEN_CREATE */)
{
    auto result = unqlite_open(&m_database, databaseFile.c_str(), mode);
    return result == UNQLITE_OK;
}

bool UnqliteDatabase::isOpen() const
{
    return m_database != nullptr;
}

bool UnqliteDatabase::close()
{
    if (m_database == nullptr)
    {
        return true;
    }

    auto result = unqlite_close(m_database);
    if (result == UNQLITE_OK)
    {
        m_database = nullptr;
        return true;
    }

    return false;
}

std::string UnqliteDatabase::lastError() const
{
    const char *buffer;
    int bufferLength;

    unqlite_config(m_database, UNQLITE_CONFIG_ERR_LOG, &buffer, &bufferLength);
    if (bufferLength > 0)
    {
        return std::string(buffer);
    }

    unqlite_config(m_database, UNQLITE_CONFIG_JX9_ERR_LOG, &buffer, &bufferLength);
    if (bufferLength > 0)
    {
        return std::string(buffer);
    }

    return std::string();
}

unqlite* UnqliteDatabase::handler() const
{
    return m_database;
}

bool UnqliteDatabase::enableThreadSafe(bool enable)
{
    int result;
    if (enable)
    {
        result = unqlite_lib_config(UNQLITE_LIB_CONFIG_THREAD_LEVEL_MULTI);
    }
    else
    {
        result = unqlite_lib_config(UNQLITE_LIB_CONFIG_THREAD_LEVEL_SINGLE);
    }

    return result == UNQLITE_OK;
}
