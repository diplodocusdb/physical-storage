/*
    Copyright (c) 2018-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/diplodocusdb/physical-storage/blob/main/LICENSE.txt
*/

#include "PageRepositoryPosition.h"

using namespace DiplodocusDB::PhysicalStorage;

PageRepositoryPosition::PageRepositoryPosition(size_t page,
                                               size_t offset)
    : m_page(page), m_offset(offset)
{
}

PageRepositoryPosition::~PageRepositoryPosition()
{
}

size_t PageRepositoryPosition::page() const
{
    return m_page;
}

size_t PageRepositoryPosition::offset() const
{
    return m_offset;
}
