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

#include "PageRepositoryReader.h"
#include "PageRepository.h"

namespace DiplodocusDB
{

PageRepositoryReader::PageRepositoryReader(PageRepository& repository, std::shared_ptr<Page> startPage,
    size_t startOffset)
    : m_repository(repository), m_currentPage(startPage), m_currentOffset(startOffset)
{
}

PageRepositoryPosition PageRepositoryReader::currentPosition() const
{
    return PageRepositoryPosition(m_currentPage->index(), m_currentOffset);
}

void PageRepositoryReader::read(char* buffer,
                                size_t n,
                                Ishiko::Error& error)
{
    if ((m_currentOffset + n) <= m_currentPage->maxDataSize())
    {
        m_currentPage->get(buffer, m_currentOffset, n, error);
        if (!error)
        {
            m_currentOffset += n;
        }
    }
    else
    {
        // TODO : only works if the read doesn't cross more than one page boundary
        size_t n1 = (m_currentPage->maxDataSize() - m_currentOffset);
        m_currentPage->get(buffer, m_currentOffset, n1, error);
        if (!error)
        {
            size_t nextPageIndex = m_currentPage->nextPage();
            if (nextPageIndex != 0)
            {
                // TODO : if error this would be modified, use a temp?
                m_currentPage = m_repository.page(nextPageIndex, error);
                if (!error)
                {
                    m_currentOffset = 0;
                    size_t n2 = (n - n1);
                    m_currentPage->get((buffer + n1), m_currentOffset, n2, error);
                    if (!error)
                    {
                        m_currentOffset += n2;
                    }
                }
            }
            else
            {
                // TODO : error
            }
        }
    }
}

size_t PageRepositoryReader::readLEB128(Ishiko::Error& error)
{
    size_t result = 0;
    size_t shift = 0;
    while (true)
    {
        char byte;
        read(&byte, 1, error);
        if (error)
        {
            break;
        }
        result += ((byte & 0x7F) << shift);
        if (!(byte & 0x80))
        {
            break;
        }
        shift += 7;
    }
    return result;
}

}
