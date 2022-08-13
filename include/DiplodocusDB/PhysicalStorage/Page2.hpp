/*
    Copyright (c) 2018-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/diplodocusdb/physical-storage/blob/main/LICENSE.txt
*/

#ifndef GUARD_DIPLODOCUSDB_PHYSICALSTORAGE_PAGE2_HPP
#define GUARD_DIPLODOCUSDB_PHYSICALSTORAGE_PAGE2_HPP

#include "Page.hpp"
#include <Ishiko/Errors.hpp>
#include <fstream>

namespace DiplodocusDB
{
namespace PhysicalStorage
{

class PageFileRepository;

/// A page.
/**
    Each page has an index which is its position in the repository.
*/
class Page2
{
public:
    /// Constructor.
    /**
        Note that the page contents are not initialized by this constructor. Use the init() function to initialize the
        page to all zeroes.

        @param index The index of the page.
    */
    Page2(size_t number);
    /// Fills the contents of the page with zeroes.
    void init();

    size_t number() const;
    /// Returns the amount of data stored in the page.
    /**
        @returns The amount of data stored in the page.
    */
    size_t dataSize() const;
    /// Returns the maximum amount of data that can be stored in the page.
    /** 
        This is less than the page size because a page has a page start marker at the beginning and a page end marker
        at the end of the data that occupy some space.

        @returns The maximum amount of data that can be store in a page.
    */
    size_t maxDataSize() const;
    /// Returns the available space in the page.
    /**
        This is equivalent to maxDataSize() - dataSize().

        @returns Yhe available space in the page.
    */
    size_t availableSpace() const;
    size_t nextPage() const;
    void setNextPage(size_t index);

    void get(char* buffer, size_t pos, size_t n, Ishiko::Error& error) const;
    void insert(const char* buffer, size_t bufferSize, size_t pos, Ishiko::Error& error);
    void erase(size_t pos, size_t n, Ishiko::Error& error);
    void moveTo(size_t pos, size_t n, Page2& targetPage, Ishiko::Error& error);

    /// Write the contents of the page to a stream.
    /**
        @param output The stream to which the page will be written.
        @param error The result of the operation.
    */
    void write(std::ostream& output, Ishiko::Error& error) const;
    /// Reads the contents of the page from a stream.
    /**
        The index of the page is used to find the start position of the page data in the stream. After this operation
        completes successfully the stream's position will be the byte past the end of the page.

        The index of the page needs to be set to the correct value before calling this function.

        @param input The stream from which the page will be read.
        @param error The result of the operation.
    */
    void read(PageFileRepository& repository, Ishiko::Error& error);

public:
    static const size_t sm_startMarkerSize = 8;
    static const size_t sm_endMarkerSize = 8;

private:
    Page m_page;
    size_t m_dataSize;
    size_t m_availableSpace;
    size_t m_nextPage;
};

}
}

#endif
