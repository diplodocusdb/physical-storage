/*
    Copyright (c) 2018-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/diplodocusdb/physical-storage/blob/main/LICENSE.txt
*/

#ifndef GUARD_DIPLODOCUSDB_PHYSICALSTORAGE_PAGEREPOSITORYREADER_H
#define GUARD_DIPLODOCUSDB_PHYSICALSTORAGE_PAGEREPOSITORYREADER_H

#include "Page2.hpp"
#include "PageRepositoryPosition.h"
#include <Ishiko/Errors.hpp>
#include <memory>

namespace DiplodocusDB
{
namespace PhysicalStorage
{

class PageFileRepository2;

class PageRepositoryReader
{
public:
    PageRepositoryReader(PageFileRepository2& repository, std::shared_ptr<Page2> startPage, size_t startOffset);

    PageRepositoryPosition currentPosition() const;

    void read(char* buffer, size_t n, Ishiko::Error& error);
    size_t readLEB128(Ishiko::Error& error);

private:
    PageFileRepository2& m_repository;
    std::shared_ptr<Page2> m_currentPage;
    size_t m_currentOffset;
};

}
}

#endif
