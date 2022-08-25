/*
    Copyright (c) 2018-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/diplodocusdb/physical-storage/blob/main/LICENSE.txt
*/

#include "PageFile.hpp"
#include "PhysicalStorageErrorCategory.hpp"
#include <boost/filesystem/operations.hpp>

using namespace DiplodocusDB::PhysicalStorage;

PageFile::PageFile()
    : m_pageCount{0}
{
}

void PageFile::create(const boost::filesystem::path& path, Ishiko::Error& error)
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

void PageFile::open(const boost::filesystem::path& path, Ishiko::Error& error)
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

void PageFile::close()
{
    m_file.close();
}

size_t PageFile::pageCount()
{
    return m_pageCount;
}

Page PageFile::load(size_t page_number, Ishiko::Error& error)
{
    Page loaded_page{page_number};
    if (page_number <= m_pageCount)
    {
        m_file.setFilePointer(page_number * Page::sm_size);
        if (!error)
        {
            size_t read_count = m_file.read(Page::sm_size, (char*)loaded_page.data.data(), error);
        }
    }
    else
    {
        // TODO
        Fail(error, PhysicalStorageErrorCategory::Value::generic_error);
    }
    return loaded_page;
}

Page PageFile::allocatePage(Ishiko::Error& error)
{
    Page new_page{m_pageCount};
    m_file.resize((m_pageCount + 1) * Page::sm_size);
    ++m_pageCount;
    return new_page;
}

void PageFile::store(const Page& page, Ishiko::Error& error)
{
    m_file.setFilePointer(page.number * Page::sm_size);
    if (!error)
    {
        m_file.write((const char*)page.data.data(), Page::sm_size, error);
    }
}

void PageFile::replace()
{
}

void PageFile::erase()
{
}
