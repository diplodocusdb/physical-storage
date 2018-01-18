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

#ifndef _DIPLODOCUSDB_TREEDB_EMBEDDEDTREEDB_ENDOFPAGERECORDDATA_H_
#define _DIPLODOCUSDB_TREEDB_EMBEDDEDTREEDB_ENDOFPAGERECORDDATA_H_

#include "RecordData.h"

namespace DiplodocusDB
{

class EndOfPageRecordData : public RecordData
{
public:
    EndOfPageRecordData();
    ~EndOfPageRecordData() override;

    size_t size() const override;
    void read(const char* buffer, size_t recordDataSize) override;
    void write(Page& page, std::set<size_t>& updatedPages, Ishiko::Error& error) const override;

private:
    size_t m_nextPage;
};

}

#endif
