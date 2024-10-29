// SPDX-FileCopyrightText: 2018-2024 Xavier Leclercq
// SPDX-License-Identifier: MIT

#ifndef GUARD_DIPLODOCUSDB_PHYSICALSTORAGE_PAGE_HPP
#define GUARD_DIPLODOCUSDB_PHYSICALSTORAGE_PAGE_HPP

#include <Ishiko/Memory.hpp>
#include <cstring>
#include <stddef.h>

namespace DiplodocusDB
{
namespace PhysicalStorage
{

struct Page
{
    size_t number{0};
    Ishiko::HeapByteBuffer data{sm_size};

    /// Constructor.
    /**
        Note that the page contents are not initialized by this constructor. Use the init() function to initialize the
        page to all zeroes.

        @param index The index of the page.
    */
    inline Page(size_t number);

    /// Fills the contents of the page with zeroes.
    inline void zero() noexcept;

    static const size_t sm_size = 4096;
};

}
}

DiplodocusDB::PhysicalStorage::Page::Page(size_t number)
    : number(number)
{
}

void DiplodocusDB::PhysicalStorage::Page::zero() noexcept
{
    memset(data.data(), 0, sm_size);
}

#endif
