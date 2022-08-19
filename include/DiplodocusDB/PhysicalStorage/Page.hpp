/*
    Copyright (c) 2018-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/diplodocusdb/physical-storage/blob/main/LICENSE.txt
*/

#ifndef GUARD_DIPLODOCUSDB_PHYSICALSTORAGE_PAGE_HPP
#define GUARD_DIPLODOCUSDB_PHYSICALSTORAGE_PAGE_HPP

#include <Ishiko/Memory.hpp>
#include <stddef.h>

namespace DiplodocusDB
{
namespace PhysicalStorage
{

class Page
{
public:
    /// Constructor.
    /**
        Note that the page contents are not initialized by this constructor. Use the init() function to initialize the
        page to all zeroes.

        @param index The index of the page.
    */
    Page(size_t number);

    /// Fills the contents of the page with zeroes.
    void init();

    /// Returns the number of the page.
    /**
        @returns The number of the page.
    */
    size_t number() const;
    inline const Ishiko::Buffer& buffer() const noexcept;
    inline Ishiko::Buffer& buffer() noexcept;

    static const size_t sm_size = 4096;

private:
    size_t m_number;
    Ishiko::Buffer m_buffer{sm_size};
};

}
}

const Ishiko::Buffer& DiplodocusDB::PhysicalStorage::Page::buffer() const noexcept
{
    return m_buffer;
}

Ishiko::Buffer& DiplodocusDB::PhysicalStorage::Page::buffer() noexcept
{
    return m_buffer;
}

#endif
