/*
    Copyright (c) 2018-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/diplodocusdb/physical-storage/blob/main/LICENSE.txt
*/

#include "PageRepository.h"

namespace DiplodocusDB
{

PageRepositoryReader PageRepository::read(const PageRepositoryPosition& pos,
                                          Ishiko::Error& error)
{
    return read(pos.page(), pos.offset(), error);
}

PageRepositoryWriter PageRepository::insert(const PageRepositoryPosition& pos,
                                            Ishiko::Error& error)
{
    return insert(pos.page(), pos.offset(), error);
}

}
