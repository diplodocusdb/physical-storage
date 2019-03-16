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

#include "PageTests.h"
#include "DiplodocusDB/PhysicalStorage/PageRepository/Page.h"
#include "DiplodocusDB/PhysicalStorage/PageRepository/PageFileRepository.h"
#include <boost/filesystem/operations.hpp>

using namespace Ishiko::Tests;

PageTests::PageTests(const TestNumber& number, const TestEnvironment& environment)
    : TestSequence(number, "Page tests", environment)
{
    append<HeapAllocationErrorsTest>("Creation test 1", CreationTest1);
    append<HeapAllocationErrorsTest>("load test 1", LoadTest1);
    append<HeapAllocationErrorsTest>("load test 2", LoadTest2);
    append<HeapAllocationErrorsTest>("get test 1", GetTest1);
    append<FileComparisonTest>("insertNextPage test 1", InsertNextPageTest1);
    append<FileComparisonTest>("insert test 1", InsertTest1);
    append<FileComparisonTest>("insert test 2", InsertTest2);
    append<FileComparisonTest>("erase test 1", EraseTest1);
    append<FileComparisonTest>("erase test 2", EraseTest2);
    append<FileComparisonTest>("erase test 3", EraseTest3);
    append<FileComparisonTest>("moveTo test 1", MoveToTest1);
    append<FileComparisonTest>("moveTo test 2", MoveToTest2);
}

void PageTests::CreationTest1(Test& test)
{
    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageCreationTest1.dpdb");

    Ishiko::Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.open(inputPath, error);

    ISHTF_ABORT_IF((bool)error);
    
    DiplodocusDB::Page page(repository, 0);

    ISHTF_PASS();
}

void PageTests::LoadTest1(Test& test)
{
    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageLoadTest1.dpdb");

    Ishiko::Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.open(inputPath, error);

    ISHTF_ABORT_IF((bool)error);
    
    DiplodocusDB::Page page(repository, 0);
    page.load(error);
    
    ISHTF_FAIL_IF((bool)error);
    ISHTF_FAIL_UNLESS(page.dataSize() == 0);
    ISHTF_FAIL_UNLESS(page.availableSpace() == 4080);
    ISHTF_PASS();
}

void PageTests::LoadTest2(Test& test)
{
    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageLoadTest2.dpdb");

    Ishiko::Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.open(inputPath, error);

    ISHTF_ABORT_IF((bool)error);
    
    DiplodocusDB::Page page(repository, 0);
    page.load(error);

    ISHTF_FAIL_IF((bool)error);
    ISHTF_FAIL_UNLESS(page.dataSize() == 6);
    ISHTF_FAIL_UNLESS(page.availableSpace() == 4074);
    ISHTF_PASS();
}

void PageTests::GetTest1(Test& test)
{
    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageGetTest1.dpdb");

    Ishiko::Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.open(inputPath, error);

    ISHTF_ABORT_IF((bool)error);
    
    DiplodocusDB::Page page(repository, 0);
    page.load(error);

    ISHTF_FAIL_IF((bool)error);
    ISHTF_FAIL_UNLESS(page.dataSize() == 6);
       
    char buffer[6];
    page.get(buffer, 0, 6, error);

    ISHTF_FAIL_IF((bool)error);
    ISHTF_FAIL_UNLESS(strncmp(buffer, "value1", 6) == 0);
    ISHTF_PASS();
}

void PageTests::InsertNextPageTest1(FileComparisonTest& test)
{
    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageInsertNextPageTest1.dpdb");
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "PageInsertNextPageTest1.dpdb");

    boost::filesystem::copy_file(inputPath, outputPath, boost::filesystem::copy_option::overwrite_if_exists);

    Ishiko::Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.open(outputPath, error);

    ISHTF_ABORT_IF((bool)error);
    
    std::shared_ptr<DiplodocusDB::Page> page1 = repository.page(0, error);

    ISHTF_ABORT_IF((bool)error);
    ISHTF_ABORT_UNLESS(page1);
    
    std::shared_ptr<DiplodocusDB::Page> page2 = page1->insertNextPage(error);

    ISHTF_ABORT_IF((bool)error);

    page1->save(error);

    ISHTF_FAIL_IF((bool)error);

    page2->save(error);
    
    ISHTF_FAIL_IF((bool)error);
        
    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PageInsertNextPageTest1.dpdb");

    ISHTF_PASS();
}

void PageTests::InsertTest1(FileComparisonTest& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageInsertTest1.dpdb");
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "PageInsertTest1.dpdb");

    boost::filesystem::copy_file(inputPath, outputPath, boost::filesystem::copy_option::overwrite_if_exists);

    Ishiko::Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.open(outputPath, error);
    if (!error)
    {
        DiplodocusDB::Page page(repository, 0);
        page.init();
        page.insert("value1", 6, 0, error);
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
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PageInsertTest1.dpdb");
    
    return result;
}

