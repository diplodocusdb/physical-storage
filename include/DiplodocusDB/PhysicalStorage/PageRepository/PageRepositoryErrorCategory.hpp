/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/diplodocusdb/physical-storage/blob/main/LICENSE.txt
*/

#ifndef GUARD_DIPLODOCUSDB_PHYSICALSTORAGE_PAGEREPOSITORYERRORCATEGORY_HPP
#define GUARD_DIPLODOCUSDB_PHYSICALSTORAGE_PAGEREPOSITORYERRORCATEGORY_HPP

#include <Ishiko/Errors.hpp>

namespace DiplodocusDB
{

class PageRepositoryErrorCategory : public Ishiko::ErrorCategory
{
public:
    enum class Value
    {
        generic_error = -1
    };

    static const PageRepositoryErrorCategory& Get() noexcept;
    std::ostream& streamOut(int value, std::ostream& os) const override;

    const char* name() const noexcept override;

private:
    PageRepositoryErrorCategory() noexcept = default;
};

void Fail(Ishiko::Error& error, PageRepositoryErrorCategory::Value value) noexcept;
void Fail(Ishiko::Error& error, PageRepositoryErrorCategory::Value value, const std::string& message,
    const char* file, int line) noexcept;

}

#endif
