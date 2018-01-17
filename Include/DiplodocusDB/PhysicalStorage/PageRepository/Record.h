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

#ifndef _DIPLODOCUSDB_PHYSICALSTORAGE_PAGEREPOSITORY_RECORD_H_
#define _DIPLODOCUSDB_PHYSICALSTORAGE_PAGEREPOSITORY_RECORD_H_

#include "Ishiko/Errors/Error.h"
#include <set>
#include <memory>

namespace DiplodocusDB
{

class Page;
class RecordData;

class Record
{
public:
    enum class ERecordType
    {
        eInvalid = 0,
        eMasterFileMetadata = 0x01,
        eKey = 0x02,
        eValue = 0x03,
        eChildren = 0x04,
        eStartOfPage = 0xF0,
        eEndOfPage = 0xF1,
    };

    Record();
    Record(std::shared_ptr<RecordData> data);
    ~Record();

    ERecordType type() const;
    size_t size() const;
    RecordData* data();

    void read(const char* buffer, Ishiko::Error& error);
    void write(Page& page, std::set<size_t>& updatedPages, Ishiko::Error& error) const;

private:
    std::shared_ptr<RecordData> m_data;
};

}

#endif