void PageTests::InsertTest2(FileComparisonTest& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageInsertTest2.dpdb");
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "PageInsertTest2.dpdb");

    boost::filesystem::copy_file(inputPath, outputPath, boost::filesystem::copy_option::overwrite_if_exists);

    Ishiko::Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.open(outputPath, error);
    if (!error)
    {
        DiplodocusDB::Page page(repository, 0);
        page.load(error);
        if (!error)
        {
            page.insert("value0", 6, 0, error);
            if (!error)
            {
                page.save(error);
                if (!error)
                {
                    result = TestResult::ePassed;
                }
            }
        }
    }

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PageInsertTest2.dpdb");

    return result;
}

void PageTests::EraseTest1(FileComparisonTest& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageEraseTest1.dpdb");
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "PageEraseTest1.dpdb");

    boost::filesystem::copy_file(inputPath, outputPath, boost::filesystem::copy_option::overwrite_if_exists);

    Ishiko::Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.open(outputPath, error);
    if (!error)
    {
        DiplodocusDB::Page page(repository, 0);
        page.load(error);
        if (!error)
        {
            page.erase(0, 6, error);
            if (!error)
            {
                if ((page.dataSize() == 0) && (page.availableSpace() == 4080))
                {
                    page.save(error);
                    if (!error)
                    {
                        result = TestResult::ePassed;
                    }
                }
            }
        }
    }

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PageEraseTest1.dpdb");

    return result;
}

void PageTests::EraseTest2(FileComparisonTest& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageEraseTest2.dpdb");
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "PageEraseTest2.dpdb");

    boost::filesystem::copy_file(inputPath, outputPath, boost::filesystem::copy_option::overwrite_if_exists);

    Ishiko::Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.open(outputPath, error);
    if (!error)
    {
        DiplodocusDB::Page page(repository, 0);
        page.load(error);
        if (!error)
        {
            page.erase(5, 1, error);
            if (!error)
            {
                if ((page.dataSize() == 5) && (page.availableSpace() == 4075))
                {
                    page.save(error);
                    if (!error)
                    {
                        result = TestResult::ePassed;
                    }
                }
            }
        }
    }

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PageEraseTest2.dpdb");

    return result;
}

void PageTests::EraseTest3(FileComparisonTest& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageEraseTest3.dpdb");
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "PageEraseTest3.dpdb");

    boost::filesystem::copy_file(inputPath, outputPath, boost::filesystem::copy_option::overwrite_if_exists);

    Ishiko::Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.open(outputPath, error);
    if (!error)
    {
        DiplodocusDB::Page page(repository, 0);
        page.load(error);
        if (!error)
        {
            page.erase(2, 10, error);
            if (!error)
            {
                if ((page.dataSize() == 2) && (page.availableSpace() == 4078))
                {
                    page.save(error);
                    if (!error)
                    {
                        result = TestResult::ePassed;
                    }
                }
            }
        }
    }

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PageEraseTest3.dpdb");

    return result;
}

void PageTests::MoveToTest1(FileComparisonTest& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageMoveToTest1.dpdb");
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "PageMoveToTest1.dpdb");

    boost::filesystem::copy_file(inputPath, outputPath, boost::filesystem::copy_option::overwrite_if_exists);

    Ishiko::Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.open(outputPath, error);
    if (!error)
    {
        std::shared_ptr<DiplodocusDB::Page> page1 = repository.page(0, error);
        if (!error)
        {
            std::shared_ptr<DiplodocusDB::Page> page2 = repository.page(1, error);
            if (!error)
            {
                page1->moveTo(0, 6, *page2, error);
                if (!error)
                {
                    if ((page1->dataSize() == 0) && (page2->dataSize() == 6))
                    {
                        page1->save(error);
                        if (!error)
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
        }
    }

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PageMoveToTest1.dpdb");

    return result;
}

void PageTests::MoveToTest2(FileComparisonTest& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageMoveToTest2.dpdb");
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "PageMoveToTest2.dpdb");

    boost::filesystem::copy_file(inputPath, outputPath, boost::filesystem::copy_option::overwrite_if_exists);

    Ishiko::Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.open(outputPath, error);
    if (!error)
    {
        std::shared_ptr<DiplodocusDB::Page> page1 = repository.page(0, error);
        if (!error)
        {
            std::shared_ptr<DiplodocusDB::Page> page2 = repository.page(1, error);
            if (!error)
            {
                page1->moveTo(0, 6, *page2, error);
                if (!error)
                {
                    if ((page1->dataSize() == 0) && (page2->dataSize() == 12))
                    {
                        page1->save(error);
                        if (!error)
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
        }
    }

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PageMoveToTest2.dpdb");

    return result;
}
