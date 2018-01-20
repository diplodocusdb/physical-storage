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

#include "PageStartMarker.h"
#include <cstring>
#include <cstdint>

namespace DiplodocusDB
{

PageStartMarker::PageStartMarker()
    : m_previousPage(0), m_dataSize(0)
{
}

PageStartMarker::~PageStartMarker()
{
}

size_t PageStartMarker::size() const
{
    return 8;
}

void PageStartMarker::setDataSize(size_t size)
{
    m_dataSize = size;
}

void PageStartMarker::write(char* buffer)
{
    memcpy(buffer, "\xF0\x06\x00\x00\x00\x00", 6);
    *((uint16_t*)(buffer + 6)) = m_dataSize;
}

}
