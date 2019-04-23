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

#ifndef _DIPLODOCUSDB_PHYSICALSTORAGE_PAGEREPOSITORY_PAGECACHE_H_
#define _DIPLODOCUSDB_PHYSICALSTORAGE_PAGEREPOSITORY_PAGECACHE_H_

#include "Page.h"
#include "Ishiko/Errors/Error.h"
#include <fstream>
#include <map>
#include <memory>

namespace DiplodocusDB
{

class PageFileRepository;

class PageCache
{
public:
    PageCache(PageFileRepository& file);

   std::shared_ptr<Page> get(size_t index, Ishiko::Error& error);
    void set(std::shared_ptr<Page>& page);

private:
    std::shared_ptr<Page> loadPage(size_t i, Ishiko::Error& error);

private:
    PageFileRepository& m_file;
    // TODO : shared pointer don't work for the cache if I return them to clients as I may remove it from the cache
    // but the client still has a copy and then if I reload the page in the cache I will have 2 instances of Cache 
    // for the same page and they can get out of sync.
    std::map<size_t, std::shared_ptr<Page>> m_pages;
};

}

#include "linkoptions.h"

#endif
