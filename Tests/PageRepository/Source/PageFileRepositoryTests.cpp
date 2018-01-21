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

#include "PageFileRepositoryTests.h"
#include "DiplodocusDB/PhysicalStorage/PageRepository/PageFileRepository.h"

void AddPageFileRepositoryTests(TestHarness& theTestHarness)
{
    TestSequence& repositoryTestSequence = theTestHarness.appendTestSequence("PageFileRepository tests");

    new HeapAllocationErrorsTest("Creation test 1", PageFileRepositoryCreationTest1, repositoryTestSequence);

    new FileComparisonTest("create test 1", PageFileRepositoryCreateTest1, repositoryTestSequence);

    new HeapAllocationErrorsTest("open test 1", PageFileRepositoryOpenTest1, repositoryTestSequence);
    new HeapAllocationErrorsTest("open test 2", PageFileRepositoryOpenTest2, repositoryTestSequence);

    new FileComparisonTest("allocatePage test 1", PageFileRepositoryAllocatePageTest1, repositoryTestSequence);
    new FileComparisonTest("allocatePage test 2", PageFileRepositoryAllocatePageTest2, repositoryTestSequence);
}

TestResult::EOutcome PageFileRepositoryCreationTest1()
{
    DiplodocusDB::PageFileRepository repository;
    return TestResult::ePassed;
}

TestResult::EOutcome PageFileRepositoryCreateTest1(FileComparisonTest& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "PageFileRepositoryCreateTest1.dpdb");

    Ishiko::Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.create(outputPath, error);
    if (!error)
    {
        result = TestResult::ePassed;
    }
    repository.close();

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PageFileRepositoryCreateTest1.dpdb");

    return result;
}

TestResult::EOutcome PageFileRepositoryOpenTest1(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageFileRepositoryOpenTest1.dpdb");

    Ishiko::Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.open(inputPath, error);
    if (!error)
    {
        if (repository.pageCount() == 0)
        {
            result = TestResult::ePassed;
        }
    }

    return result;
}

TestResult::EOutcome PageFileRepositoryOpenTest2(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageFileRepositoryOpenTest2.dpdb");

    Ishiko::Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.open(inputPath, error);
    if (!error)
    {
        if (repository.pageCount() == 1)
        {
            result = TestResult::ePassed;
        }
    }

    return result;
}

TestResult::EOutcome PageFileRepositoryAllocatePageTest1(FileComparisonTest& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "PageFileRepositoryAllocatePageTest1.dpdb");

    Ishiko::Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.create(outputPath, error);
    if (!error)
    {
        std::shared_ptr<DiplodocusDB::Page> page = repository.allocatePage(error);
        if (!error && page)
        {
            page->save(error);
            if (!error)
            {
                result = TestResult::ePassed;
            }
        }
    }
    repository.close();

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PageFileRepositoryAllocatePageTest1.dpdb");

    return result;
}

TestResult::EOutcome PageFileRepositoryAllocatePageTest2(FileComparisonTest& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "PageFileRepositoryAllocatePageTest2.dpdb");

    Ishiko::Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.create(outputPath, error);
    if (!error)
    {
        std::shared_ptr<DiplodocusDB::Page> page1 = repository.allocatePage(error);
        if (!error && page1)
        {
            page1->save(error);
            if (!error)
            {
                std::shared_ptr<DiplodocusDB::Page> page2 = repository.allocatePage(error);
                if (!error && page2)
                {
                    page2->save(error);
                    if (!error)
                    {
                        result = TestResult::ePassed;
                    }
                }
            }
        }
    }
    repository.close();

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PageFileRepositoryAllocatePageTest2.dpdb");

    return result;
}