/*
    Copyright (c) 2018-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/diplodocusdb/physical-storage/blob/main/LICENSE.txt
*/

#include "PageRepositoryReader.h"
#include "PageRepository.h"

using namespace DiplodocusDB::PhysicalStorage;

PageRepositoryReader::PageRepositoryReader(PageRepository& repository, std::shared_ptr<Page2> startPage,
    size_t startOffset)
    : m_repository(repository), m_currentPage(startPage), m_currentOffset(startOffset)
{
}

PageRepositoryPosition PageRepositoryReader::currentPosition() const
{
    return PageRepositoryPosition(m_currentPage->number(), m_currentOffset);
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
