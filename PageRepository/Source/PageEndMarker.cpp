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

#include "PageEndMarker.h"
#include <cstring>
#include <cstdint>

namespace DiplodocusDB
{

PageEndMarker::PageEndMarker()
    : m_nextPage(0)
{
}

PageEndMarker::~PageEndMarker()
{
}

size_t PageEndMarker::nextPage() const
{
    return m_nextPage;
}

void PageEndMarker::setNextPage(size_t nextPage)
{
    m_nextPage = nextPage;
}

void PageEndMarker::write(char* buffer) const
{
    memcpy(buffer, "\xF1\x06\x00\x00\x00\x00\x00\x00", 8);
    *((uint32_t*)(buffer + 2)) = m_nextPage;
}

}
