/*
    Copyright (c) 2018-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/diplodocusdb/physical-storage/blob/main/LICENSE.txt
*/

#ifndef GUARD_DIPLODOCUSDB_PHYSICALSTORAGE_PAGEREPOSITORY_HPP
#define GUARD_DIPLODOCUSDB_PHYSICALSTORAGE_PAGEREPOSITORY_HPP

#include "Page.hpp"
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
    virtual Page allocatePage(Ishiko::Error& error) = 0;
    virtual Page load(size_t page_number, Ishiko::Error& error) = 0;
    virtual void store(const Page& page, Ishiko::Error& error) = 0;
};

}
}

#endif
