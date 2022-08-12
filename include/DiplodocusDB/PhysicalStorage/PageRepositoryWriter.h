/*
    Copyright (c) 2018-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/diplodocusdb/physical-storage/blob/main/LICENSE.txt
*/

#ifndef GUARD_DIPLODOCUSDB_PHYSICALSTORAGE_PAGEREPOSITORYWRITER_H
#define GUARD_DIPLODOCUSDB_PHYSICALSTORAGE_PAGEREPOSITORYWRITER_H

#include "Page2.hpp"
#include "PageRepositoryPosition.h"
#include <Ishiko/Errors.hpp>
#include <set>
#include <memory>

namespace DiplodocusDB
{
namespace PhysicalStorage
{

class PageFileRepository2;

class PageRepositoryWriter
{
public:
    PageRepositoryWriter(PageFileRepository2& repository, std::shared_ptr<Page2> startPage, size_t startOffset);
    
    PageRepositoryPosition currentPosition() const;

    void write(const char* buffer, size_t bufferSize, Ishiko::Error& error);
    void writeLEB128(size_t value, Ishiko::Error& error);
    void save(Ishiko::Error& error);

private:
    PageFileRepository2& m_repository;
    std::shared_ptr<Page2> m_currentPage;
    size_t m_currentOffset;
    std::set<std::shared_ptr<Page2>> m_updatedPages;
};

}
}

#endif
