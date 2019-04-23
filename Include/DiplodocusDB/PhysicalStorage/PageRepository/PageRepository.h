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

#ifndef _DIPLODOCUSDB_PHYSICALSTORAGE_PAGEREPOSITORY_PAGEREPOSITORY_H_
#define _DIPLODOCUSDB_PHYSICALSTORAGE_PAGEREPOSITORY_PAGEREPOSITORY_H_

#include "Page.h"
#include "PageRepositoryPosition.h"
#include "PageRepositoryReader.h"
#include "PageRepositoryWriter.h"
#include "Ishiko/Errors/Error.h"
#include <memory>

namespace DiplodocusDB
{

/// Abstract base class for repositories that store a series of consecutive pages of fixed size.
class PageRepository
{
public:
    /// Destructor.
    virtual ~PageRepository() noexcept = default;

    virtual size_t pageCount() = 0;
    virtual std::shared_ptr<Page> page(size_t index, Ishiko::Error& error) = 0;
    virtual std::shared_ptr<Page> allocatePage(Ishiko::Error& error) = 0;
    virtual std::shared_ptr<Page> insertPageAfter(Page& page, Ishiko::Error& error) = 0;
    virtual void save(const Page& page, Ishiko::Error& error) = 0;

    PageRepositoryReader read(const PageRepositoryPosition& pos, Ishiko::Error& error);
    virtual PageRepositoryReader read(size_t startPage, size_t offset, Ishiko::Error& error) = 0;
    virtual PageRepositoryReader read(std::shared_ptr<Page> startPage, size_t offset, Ishiko::Error& error) = 0;

    PageRepositoryWriter insert(const PageRepositoryPosition& pos, Ishiko::Error& error);
    virtual PageRepositoryWriter insert(size_t startPage, size_t offset, Ishiko::Error& error) = 0;
    virtual PageRepositoryWriter insert(std::shared_ptr<Page> startPage, size_t offset, Ishiko::Error& error) = 0;
    virtual void replace() = 0;
    virtual void erase() = 0;
};

}

#include "linkoptions.h"

#endif
