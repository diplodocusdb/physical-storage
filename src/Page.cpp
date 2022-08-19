/*
    Copyright (c) 2018-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/diplodocusdb/physical-storage/blob/main/LICENSE.txt
*/

#include "Page.hpp"
#include <cstring>

using namespace DiplodocusDB::PhysicalStorage;

Page::Page(size_t number)
    : m_number(number)
{
}

void Page::init()
{
    memset(m_buffer.data(), 0, sm_size);
}

size_t Page::number() const
{
    return m_number;
}
