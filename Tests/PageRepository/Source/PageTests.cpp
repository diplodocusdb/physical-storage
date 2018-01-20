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

#include "PageTests.h"
#include "DiplodocusDB/PhysicalStorage/PageRepository/Page.h"
#include "DiplodocusDB/PhysicalStorage/PageRepository/PageFileRepository.h"
#include <boost/filesystem/operations.hpp>

void AddPageTests(TestHarness& theTestHarness)
{
    TestSequence& pageTestSequence = theTestHarness.appendTestSequence("Page tests");

    new HeapAllocationErrorsTest("Creation test 1", PageCreationTest1, pageTestSequence);

    new HeapAllocationErrorsTest("load test 1", PageLoadTest1, pageTestSequence);

    new HeapAllocationErrorsTest("read test 1", PageReadTest1, pageTestSequence);

    new FileComparisonTest("write test 1", PageWriteTest1, pageTestSequence);
}

TestResult::EOutcome PageCreationTest1(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageCreationTest1.dpdb");

    Ishiko::Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.open(inputPath, error);
    if (!error)
    {
        DiplodocusDB::Page page(repository, 0);
        result = TestResult::ePassed;
    }

    return result;
}

TestResult::EOutcome PageLoadTest1(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageLoadTest1.dpdb");

    Ishiko::Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.open(inputPath, error);
    if (!error)
    {
        DiplodocusDB::Page page(repository, 0);
        page.load(error);
        if (!error)
        {
            if (page.dataSize() == 0)
            {
                result = TestResult::ePassed;
            }
        }
    }

    return result;
}

TestResult::EOutcome PageReadTest1(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;
    return result;
}

TestResult::EOutcome PageWriteTest1(FileComparisonTest& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageWriteTest1.dpdb");
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "PageWriteTest1.dpdb");

    boost::filesystem::copy_file(inputPath, outputPath, boost::filesystem::copy_option::overwrite_if_exists);

    Ishiko::Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.open(outputPath, error);
    if (!error)
    {
        DiplodocusDB::Page page(repository, 0);
        page.init();
        page.write("value1", 6, error);
        if (!error)
        {
            page.save(error);
            if (!error)
            {
                result = TestResult::ePassed;
            }
        }
    }

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PageWriteTest1.dpdb");
    
    return result;
}
