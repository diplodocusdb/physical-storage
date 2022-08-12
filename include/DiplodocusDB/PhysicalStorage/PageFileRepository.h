/*
    Copyright (c) 2018-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/diplodocusdb/physical-storage/blob/main/LICENSE.txt
*/

#ifndef GUARD_DIPLODOCUSDB_PHYSICALSTORAGE_PAGEFILEREPOSITORY_H
#define GUARD_DIPLODOCUSDB_PHYSICALSTORAGE_PAGEFILEREPOSITORY_H

#include "PageRepository.h"
#include "Page2.hpp"
#include "PageCache.h"
#include <boost/filesystem/path.hpp>
#include <Ishiko/Errors.hpp>
#include <fstream>
#include <memory>

namespace DiplodocusDB
{
namespace PhysicalStorage
{

/// A file organized in a series of pages of fixed size.
class PageFileRepository : public PageRepository
{
public:
    PageFileRepository();

    void create(const boost::filesystem::path& path, Ishiko::Error& error);
    void open(const boost::filesystem::path& path, Ishiko::Error& error);
    void close();

    size_t pageCount() override;
    std::shared_ptr<Page2> page(size_t index, Ishiko::Error& error) override;
    std::shared_ptr<Page2> allocatePage(Ishiko::Error& error) override;
    void store(const Page2& page, Ishiko::Error& error) override;

    using PageRepository::read;
    PageRepositoryReader read(size_t startPage, size_t offset, Ishiko::Error& error) override;
    PageRepositoryReader read(std::shared_ptr<Page2> startPage, size_t offset, Ishiko::Error& error) override;

    void replace() override;
    void erase() override;

private:
    std::fstream m_file;
    size_t m_pageCount;
    PageCache m_pageCache;
};

}
}

#endif
