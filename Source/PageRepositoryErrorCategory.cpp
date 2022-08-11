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

std::ostream& PageRepositoryErrorCategory::streamOut(int value, std::ostream& os) const
{
    switch (static_cast<Value>(value))
    {
    case Value::generic_error:
        os << "generic error";
        break;

    default:
        os << "unknown value";
        break;
    }
    return os;
}

void DiplodocusDB::Fail(Ishiko::Error& error, PageRepositoryErrorCategory::Value value) noexcept
{
    error.fail(PageRepositoryErrorCategory::Get(), static_cast<int>(value));
}

void DiplodocusDB::Fail(Ishiko::Error& error, PageRepositoryErrorCategory::Value value,
    const std::string& message, const char* file, int line) noexcept
{
    error.fail(PageRepositoryErrorCategory::Get(), static_cast<int>(value), message, file, line);
}
