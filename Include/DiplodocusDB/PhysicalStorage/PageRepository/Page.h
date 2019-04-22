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

#ifndef _DIPLODOCUSDB_PHYSICALSTORAGE_PAGEREPOSITORY_PAGE_H_
#define _DIPLODOCUSDB_PHYSICALSTORAGE_PAGEREPOSITORY_PAGE_H_

#include "PageStartMarker.h"
#include "PageEndMarker.h"
#include "Ishiko/Errors/Error.h"
#include <fstream>
#include <set>
#include <memory>

namespace DiplodocusDB
{

class PageFileRepository;

class Page
{
public:
    Page(PageFileRepository& file, size_t index);
    ~Page();

    size_t index() const;
    size_t dataSize() const;
    size_t maxDataSize() const;
    size_t availableSpace() const;
    size_t nextPage() const;
    void setNextPage(size_t index);

    void get(char* buffer, size_t pos, size_t n, Ishiko::Error& error) const;
    void insert(const char* buffer, size_t bufferSize, size_t pos, Ishiko::Error& error);
    void erase(size_t pos, size_t n, Ishiko::Error& error);
    void moveTo(size_t pos, size_t n, Page& targetPage, Ishiko::Error& error);

    void write(std::ostream& output, Ishiko::Error& error) const;
    void init();
    void read(std::istream& input, Ishiko::Error& error);

public:
    static const size_t sm_size = 4096;

private:
    PageFileRepository& m_file;
    size_t m_index;
    // TODO: avoid this mutable thing
    mutable char m_buffer[sm_size];
    size_t m_dataSize;
    size_t m_availableSpace;
    // TODO: avoid this mutable thing
    mutable PageStartMarker m_startMarker;
    PageEndMarker m_endMarker;
};

}

#include "linkoptions.h"

#endif
