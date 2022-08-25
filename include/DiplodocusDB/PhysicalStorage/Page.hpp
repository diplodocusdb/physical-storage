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

struct Page
{
    /// Fills the contents of the page with zeroes.
    void init();

    static const size_t sm_size = 4096;

    size_t number;
    Ishiko::ByteBuffer data{sm_size};
};

}
}

#endif
