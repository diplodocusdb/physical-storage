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

#include "StartOfPageRecordData.h"
#include "Page.h"

namespace DiplodocusDB
{

StartOfPageRecordData::StartOfPageRecordData()
    : RecordData(Record::ERecordType::eStartOfPage),
    m_previousPage(0)
{
}

StartOfPageRecordData::~StartOfPageRecordData()
{
}

void StartOfPageRecordData::setSize(size_t size)
{
    m_size = size;
}

size_t StartOfPageRecordData::size() const
{
    return 8;
}

void StartOfPageRecordData::read(const char* buffer,
                                 size_t recordDataSize)
{
}

void StartOfPageRecordData::write(Page& page,
                                  std::set<size_t>& updatedPages,
                                  Ishiko::Error& error) const
{
    page.write((char*)&m_previousPage, 4, updatedPages, error);
    if (!error)
    {
        page.write((char*)&m_size, 4, updatedPages, error);
    }
}

}
