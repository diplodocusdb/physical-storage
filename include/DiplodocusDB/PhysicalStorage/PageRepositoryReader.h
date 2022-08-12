/*
    Copyright (c) 2018-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/diplodocusdb/physical-storage/blob/main/LICENSE.txt
*/

#ifndef GUARD_DIPLODOCUSDB_PHYSICALSTORAGE_PAGEREPOSITORYREADER_H
#define GUARD_DIPLODOCUSDB_PHYSICALSTORAGE_PAGEREPOSITORYREADER_H

#include "Page.hpp"
#include "PageRepositoryPosition.h"
#include <Ishiko/Errors.hpp>
#include <memory>

namespace DiplodocusDB
{

class PageRepository;

class PageRepositoryReader
{
public:
    PageRepositoryReader(PageRepository& repository, std::shared_ptr<Page> startPage, size_t startOffset);

    PageRepositoryPosition currentPosition() const;

    void read(char* buffer, size_t n, Ishiko::Error& error);
    size_t readLEB128(Ishiko::Error& error);

private:
    PageRepository& m_repository;
    std::shared_ptr<Page> m_currentPage;
    size_t m_currentOffset;
};

}

#endif
