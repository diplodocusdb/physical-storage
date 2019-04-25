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

#include "Page.h"
#include "PageFileRepository.h"
#include <sstream>

namespace DiplodocusDB
{

Page::Page(size_t index)
    : m_index(index), m_dataSize(0), m_availableSpace(sm_size - m_startMarker.size() - m_endMarker.size())
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
    return (sm_size - m_startMarker.size() - m_endMarker.size());
}

size_t Page::availableSpace() const
{
    return m_availableSpace;
}

size_t Page::nextPage() const
{
    return m_endMarker.nextPage();
}

void Page::setNextPage(size_t index)
{
    m_endMarker.setNextPage(index);
}

void Page::get(char* buffer,
               size_t pos,
               size_t n,
               Ishiko::Error& error) const
{
    if ((pos + n) <= m_dataSize)
    {
        memcpy(buffer, m_buffer + m_startMarker.size() + pos, n);
    }
    else
    {
        std::stringstream message;
        message << "Page::get (m_index: " << m_index << ", pos:" << pos << ", n:" << n
            << ") exceeds data size (m_datasize: " << m_dataSize << ")";
        error.fail(-1, message.str(), __FILE__, __LINE__);
    }
}

void Page::insert(const char* buffer, size_t bufferSize, size_t pos, Ishiko::Error& error)
{
    if (bufferSize <= m_availableSpace)
    {
        char* p = (m_buffer + m_startMarker.size());
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
        error.fail(-1, "Failed to insert page", __FILE__, __LINE__);
    }
}

void Page::erase(size_t pos, size_t n, Ishiko::Error& error)
{
    memmove(m_buffer + m_startMarker.size() + pos, m_buffer + m_startMarker.size() + pos + n, m_dataSize + m_endMarker.size() - pos - n);
    memset(m_buffer + m_startMarker.size() + m_dataSize + m_endMarker.size() - n, 0, n);
    m_dataSize -= n;
    m_availableSpace += n;
}

void Page::moveTo(size_t pos, size_t n, Page& targetPage, Ishiko::Error& error)
{
    targetPage.insert(m_buffer + m_startMarker.size() + pos, n, 0, error);
    if (!error)
    {
        erase(pos, n, error);
    }
}

void Page::write(std::ostream& output, Ishiko::Error& error) const
{
    output.seekp(m_index * sm_size);
    if (!output.good())
    {
        // TODO add details
        error.fail(-1, "Failed to save page", __FILE__, __LINE__);
        return;
    }

    m_startMarker.setDataSize(m_dataSize);
    m_startMarker.write(m_buffer);
    m_endMarker.write(m_buffer + m_startMarker.size() + m_dataSize);
    
    output.write(m_buffer, sm_size);
    if (!output.good())
    {
        // TODO add details
        error.fail(-1, "Failed to save page", __FILE__, __LINE__);
        return;
    }
}

void Page::read(std::istream& input, Ishiko::Error& error)
{
    input.seekg(m_index * sm_size);
    if (!input.good())
    {
        // TODO add details
        error.fail(-1, "Failed to load page", __FILE__, __LINE__);
        return;
    }

    input.read(m_buffer, sm_size);
    if (!input.good())
    {
        // TODO add details
        error.fail(-1, "Failed to load page", __FILE__, __LINE__);
        return;
    }

    m_dataSize = *((uint16_t*)(m_buffer + 6));
    m_availableSpace = sm_size - m_startMarker.size() - m_endMarker.size() - m_dataSize;

    uint32_t nextPage = *((uint32_t*)(m_buffer + m_startMarker.size() + m_dataSize + 2));
    m_endMarker.setNextPage(nextPage);
}

}
