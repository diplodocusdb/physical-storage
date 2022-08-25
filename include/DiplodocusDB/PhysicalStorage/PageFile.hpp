/*
    Copyright (c) 2018-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/diplodocusdb/physical-storage/blob/main/LICENSE.txt
*/

#ifndef GUARD_DIPLODOCUSDB_PHYSICALSTORAGE_PAGEFILE_HPP
#define GUARD_DIPLODOCUSDB_PHYSICALSTORAGE_PAGEFILE_HPP

#include "PageRepository.hpp"
#include "Page.hpp"
#include <boost/filesystem/path.hpp>
#include <Ishiko/Errors.hpp>
#include <Ishiko/FileSystem.hpp>
#include <memory>

namespace DiplodocusDB
{
namespace PhysicalStorage
{

/// A file organized in a series of pages of fixed size.
class PageFile : public PageRepository
{
public:
    PageFile();

    void create(const boost::filesystem::path& path, Ishiko::Error& error);
    void open(const boost::filesystem::path& path, Ishiko::Error& error);
    void close();

    size_t pageCount() override;
    Page allocatePage(Ishiko::Error& error) override;
    Page load(size_t page_number, Ishiko::Error& error) override;
    void store(const Page& page, Ishiko::Error& error) override;

    void replace() override;
    void erase() override;

private:
    Ishiko::BinaryFile m_file;
    size_t m_pageCount;
};

}
}

#endif
