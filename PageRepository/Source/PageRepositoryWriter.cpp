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
#include "PageRepository.h"

namespace DiplodocusDB
{

PageRepositoryWriter::PageRepositoryWriter(PageRepository& repository,
                                           std::shared_ptr<Page> startPage,
                                           size_t startOffset)
    : m_repository(repository), m_currentPage(startPage),
    m_currentOffset(startOffset)
{
}

PageRepositoryWriter::~PageRepositoryWriter()
{
}

void PageRepositoryWriter::write(const char* buffer,
                                 size_t bufferSize,
                                 Ishiko::Error& error)
{
    if ((m_currentOffset + bufferSize) <= m_currentPage->maxDataSize())
    {
        // The new data can fit in the current page. We may have to move
        // existing data though.
        size_t availableSpace = m_currentPage->availableSpace();
        if (bufferSize <= availableSpace)
        {
            // The new data can fit in the current page without moving any
            // data.
            m_currentPage->insert(buffer, bufferSize, m_currentOffset, error);
            if (!error)
            {
                m_currentOffset += bufferSize;
            }
            m_updatedPages.insert(m_currentPage);
        }
        else
        {
            // We need to move some data to a new page
            size_t nextPageIndex = m_currentPage->nextPage();
            std::shared_ptr<Page> newPage;
            if (nextPageIndex != 0)
            {
                newPage = m_repository.page(nextPageIndex, error);
            }
            else
            {
                newPage = m_currentPage->insertNextPage(error);
            }
            if (error)
            {
                return;
            }
            size_t spaceNeeded = (bufferSize - availableSpace);
            m_currentPage->moveTo(m_currentPage->dataSize() - spaceNeeded, spaceNeeded, *newPage, error);
            if (error)
            {
                return;
            }
            m_currentPage->insert(buffer, bufferSize, m_currentOffset, error);
            if (!error)
            {
                m_currentOffset += bufferSize;
            }
            m_updatedPages.insert(m_currentPage);
            m_updatedPages.insert(newPage);
        }
    }
    else
    {
        // Only part of the new data can fit in the current page. We have to
        // move any existing data to the next page.
        size_t nextPageIndex = m_currentPage->nextPage();
        std::shared_ptr<Page> newPage;
        if (nextPageIndex != 0)
        {
            newPage = m_repository.page(nextPageIndex, error);
        }
        else
        {
            newPage = m_currentPage->insertNextPage(error);
        }
        if (error)
        {
            return;
        }
        size_t spaceInCurrentPage = (m_currentPage->maxDataSize() - m_currentOffset);
        if (!error)
        {
            m_currentPage->moveTo(m_currentOffset, m_currentPage->dataSize() - m_currentOffset, *newPage, error);
            if (!error)
            {
                write(buffer, m_currentPage->availableSpace(), error);
                if (!error)
                {
                    m_currentPage = newPage;
                    m_currentOffset = 0;
                    m_updatedPages.insert(m_currentPage);
                    write(buffer + spaceInCurrentPage, bufferSize - spaceInCurrentPage, error);
                }
            }
        }
    }
}

void PageRepositoryWriter::save(Ishiko::Error& error)
{
    for (const std::shared_ptr<Page>& page : m_updatedPages)
    {
        page->save(error);
    }
}

}
