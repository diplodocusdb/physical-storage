/*
    Copyright (c) 2018 Xavier Leclercq

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

PageFileRepository::~PageFileRepository()
{
}

void PageFileRepository::create(const boost::filesystem::path& path,
                                Ishiko::Error& error)
{
    std::fstream file(path.c_str(), std::fstream::out | std::fstream::binary);
    if (!file.good())
    {
        error = -1;
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
        error = -1;
    }
    else
    {
        m_file.open(path.c_str(), std::fstream::in | std::fstream::out | std::fstream::binary);
        if (!m_file.good())
        {
            error = -1;
        }
        m_pageCount = (filesize / Page::sm_size);
    }
}

void PageFileRepository::close()
{
    m_file.close();
}

Page* PageFileRepository::allocatePage(Ishiko::Error& error)
{
    Page* page = m_pageCache.allocatePage(m_pageCount, error);
    if (!error)
    {
        ++m_pageCount;
    }
    return page;
}

Page* PageFileRepository::page(size_t i,
                               Ishiko::Error& error)
{
    return m_pageCache.page(i, error);
}

std::fstream& PageFileRepository::file()
{
    return m_file;
}

}
