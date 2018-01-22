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

#include "PageRepositoryWriter.h"

namespace DiplodocusDB
{

PageRepositoryWriter::PageRepositoryWriter(std::shared_ptr<Page> startPage,
                                           size_t startOffset)
    : m_currentPage(startPage), m_currentOffset(startOffset)
{
}

PageRepositoryWriter::~PageRepositoryWriter()
{
}

void PageRepositoryWriter::write(const char* buffer,
                                 size_t bufferSize,
                                 Ishiko::Error& error)
{
    size_t availableSpace = m_currentPage->availableSpace();
    if (bufferSize > availableSpace)
    {
        if (availableSpace > 0)
        {
            m_currentPage->insert(buffer, availableSpace, m_currentOffset, error);
            if (error)
            {
                return;
            }
            buffer += availableSpace;
            bufferSize -= availableSpace;
        }
        std::shared_ptr<Page> newPage = m_currentPage->insertNextPage(error);
        if (error)
        {
            return;
        }
        m_previousPages.push_back(m_currentPage);
        m_currentPage = newPage;
        m_currentOffset = 0;
    }
    m_currentPage->insert(buffer, bufferSize, m_currentOffset, error);
    if (!error)
    {
        m_currentOffset += bufferSize;
    }
}

void PageRepositoryWriter::save(Ishiko::Error& error)
{
    for (std::shared_ptr<Page>& page : m_previousPages)
    {
        page->save(error);
    }
    m_currentPage->save(error);
}

}
