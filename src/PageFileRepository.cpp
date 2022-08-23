/*
    Copyright (c) 2018-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/diplodocusdb/physical-storage/blob/main/LICENSE.txt
*/

#include "PageFileRepository.h"
#include "PhysicalStorageErrorCategory.hpp"
#include <boost/filesystem/operations.hpp>

using namespace DiplodocusDB::PhysicalStorage;

PageFileRepository::PageFileRepository()
    : m_pageCount{0}
{
}

void PageFileRepository::create(const boost::filesystem::path& path,
                                Ishiko::Error& error)
{
    std::fstream file(path.c_str(), std::fstream::out | std::fstream::binary);
    if (!file.good())
    {
        // TODO add details
        Fail(error, PhysicalStorageErrorCategory::Value::generic_error, "Failed to create file", __FILE__, __LINE__);
    }
    else
    {
        file.close();
        open(path, error);
    }
}

void PageFileRepository::open(const boost::filesystem::path& path,
                              Ishiko::Error& error)
{
    boost::system::error_code ec;
    boost::uintmax_t filesize = boost::filesystem::file_size(path, ec);
    if (ec)
    {
        // TODO add details
        Fail(error, PhysicalStorageErrorCategory::Value::generic_error, "Failed to get file size", __FILE__, __LINE__);
    }
    else
    {
        m_file.open(path, error);
        if (!error)
        {
            m_pageCount = (filesize / Page::sm_size);
        }
    }
}

void PageFileRepository::close()
{
    m_file.close();
}

size_t PageFileRepository::pageCount()
{
    return m_pageCount;
}

std::shared_ptr<Page2> PageFileRepository::page(size_t index, Ishiko::Error& error)
{
    std::shared_ptr<Page2> result;
    if (index <= m_pageCount)
    {
        bool foundInCache = m_pageCache.get(index, result);
        if (!foundInCache)
        {
            result = std::make_shared<Page2>(index);
            result->read(*this, error);
            if (!error)
            {
                m_pageCache.set(result);
            }
        }
    }
    else
    {
        // TODO
        Fail(error, PhysicalStorageErrorCategory::Value::generic_error);
    }
    return result;
}

std::shared_ptr<Page2> PageFileRepository::allocatePage(Ishiko::Error& error)
{
    std::shared_ptr<Page2> page = std::make_shared<Page2>(m_pageCount);
    page->init();
    m_file.resize((m_pageCount + 1) * Page::sm_size);
    //m_pageCache.set(page);
    ++m_pageCount;
    return page;
}

void PageFileRepository::store(const Page2& page, Ishiko::Error& error)
{
    page.write(*this, error);
}

void PageFileRepository::replace()
{
}

void PageFileRepository::erase()
{
}
