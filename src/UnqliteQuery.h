#pragma once

#include "UnqliteDatabase.h"

#include <string>
#include <utility>

class UnqliteQuery
{
public:
    UnqliteQuery(const UnqliteDatabase &database);
    ~UnqliteQuery();

    // key/value
    bool storeString(const std::string &key,
                     const std::string &value);
    bool storeInt(const std::string &key,
                  const int value);

    std::pair<std::string, bool> fetchString(const std::string &key);
    std::pair<int, bool> fetchInt(const std::string &key);

    bool removeValue(const std::string &key);

    // document
    bool storeDocument(const std::string &collection,
                       const std::string &document);
    std::pair<std::string, bool> fetchDocument(const std::string &collection,
                                               const std::string &filter);

    // transaction
    bool begin();
    bool commit();
    bool rollback();

    // other
    std::string lastError() const;

private:
    const UnqliteDatabase &m_database;
    bool m_activeTransaction = false;
};
