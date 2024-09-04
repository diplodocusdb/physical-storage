/*
    Copyright (c) 2022-2024 Xavier Leclercq
    Released under the MIT License
    See https://github.com/diplodocusdb/physical-storage/blob/main/LICENSE.txt
*/

#include "PhysicalStorageErrorCategory.hpp"

using namespace DiplodocusDB::PhysicalStorage;

const PhysicalStorageErrorCategory& PhysicalStorageErrorCategory::Get() noexcept
{
    static PhysicalStorageErrorCategory theCategory;
    return theCategory;
}

const char* PhysicalStorageErrorCategory::name() const noexcept
{
    return "DiplodocusDB::PhypsicalStorageErrorCategory";
}

const char* PhysicalStorageErrorCategory::message(int ev, char* buffer, size_t len) const noexcept
{
    switch (static_cast<Value>(ev))
    {
    case Value::generic_error:
        return "generic error";

    default:
        return "unknown value";
    }
}

void DiplodocusDB::PhysicalStorage::Fail(Ishiko::Error& error, PhysicalStorageErrorCategory::Value value) noexcept
{
    error.fail(PhysicalStorageErrorCategory::Get(), static_cast<int>(value));
}

void DiplodocusDB::PhysicalStorage::Fail(Ishiko::Error& error, PhysicalStorageErrorCategory::Value value,
    const std::string& message, const char* file, int line) noexcept
{
    error.fail(PhysicalStorageErrorCategory::Get(), static_cast<int>(value), message, file, line);
}
