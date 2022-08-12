/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/diplodocusdb/physical-storage/blob/main/LICENSE.txt
*/

#ifndef GUARD_DIPLODOCUSDB_PHYSICALSTORAGE_PHYSICALSTORAGEERRORCATEGORY_HPP
#define GUARD_DIPLODOCUSDB_PHYSICALSTORAGE_PHYSICALSTORAGEERRORCATEGORY_HPP

#include <Ishiko/Errors.hpp>

namespace DiplodocusDB
{
namespace PhysicalStorage
{

class PhysicalStorageErrorCategory : public Ishiko::ErrorCategory
{
public:
    enum class Value
    {
        generic_error = -1
    };

    static const PhysicalStorageErrorCategory& Get() noexcept;
    std::ostream& streamOut(int value, std::ostream& os) const override;

    const char* name() const noexcept override;

private:
    PhysicalStorageErrorCategory() noexcept = default;
};

void Fail(Ishiko::Error& error, PhysicalStorageErrorCategory::Value value) noexcept;
void Fail(Ishiko::Error& error, PhysicalStorageErrorCategory::Value value, const std::string& message,
    const char* file, int line) noexcept;

}
}

#endif
