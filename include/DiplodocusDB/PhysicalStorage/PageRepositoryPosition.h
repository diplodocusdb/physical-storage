/*
    Copyright (c) 2018-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/diplodocusdb/physical-storage/blob/main/LICENSE.txt
*/

#ifndef _DIPLODOCUSDB_PHYSICALSTORAGE_PAGEREPOSITORY_PAGEREPOSITORYPOSITION_H_
#define _DIPLODOCUSDB_PHYSICALSTORAGE_PAGEREPOSITORY_PAGEREPOSITORYPOSITION_H_

#include <cstddef>

namespace DiplodocusDB
{
namespace PhysicalStorage
{

class PageRepositoryPosition
{
public:
    PageRepositoryPosition(size_t page, size_t offset);
    ~PageRepositoryPosition();

    size_t page() const;
    size_t offset() const;

private:
    size_t m_page;
    size_t m_offset;
};

}
}

#endif
