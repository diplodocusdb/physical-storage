/*
    Copyright (c) 2018-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/diplodocusdb/physical-storage/blob/main/LICENSE.txt
*/

#ifndef GUARD_DIPLODOCUSDB_PHYSICALSTORAGE_PAGEREPOSITORY_H
#define GUARD_DIPLODOCUSDB_PHYSICALSTORAGE_PAGEREPOSITORY_H

#include "Page2.hpp"
#include "PageRepositoryPosition.h"
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
    virtual std::shared_ptr<Page2> page(size_t index, Ishiko::Error& error) = 0;
    virtual std::shared_ptr<Page2> allocatePage(Ishiko::Error& error) = 0;
    virtual void store(const Page2& page, Ishiko::Error& error) = 0;

    virtual void replace() = 0;
    virtual void erase() = 0;
};

}
}

#endif
