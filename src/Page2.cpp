/*
    Copyright (c) 2018-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/diplodocusdb/physical-storage/blob/main/LICENSE.txt
*/

#include "Page2.hpp"
#include "PageFileRepository.h"
#include "PhysicalStorageErrorCategory.hpp"
#include <Ishiko/Errors.hpp>
#include <Ishiko/IO.hpp>
#include <sstream>

using namespace DiplodocusDB::PhysicalStorage;

Page2::Page2(size_t number)
    : m_page(number), m_dataSize(0), m_availableSpace(Page::sm_size - sm_startMarkerSize - sm_endMarkerSize),
    m_nextPage(0)
{
}

void Page2::init()
{
    m_page.init();
}

size_t Page2::number() const
{
    return m_page.number();
}

size_t Page2::dataSize() const
{
    return m_dataSize;
}

size_t Page2::maxDataSize() const
{
    return (Page::sm_size - sm_startMarkerSize - sm_endMarkerSize);
}

size_t Page2::availableSpace() const
{
    return m_availableSpace;
}

size_t Page2::nextPage() const
{
    return m_nextPage;
}

void Page2::setNextPage(size_t index)
{
    m_nextPage = index;
}

void Page2::get(char* buffer, size_t pos, size_t n, Ishiko::Error& error) const
{
    if ((pos + n) <= m_dataSize)
    {
        memcpy(buffer, m_page.buffer().data() + sm_startMarkerSize + pos, n);
    }
    else
    {
        std::stringstream message;
        message << "Page::get (m_index: " << m_page.number() << ", pos:" << pos << ", n:" << n
            << ") exceeds data size (m_datasize: " << m_dataSize << ")";
        Fail(error, PhysicalStorageErrorCategory::Value::generic_error, message.str(), __FILE__, __LINE__);
    }
}

void Page2::insert(const char* buffer, size_t bufferSize, size_t pos, Ishiko::Error& error)
{
    if (bufferSize <= m_availableSpace)
    {
        char* p = (char*)(m_page.buffer().data() + sm_startMarkerSize);
        if (pos != m_dataSize)
        {
            memmove(p + pos + bufferSize, p + pos, (m_dataSize - pos));
        }
        memcpy(p + pos, buffer, bufferSize);
        m_dataSize += bufferSize;
        m_availableSpace -= bufferSize;
    }
    else
    {
        // TODO : add page details
        Fail(error, PhysicalStorageErrorCategory::Value::generic_error, "Failed to insert page", __FILE__, __LINE__);
    }
}

void Page2::erase(size_t pos, size_t n, Ishiko::Error& error)
{
    memmove(m_page.buffer().data() + sm_startMarkerSize + pos, m_page.buffer().data() + sm_startMarkerSize + pos + n,
        m_dataSize + sm_endMarkerSize - pos - n);
    memset(m_page.buffer().data() + sm_startMarkerSize + m_dataSize + sm_endMarkerSize - n, 0, n);
    m_dataSize -= n;
    m_availableSpace += n;
}

void Page2::moveTo(size_t pos, size_t n, Page2& targetPage, Ishiko::Error& error)
{
    targetPage.insert((const char*)(m_page.buffer().data() + sm_startMarkerSize + pos), n, 0, error);
    if (!error)
    {
        erase(pos, n, error);
    }
}

void Page2::write(PageFileRepository& repository, Ishiko::Error& error) const
{
    repository.m_file.setFilePointer(m_page.number() * Page::sm_size);
    if (!error)
    {
        memcpy(m_page.buffer().data(), "\xF0\x06\x00\x00\x00\x00", 6);
        *((uint16_t*)(m_page.buffer().data() + 6)) = (uint16_t)m_dataSize;
        memcpy(m_page.buffer().data() + sm_startMarkerSize + m_dataSize, "\xF1\x06\x00\x00\x00\x00\x00\x00", 8);
        *((uint32_t*)(m_page.buffer().data() + sm_startMarkerSize + m_dataSize + 2)) = m_nextPage;
        
        repository.m_file.write((const char*)m_page.buffer().data(), Page::sm_size, error);
    }
}

void Page2::read(PageFileRepository& repository, Ishiko::Error& error)
{
    repository.m_file.setFilePointer(m_page.number() * Page::sm_size);
    if (!error)
    {
        size_t read_count = repository.m_file.read(Page::sm_size, (char*)m_page.buffer().data(), error);
        if (!error)
        {
            if (read_count == Page::sm_size)
            {
                m_dataSize = *((uint16_t*)(m_page.buffer().data() + 6));
                m_availableSpace = Page::sm_size - sm_startMarkerSize - sm_endMarkerSize - m_dataSize;

                uint32_t nextPage = *((uint32_t*)(m_page.buffer().data() + sm_startMarkerSize + m_dataSize + 2));
                m_nextPage = nextPage;
            }
            else
            {
                // TODO
                Fail(error, PhysicalStorageErrorCategory::Value::generic_error, "", __FILE__, __LINE__);
            }
        }
    }
}
