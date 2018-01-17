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

#ifndef _DIPLODOCUSDB_PHYSICALSTORAGE_PAGEREPOSITORY_MASTERFILE_H_
#define _DIPLODOCUSDB_PHYSICALSTORAGE_PAGEREPOSITORY_MASTERFILE_H_

#include "MasterFileMetadata.h"
#include "EmbeddedTreeDBNodeImpl.h"
#include "PageFileRepository.h"
#include "DiplodocusDB/TreeDB/Core/TreeDBKey.h"
#include "Ishiko/Errors/Error.h"
#include <boost/filesystem/path.hpp>
#include <fstream>
#include <memory>

namespace DiplodocusDB
{

class MasterFile
{
public:
    MasterFile();
    ~MasterFile();

    void create(const boost::filesystem::path& path, Ishiko::Error& error);
    void open(const boost::filesystem::path& path, Ishiko::Error& error);
    void close();

    bool findNode(const TreeDBKey& key, EmbeddedTreeDBNodeImpl& node, Ishiko::Error& error);
    void commitNode(const EmbeddedTreeDBNodeImpl& node, Ishiko::Error& error);

private:
    std::string readString(size_t& offset, Ishiko::Error& error);
    bool readValue(size_t& offset, std::string& value, Ishiko::Error& error);

private:
    PageFileRepository m_repository;
    std::shared_ptr<MasterFileMetadata> m_metadata;
};

}

#endif
