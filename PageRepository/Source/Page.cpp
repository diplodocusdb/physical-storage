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

#include "Page.h"
#include "PageFileRepository.h"

namespace DiplodocusDB
{

Page::Page(PageFileRepository& file,
           size_t index)
    : m_file(file), m_index(index), m_dataSize(0),
    m_availableSpace(sm_size - m_startMarker.size() - m_endMarker.size())
{
}

Page::~Page()
{
}

size_t Page::index() const
{
    return m_index;
}

size_t Page::dataSize() const
{
    return m_dataSize;
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
        error = -1;
    }
}

void Page::insert(const char* buffer,
                  size_t bufferSize,
                  size_t pos,
                  Ishiko::Error& error)
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
        error = -1;
    }
}

void Page::save(Ishiko::Error& error)
{
    std::fstream& file = m_file.file();

    file.seekp(m_index * sm_size);
    if (!file.good())
    {
        error = -1;
        return;
    }

    m_startMarker.setDataSize(m_dataSize);
    m_startMarker.write(m_buffer);
    m_endMarker.write(m_buffer + m_startMarker.size() + m_dataSize);
    
    file.write(m_buffer, sm_size);
    if (!file.good())
    {
        error = -1;
        return;
    }
}

void Page::init()
{
    memset(m_buffer, 0, sm_size);
}

void Page::load(Ishiko::Error& error)
{
    std::fstream& file = m_file.file();

    file.seekg(m_index * sm_size);
    if (!file.good())
    {
        error = -1;
        return;
    }

    file.read(m_buffer, sm_size);
    if (!file.good())
    {
        error = -1;
        return;
    }

    m_dataSize = *((uint16_t*)(m_buffer + 6));
}

}
