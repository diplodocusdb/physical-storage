/*
    Copyright (c) 2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/diplodocusdb/physical-storage/blob/main/LICENSE.txt
*/

#include "PageFileRepository2.hpp"

using namespace DiplodocusDB::PhysicalStorage;

void PageFileRepository2::create(const boost::filesystem::path& path, Ishiko::Error& error)
{
    m_page_file_repository.create(path, error);
}

void PageFileRepository2::open(const boost::filesystem::path& path, Ishiko::Error& error)
{
    m_page_file_repository.open(path, error);
}

void PageFileRepository2::close()
{
    m_page_file_repository.close();
}

std::shared_ptr<Page2> PageFileRepository2::page(size_t index, Ishiko::Error& error)
{
    return m_page_file_repository.page(index, error);
}

std::shared_ptr<Page2> PageFileRepository2::allocatePage(Ishiko::Error& error)
{
    return m_page_file_repository.allocatePage(error);
}

std::shared_ptr<Page2> PageFileRepository2::insertPageAfter(Page2& page, Ishiko::Error& error)
{
    std::shared_ptr<Page2> newPage = m_page_file_repository.allocatePage(error);
    if (!error)
    {
        newPage->init();
        newPage->setNextPage(page.nextPage());
        page.setNextPage(newPage->number());
    }
    return newPage;
}

void PageFileRepository2::store(const Page2& page, Ishiko::Error& error)
{
    m_page_file_repository.store(page, error);
}

PageRepositoryWriter PageFileRepository2::insert(const PageRepositoryPosition& pos, Ishiko::Error& error)
{
    return insert(pos.page(), pos.offset(), error);
}

PageRepositoryWriter PageFileRepository2::insert(size_t startPage, size_t offset, Ishiko::Error& error)
{
    std::shared_ptr<Page2> p = page(startPage, error);
    return insert(p, offset, error);
}

PageRepositoryWriter PageFileRepository2::insert(std::shared_ptr<Page2> startPage, size_t offset, Ishiko::Error& error)
{
    return PageRepositoryWriter(*this, startPage, offset);
}
