#include "UnqliteQuery.h"

#include <cstdlib>
#include <sstream>

UnqliteQuery::UnqliteQuery(const UnqliteDatabase &database)
    : m_database(database)
{
    // pass
}

UnqliteQuery::~UnqliteQuery()
{
    if (m_activeTransaction)
    {
        rollback();
    }
}

bool UnqliteQuery::storeString(const std::string &key,
                               const std::string &value)
{
    auto result = unqlite_kv_store(m_database.handler(), key.c_str(), -1, value.c_str(), value.size());
    return result == UNQLITE_OK;
}

bool UnqliteQuery::storeInt(const std::string &key,
                            const int value)
{
    auto result = unqlite_kv_store(m_database.handler(), key.c_str(), -1, &value, sizeof(value));
    return result == UNQLITE_OK;
}

std::pair<std::string, bool> UnqliteQuery::fetchString(const std::string &key)
{
    std::pair<std::string, bool> output(std::string(), false);
    unqlite_int64 numberOfBytes;

    auto result = unqlite_kv_fetch(m_database.handler(), key.c_str(), -1, nullptr, &numberOfBytes);
    if (result != UNQLITE_OK )
    {
        return output;
    }

    if (numberOfBytes <= 0)
    {
        return output;
    }

    char *buffer = (char*)malloc(numberOfBytes);
    if (buffer == nullptr)
    {
        return output;
    }

    result = unqlite_kv_fetch(m_database.handler(), key.c_str(), -1, buffer, &numberOfBytes);
    if (result != UNQLITE_OK)
    {
        free(buffer);
        return output;
    }

    output.first = std::string(buffer, numberOfBytes);
    output.second = true;

    free(buffer);

    return output;
}

std::pair<int, bool> UnqliteQuery::fetchInt(const std::string &key)
{
    std::pair<int, bool> output(0, false);

    char buffer[sizeof(int)];
    unqlite_int64 numberOfBytes;
    auto result = unqlite_kv_fetch(m_database.handler(), key.c_str(), -1, buffer, &numberOfBytes);
    if (result != UNQLITE_OK)
    {
        return output;
    }

    output.first = *((int*)buffer);
    output.second = true;

    return output;
}

bool UnqliteQuery::removeValue(const std::string &key)
{
    auto result = unqlite_kv_delete(m_database.handler(), key.c_str(), -1);
    return result == UNQLITE_OK;
}

bool UnqliteQuery::storeDocument(const std::string &collection,
                                 const std::string &document)
{
    std::ostringstream oss;
    oss << "if (!db_exists('" << collection << "')) {"
        << "db_create('" << collection << "'); }"
        << " db_store('" << collection << "', " << document << ");";

    unqlite_vm *vmResult;
    auto result = unqlite_compile(m_database.handler(), oss.str().c_str(), -1, &vmResult);

    if (result != UNQLITE_OK)
    {
        return false;
    }

    result = unqlite_vm_exec(vmResult);
    if (result != UNQLITE_OK)
    {
        unqlite_vm_release(vmResult);
        return false;
    }

    unqlite_vm_release(vmResult);

    return true;
}

std::pair<std::string, bool> UnqliteQuery::fetchDocument(const std::string &collection,
                                                         const std::string &filter)
{
    std::pair<std::string, bool> output(std::string(), false);

    auto comma = "";
    if (!filter.empty())
    {
        comma = ",";
    }

    std::ostringstream oss;
    oss << "$result = db_fetch_all('" << collection << "' " << comma << filter << ");";

    unqlite_vm *vmResult;
    auto result = unqlite_compile(m_database.handler(), oss.str().c_str(), -1, &vmResult);

    if (result != UNQLITE_OK)
    {
        return output;
    }

    result = unqlite_vm_exec(vmResult);
    if (result != UNQLITE_OK)
    {
        unqlite_vm_release(vmResult);
        return output;
    }

    auto unqliteData = unqlite_vm_extract_variable(vmResult, "result");
    if (nullptr == unqliteData)
    {
        unqlite_vm_release(vmResult);
        return output;
    }

    output.first = unqlite_value_to_string(unqliteData, nullptr);
    output.second = true;

    unqlite_vm_release(vmResult);

    return output;
}

bool UnqliteQuery::begin()
{
    if (unqlite_begin(m_database.handler()) == UNQLITE_OK)
    {
        m_activeTransaction = true;
        return true;
    }

    return false;
}

bool UnqliteQuery::commit()
{
    if (unqlite_commit(m_database.handler()) == UNQLITE_OK)
    {
        m_activeTransaction = false;
        return true;
    }

    return false;
}

bool UnqliteQuery::rollback()
{
    if (unqlite_rollback(m_database.handler()) == UNQLITE_OK)
    {
        m_activeTransaction = false;
        return true;
    }

    return false;
}

std::string UnqliteQuery::lastError() const
{
    return m_database.lastError();
}
