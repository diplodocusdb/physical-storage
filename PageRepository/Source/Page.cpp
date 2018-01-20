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

void Page::read(char* buffer,
                size_t pos,
                size_t n,
                Ishiko::Error& error) const
{
    memcpy(buffer, m_buffer + m_startMarker.size() + pos, n);
}

Page* Page::write(const char* buffer,
                  size_t bufferSize,
                  std::set<size_t>& updatedPages,
                  Ishiko::Error& error)
{
    if (bufferSize <= m_availableSpace)
    {
        memcpy(m_buffer + m_startMarker.size() + m_dataSize, buffer, bufferSize);
        updatedPages.insert(m_index);
        m_dataSize += bufferSize;
        m_availableSpace -= bufferSize;
        return this;
    }
    else
    {
        Page* nextPage = m_file.allocatePage(error);
        if (m_availableSpace > 0)
        {
            memcpy(m_buffer + m_startMarker.size() + m_dataSize, buffer, m_availableSpace);
            updatedPages.insert(m_index);
            m_dataSize += m_availableSpace;
            m_availableSpace = 0;
        }
        return nextPage->write((buffer + m_availableSpace), (bufferSize - m_availableSpace), updatedPages, error);
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

    m_dataSize = *((uint32_t*)(m_buffer + 6));
}

}
