/*
    Copyright (c) 2018-2019 Xavier Leclercq

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
*/

#include "PageFileRepository.h"
#include <boost/filesystem/operations.hpp>

namespace DiplodocusDB
{

PageFileRepository::PageFileRepository()
    : m_pageCount(0), m_pageCache(*this)
{
}

void PageFileRepository::create(const boost::filesystem::path& path,
                                Ishiko::Error& error)
{
    std::fstream file(path.c_str(), std::fstream::out | std::fstream::binary);
    if (!file.good())
    {
        // TODO add details
        error.fail(-1, "Failed to create file", __FILE__, __LINE__);
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
        error.fail(-1, "Failed to get file size", __FILE__, __LINE__);
    }
    else
    {
        m_file.open(path.c_str(), std::fstream::in | std::fstream::out | std::fstream::binary);
        if (!m_file.good())
        {
            // TODO add details
            error.fail(-1, "Failed to open file", __FILE__, __LINE__);
        }
        m_pageCount = (filesize / Page::sm_size);
    }
}

void PageFileRepository::close()
{
    m_file.close();
}

void PageFileRepository::load(Page& page, Ishiko::Error& error)
{
    page.read(m_file, error);
}

void PageFileRepository::save(const Page& page, Ishiko::Error& error)
{
    page.write(m_file, error);
}

size_t PageFileRepository::pageCount()
{
    return m_pageCount;
}

std::shared_ptr<Page> PageFileRepository::allocatePage(Ishiko::Error& error)
{
    std::shared_ptr<Page> page = m_pageCache.allocatePage(m_pageCount, error);
    if (!error)
    {
        ++m_pageCount;
    }
    return page;
}

std::shared_ptr<Page> PageFileRepository::insertPageAfter(Page& page, Ishiko::Error& error)
{
    std::shared_ptr<Page> newPage = allocatePage(error);
    if (!error)
    {
        newPage->init();
        newPage->setNextPage(page.nextPage());
        page.setNextPage(newPage->index());
    }
    return newPage;
}

std::shared_ptr<Page> PageFileRepository::page(size_t i,
                                               Ishiko::Error& error)
{
    return m_pageCache.page(i, error);
}

PageRepositoryReader PageFileRepository::read(size_t startPage,
                                              size_t offset,
                                              Ishiko::Error& error)
{
    std::shared_ptr<Page> p = page(startPage, error);
    return read(p, offset, error);
}

PageRepositoryReader PageFileRepository::read(std::shared_ptr<Page> startPage,
                                              size_t offset,
                                              Ishiko::Error& error)
{
    return PageRepositoryReader(*this, startPage, offset);
}

PageRepositoryWriter PageFileRepository::insert(size_t startPage,
                                                size_t offset,
                                                Ishiko::Error& error)
{
    std::shared_ptr<Page> p = page(startPage, error);
    return insert(p, offset, error);
}

PageRepositoryWriter PageFileRepository::insert(std::shared_ptr<Page> startPage,
                                                size_t offset,
                                                Ishiko::Error& error)
{
    return PageRepositoryWriter(*this, startPage, offset);
}

void PageFileRepository::replace()
{
}

void PageFileRepository::erase()
{
}

std::fstream& PageFileRepository::file()
{
    return m_file;
}

}
