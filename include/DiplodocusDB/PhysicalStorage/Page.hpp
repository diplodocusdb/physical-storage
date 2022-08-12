/*
    Copyright (c) 2018-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/diplodocusdb/physical-storage/blob/main/LICENSE.txt
*/

#ifndef GUARD_DIPLODOCUSDB_PHYSICALSTORAGE_PAGE_HPP
#define GUARD_DIPLODOCUSDB_PHYSICALSTORAGE_PAGE_HPP

#include "Ishiko/Errors.hpp"
#include <fstream>

namespace DiplodocusDB
{

/// A page.
/**
    Each page has an index which is its position in the repository.
*/
class Page
{
public:
    /// Constructor.
    /**
        Note that the page contents are not initialized by this constructor. Use the init() function to initialize the
        page to all zeroes.

        @param index The index of the page.
    */
    Page(size_t index);
    /// Fills the contents of the page with zeroes.
    void init();

    
    /// Returns the index of the page.
    /**
        @returns The index of the page.
    */
    size_t index() const;
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
    void moveTo(size_t pos, size_t n, Page& targetPage, Ishiko::Error& error);

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
    void read(std::istream& input, Ishiko::Error& error);

public:
    static const size_t sm_size = 4096;
    static const size_t sm_startMarkerSize = 8;
    static const size_t sm_endMarkerSize = 8;

private:
    size_t m_index;
    // TODO: avoid this mutable thing
    mutable char m_buffer[sm_size];
    size_t m_dataSize;
    size_t m_availableSpace;
    size_t m_nextPage;
};

}

#endif
