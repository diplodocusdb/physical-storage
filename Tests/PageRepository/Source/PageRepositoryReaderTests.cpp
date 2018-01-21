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

#include "PageRepositoryReaderTests.h"
#include "DiplodocusDB/PhysicalStorage/PageRepository/PageRepositoryReader.h"
#include "DiplodocusDB/PhysicalStorage/PageRepository/PageFileRepository.h"

void AddPageRepositoryReaderTests(TestHarness& theTestHarness)
{
    TestSequence& readerTestSequence = theTestHarness.appendTestSequence("PageRepositoryReader tests");

    new HeapAllocationErrorsTest("Creation test 1", PageRepositoryReaderCreationTest1, readerTestSequence);

    new HeapAllocationErrorsTest("read test 1", PageRepositoryReaderReadTest1, readerTestSequence);
    new HeapAllocationErrorsTest("read test 2", PageRepositoryReaderReadTest2, readerTestSequence);
}

TestResult::EOutcome PageRepositoryReaderCreationTest1(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageRepositoryReaderCreationTest1.dpdb");

    Ishiko::Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.open(inputPath, error);
    if (!error)
    {
        std::shared_ptr<DiplodocusDB::Page> page = repository.page(0, error);
        if (!error)
        {
            DiplodocusDB::PageRepositoryReader reader(page);
            result = TestResult::ePassed;
        }
    }

    return result;
}

TestResult::EOutcome PageRepositoryReaderReadTest1(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageRepositoryReaderReadTest1.dpdb");

    Ishiko::Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.open(inputPath, error);
    if (!error)
    {
        DiplodocusDB::PageRepositoryReader reader = repository.read(0, 0, error);
        if (!error)
        {
            char buffer[6];
            reader.read(buffer, 6, error);
            if (!error)
            {
                if (strncmp(buffer, "value1", 6) == 0)
                {
                    result = TestResult::ePassed;
                }
            }
        }
    }

    return result;
}

TestResult::EOutcome PageRepositoryReaderReadTest2(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageRepositoryReaderReadTest2.dpdb");

    Ishiko::Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.open(inputPath, error);
    if (!error)
    {
        DiplodocusDB::PageRepositoryReader reader = repository.read(0, 0, error);
        if (!error)
        {
            char buffer[6];
            reader.read(buffer, 6, error);
            if (!error)
            {
                if (strncmp(buffer, "value1", 6) == 0)
                {
                    reader.read(buffer, 6, error);
                    if (!error)
                    {
                        if (strncmp(buffer, "value2", 6) == 0)
                        {
                            result = TestResult::ePassed;
                        }
                    }
                }
            }
        }
    }

    return result;
}
