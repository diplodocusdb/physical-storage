/*
    Copyright (c) 2018-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/diplodocusdb/physical-storage/blob/main/LICENSE.txt
*/

#include "Page.h"
#include "PageFileRepository.h"
#include "PageRepositoryErrorCategory.hpp"
#include <Ishiko/Errors.hpp>
#include <Ishiko/IO.hpp>
#include <sstream>

using namespace DiplodocusDB;

Page::Page(size_t index)
    : m_index(index), m_dataSize(0), m_availableSpace(sm_size - sm_startMarkerSize - sm_endMarkerSize), m_nextPage(0)
{
}

void Page::init()
{
    memset(m_buffer, 0, sm_size);
}

size_t Page::index() const
{
    return m_index;
}

size_t Page::dataSize() const
{
    return m_dataSize;
}

size_t Page::maxDataSize() const
{
    return (sm_size - sm_startMarkerSize - sm_endMarkerSize);
}

size_t Page::availableSpace() const
{
    return m_availableSpace;
}

size_t Page::nextPage() const
{
    return m_nextPage;
}

void Page::setNextPage(size_t index)
{
    m_nextPage = index;
}

void Page::get(char* buffer,
               size_t pos,
               size_t n,
               Ishiko::Error& error) const
{
    if ((pos + n) <= m_dataSize)
    {
        memcpy(buffer, m_buffer + sm_startMarkerSize + pos, n);
    }
    else
    {
        std::stringstream message;
        message << "Page::get (m_index: " << m_index << ", pos:" << pos << ", n:" << n
            << ") exceeds data size (m_datasize: " << m_dataSize << ")";
        Fail(error, PageRepositoryErrorCategory::eGeneric, message.str(), __FILE__, __LINE__);
    }
}

void Page::insert(const char* buffer, size_t bufferSize, size_t pos, Ishiko::Error& error)
{
    if (bufferSize <= m_availableSpace)
    {
        char* p = (m_buffer + sm_startMarkerSize);
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
        Fail(error, PageRepositoryErrorCategory::eGeneric, "Failed to insert page", __FILE__, __LINE__);
    }
}

void Page::erase(size_t pos, size_t n, Ishiko::Error& error)
{
    memmove(m_buffer + sm_startMarkerSize + pos, m_buffer + sm_startMarkerSize + pos + n,
        m_dataSize + sm_endMarkerSize - pos - n);
    memset(m_buffer + sm_startMarkerSize + m_dataSize + sm_endMarkerSize - n, 0, n);
    m_dataSize -= n;
    m_availableSpace += n;
}

void Page::moveTo(size_t pos, size_t n, Page& targetPage, Ishiko::Error& error)
{
    targetPage.insert(m_buffer + sm_startMarkerSize + pos, n, 0, error);
    if (!error)
    {
        erase(pos, n, error);
    }
}

void Page::write(std::ostream& output, Ishiko::Error& error) const
{
    output.seekp(m_index * sm_size);
    Ishiko::IOErrorExtension::Fail(error, output, __FILE__, __LINE__);
    if (!error)
    {
        memcpy(m_buffer, "\xF0\x06\x00\x00\x00\x00", 6);
        *((uint16_t*)(m_buffer + 6)) = (uint16_t)m_dataSize;
        memcpy(m_buffer + sm_startMarkerSize + m_dataSize, "\xF1\x06\x00\x00\x00\x00\x00\x00", 8);
        *((uint32_t*)(m_buffer + sm_startMarkerSize + m_dataSize + 2)) = m_nextPage;
        
        output.write(m_buffer, sm_size);
        Ishiko::IOErrorExtension::Fail(error, output, __FILE__, __LINE__);
    }
}

void Page::read(std::istream& input, Ishiko::Error& error)
{
    input.seekg(m_index * sm_size);
    Ishiko::IOErrorExtension::Fail(error, input, __FILE__, __LINE__);
    if (!error)
    {
        input.read(m_buffer, sm_size);
        Ishiko::IOErrorExtension::Fail(error, input, __FILE__, __LINE__);
        if (!error)
        {
            m_dataSize = *((uint16_t*)(m_buffer + 6));
            m_availableSpace = sm_size - sm_startMarkerSize - sm_endMarkerSize - m_dataSize;

            uint32_t nextPage = *((uint32_t*)(m_buffer + sm_startMarkerSize + m_dataSize + 2));
            m_nextPage = nextPage;
        }
    }
}
