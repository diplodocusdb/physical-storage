/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/diplodocusdb/physical-storage/blob/main/LICENSE.txt
*/

#ifndef _DIPLODOCUSDB_PHYSICALSTORAGE_PAGEREPOSITORYERRORCATEGORY_HPP_
#define _DIPLODOCUSDB_PHYSICALSTORAGE_PAGEREPOSITORYERRORCATEGORY_HPP_

#include <Ishiko/Errors.hpp>

namespace DiplodocusDB
{

class PageRepositoryErrorCategory : public Ishiko::ErrorCategory
{
public:
    enum EErrorValues
    {
        eGeneric = -1
    };

    static const PageRepositoryErrorCategory& Get() noexcept;

    const char* name() const noexcept override;

private:
    PageRepositoryErrorCategory() noexcept = default;
};

void Fail(Ishiko::Error& error, PageRepositoryErrorCategory::EErrorValues value) noexcept;
void Fail(Ishiko::Error& error, PageRepositoryErrorCategory::EErrorValues value, const std::string& message,
    const char* file, int line) noexcept;

}

#endif
