/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/diplodocusdb/physical-storage/blob/main/LICENSE.txt
*/

#ifndef GUARD_DIPLODOCUSDB_PHYSICALSTORAGE_PAGEFILEREPOSITORY2_HPP
#define GUARD_DIPLODOCUSDB_PHYSICALSTORAGE_PAGEFILEREPOSITORY2_HPP

#include "PageFileRepository.h"
#include "Page2.hpp"
#include <Ishiko/Errors.hpp>
#include <memory>

namespace DiplodocusDB
{
namespace PhysicalStorage
{

class PageFileRepository2
{
public:
    void create(const boost::filesystem::path& path, Ishiko::Error& error);
    void open(const boost::filesystem::path& path, Ishiko::Error& error);
    void close();

    size_t pageCount();
    std::shared_ptr<Page2> page(size_t index, Ishiko::Error& error);
    std::shared_ptr<Page2> allocatePage(Ishiko::Error& error);
    std::shared_ptr<Page2> insertPageAfter(Page2& page, Ishiko::Error& error);
    void store(const Page2& page, Ishiko::Error& error);

    PageRepositoryReader read(const PageRepositoryPosition& pos, Ishiko::Error& error);
    PageRepositoryReader read(size_t startPage, size_t offset, Ishiko::Error& error);
    PageRepositoryReader read(std::shared_ptr<Page2> startPage, size_t offset, Ishiko::Error& error);

    PageRepositoryWriter insert(const PageRepositoryPosition& pos, Ishiko::Error& error);
    PageRepositoryWriter insert(size_t startPage, size_t offset, Ishiko::Error& error);
    PageRepositoryWriter insert(std::shared_ptr<Page2> startPage, size_t offset, Ishiko::Error& error);

private:
    PageFileRepository m_page_file_repository;
};

}
}

#endif
