/*
    Copyright (c) 2018-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/diplodocusdb/physical-storage/blob/main/LICENSE.txt
*/

#ifndef GUARD_DIPLODOCUSDB_PHYSICALSTORAGE_PAGEREPOSITORY_H
#define GUARD_DIPLODOCUSDB_PHYSICALSTORAGE_PAGEREPOSITORY_H

#include "Page.hpp"
#include "PageRepositoryPosition.h"
#include "PageRepositoryReader.h"
#include "PageRepositoryWriter.h"
#include <Ishiko/Errors.hpp>
#include <memory>

namespace DiplodocusDB
{
namespace PhysicalStorage
{

/// Abstract base class for repositories that store a series of consecutive pages of fixed size.
class PageRepository
{
public:
    /// Destructor.
    virtual ~PageRepository() noexcept = default;

    virtual size_t pageCount() = 0;
    virtual std::shared_ptr<Page> page(size_t index, Ishiko::Error& error) = 0;
    virtual std::shared_ptr<Page> allocatePage(Ishiko::Error& error) = 0;
    virtual std::shared_ptr<Page> insertPageAfter(Page& page, Ishiko::Error& error) = 0;
    virtual void save(const Page& page, Ishiko::Error& error) = 0;

    PageRepositoryReader read(const PageRepositoryPosition& pos, Ishiko::Error& error);
    virtual PageRepositoryReader read(size_t startPage, size_t offset, Ishiko::Error& error) = 0;
    virtual PageRepositoryReader read(std::shared_ptr<Page> startPage, size_t offset, Ishiko::Error& error) = 0;

    PageRepositoryWriter insert(const PageRepositoryPosition& pos, Ishiko::Error& error);
    virtual PageRepositoryWriter insert(size_t startPage, size_t offset, Ishiko::Error& error) = 0;
    virtual PageRepositoryWriter insert(std::shared_ptr<Page> startPage, size_t offset, Ishiko::Error& error) = 0;
    virtual void replace() = 0;
    virtual void erase() = 0;
};

}
}

#endif
