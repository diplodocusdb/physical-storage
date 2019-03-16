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

#include "PageRepositoryReaderTests.h"
#include "DiplodocusDB/PhysicalStorage/PageRepository/PageRepositoryReader.h"
#include "DiplodocusDB/PhysicalStorage/PageRepository/PageFileRepository.h"

using namespace Ishiko::Tests;

PageRepositoryReaderTests::PageRepositoryReaderTests(const TestNumber& number, const TestEnvironment& environment)
    : TestSequence(number, "PageRepositoryReader tests", environment)
{
    append<HeapAllocationErrorsTest>("Creation test 1", CreationTest1);
    append<HeapAllocationErrorsTest>("read test 1", ReadTest1);
    append<HeapAllocationErrorsTest>("read test 2", ReadTest2);
    append<HeapAllocationErrorsTest>("read test 3", ReadTest3);
}

void PageRepositoryReaderTests::CreationTest1(Test& test)
{
    boost::filesystem::path inputPath(test.environment().getTestDataDirectory()
        / "PageRepositoryReaderCreationTest1.dpdb");

    Ishiko::Error error(0);

    DiplodocusDB::PageFileRepository repository;
    repository.open(inputPath, error);

    ISHTF_ABORT_IF((bool)error);

    std::shared_ptr<DiplodocusDB::Page> page = repository.page(0, error);

    ISHTF_ABORT_IF((bool)error);
    
    DiplodocusDB::PageRepositoryReader reader(page, 0);

    ISHTF_PASS();
}

void PageRepositoryReaderTests::ReadTest1(Test& test)
{
    boost::filesystem::path inputPath(test.environment().getTestDataDirectory()
        / "PageRepositoryReaderReadTest1.dpdb");

    Ishiko::Error error(0);

    DiplodocusDB::PageFileRepository repository;
    repository.open(inputPath, error);
    
    ISHTF_ABORT_IF((bool)error);

    DiplodocusDB::PageRepositoryReader reader = repository.read(0, 0, error);

    ISHTF_ABORT_IF((bool)error);

    char buffer[6];
    reader.read(buffer, 6, error);
    
    ISHTF_FAIL_IF((bool)error);
    ISHTF_FAIL_UNLESS(strncmp(buffer, "value1", 6) == 0);
    ISHTF_PASS();
}

void PageRepositoryReaderTests::ReadTest2(Test& test)
{
    boost::filesystem::path inputPath(test.environment().getTestDataDirectory()
        / "PageRepositoryReaderReadTest2.dpdb");

    Ishiko::Error error(0);

    DiplodocusDB::PageFileRepository repository;
    repository.open(inputPath, error);

    ISHTF_ABORT_IF((bool)error);
    
    DiplodocusDB::PageRepositoryReader reader = repository.read(0, 0, error);

    ISHTF_ABORT_IF((bool)error);
        
    char buffer[6];
    reader.read(buffer, 6, error);

    ISHTF_FAIL_IF((bool)error);
    ISHTF_FAIL_UNLESS(strncmp(buffer, "value1", 6) == 0);
                
    reader.read(buffer, 6, error);

    ISHTF_FAIL_IF((bool)error);
    ISHTF_FAIL_UNLESS(strncmp(buffer, "value2", 6) == 0);
    ISHTF_PASS();
}

void PageRepositoryReaderTests::ReadTest3(Test& test)
{
    boost::filesystem::path inputPath(test.environment().getTestDataDirectory()
        / "PageRepositoryReaderReadTest3.dpdb");

    Ishiko::Error error(0);

    DiplodocusDB::PageFileRepository repository;
    repository.open(inputPath, error);

    ISHTF_ABORT_IF((bool)error);

    DiplodocusDB::PageRepositoryReader reader = repository.read(0, 6, error);

    ISHTF_FAIL_IF((bool)error);
        
    char buffer[6];
    reader.read(buffer, 6, error);

    ISHTF_FAIL_IF((bool)error);
    ISHTF_FAIL_UNLESS(strncmp(buffer, "value2", 6) == 0);
    ISHTF_PASS();
}
