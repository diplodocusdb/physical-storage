/*
    Copyright (c) 2018-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/diplodocusdb/physical-storage/blob/main/LICENSE.txt
*/

#ifndef GUARD_DIPLODOCUSDB_PHYSICALSTORAGE_PAGECACHE_H
#define GUARD_DIPLODOCUSDB_PHYSICALSTORAGE_PAGECACHE_H

#include "Page.hpp"
#include <map>
#include <memory>

namespace DiplodocusDB
{
namespace PhysicalStorage
{

/// An in-memory cache for the pages.
class PageCache
{
public:
    /// Constructor.
    PageCache() = default;

    bool get(size_t index, std::shared_ptr<Page>& page);
    void set(std::shared_ptr<Page>& page);

private:
    std::map<size_t, std::shared_ptr<Page>> m_pages;
};

}
}

#endif
