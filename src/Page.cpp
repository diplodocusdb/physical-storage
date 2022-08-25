/*
    Copyright (c) 2018-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/diplodocusdb/physical-storage/blob/main/LICENSE.txt
*/

#include "Page.hpp"
#include <cstring>

using namespace DiplodocusDB::PhysicalStorage;

Page::Page(size_t number)
    : number(number)
{
}

void Page::init()
{
    memset(data.data(), 0, sm_size);
}
