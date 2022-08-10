/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/diplodocusdb/physical-storage/blob/main/LICENSE.txt
*/

#include "PageRepositoryErrorCategory.hpp"

using namespace DiplodocusDB;

const PageRepositoryErrorCategory& PageRepositoryErrorCategory::Get() noexcept
{
    static PageRepositoryErrorCategory theCategory;
    return theCategory;
}

const char* PageRepositoryErrorCategory::name() const noexcept
{
    return "DiplodocusDB::PageRepositoryErrorCategory";
}

void DiplodocusDB::Fail(Ishiko::Error& error, PageRepositoryErrorCategory::EErrorValues value) noexcept
{
    error.fail(value, PageRepositoryErrorCategory::Get());
}

void DiplodocusDB::Fail(Ishiko::Error& error, PageRepositoryErrorCategory::EErrorValues value,
    const std::string& message, const char* file, int line) noexcept
{
    error.fail(value, PageRepositoryErrorCategory::Get(), message, file, line);
}
