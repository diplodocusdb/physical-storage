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

#include "PageFileRepositoryTests.h"
#include "DiplodocusDB/PhysicalStorage/PageRepository/PageFileRepository.h"

using namespace Ishiko::Tests;

PageFileRepositoryTests::PageFileRepositoryTests(const TestNumber& number, const TestEnvironment& environment)
    : TestSequence(number, "PageFileRepository tests", environment)
{
    append<HeapAllocationErrorsTest>("Creation test 1", CreationTest1);
    append<FileComparisonTest>("create test 1", CreateTest1);
    append<HeapAllocationErrorsTest>("open test 1", OpenTest1);
    append<HeapAllocationErrorsTest>("open test 2", OpenTest2);
    append<FileComparisonTest>("allocatePage test 1", AllocatePageTest1);
    append<FileComparisonTest>("allocatePage test 2", AllocatePageTest2);
}

void PageFileRepositoryTests::CreationTest1(Test& test)
{
    DiplodocusDB::PageFileRepository repository;
    ISHTF_PASS();
}

void PageFileRepositoryTests::CreateTest1(FileComparisonTest& test)
{
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory()
        / "PageFileRepositoryCreateTest1.dpdb");

    Ishiko::Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.create(outputPath, error);

    ISHTF_FAIL_IF((bool)error);

    repository.close();

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PageFileRepositoryCreateTest1.dpdb");

    ISHTF_PASS();
}

void PageFileRepositoryTests::OpenTest1(Test& test)
{
    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageFileRepositoryOpenTest1.dpdb");

    Ishiko::Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.open(inputPath, error);

    ISHTF_FAIL_IF((bool)error);
    ISHTF_FAIL_UNLESS(repository.pageCount() == 0);
    ISHTF_PASS();
}

void PageFileRepositoryTests::OpenTest2(Test& test)
{
    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageFileRepositoryOpenTest2.dpdb");

    Ishiko::Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.open(inputPath, error);
    
    ISHTF_FAIL_IF((bool)error);
    ISHTF_FAIL_UNLESS(repository.pageCount() == 1);
    ISHTF_PASS();
}

void PageFileRepositoryTests::AllocatePageTest1(FileComparisonTest& test)
{
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory()
        / "PageFileRepositoryAllocatePageTest1.dpdb");

    Ishiko::Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.create(outputPath, error);

    ISHTF_ABORT_IF((bool)error);
    
    std::shared_ptr<DiplodocusDB::Page> page = repository.allocatePage(error);

    ISHTF_ABORT_IF((bool)error);
    ISHTF_ABORT_UNLESS(page);

    page->save(error);

    ISHTF_FAIL_IF((bool)error);

    repository.close();

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory()
        / "PageFileRepositoryAllocatePageTest1.dpdb");

    ISHTF_PASS();
}

void PageFileRepositoryTests::AllocatePageTest2(FileComparisonTest& test)
{
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory()
        / "PageFileRepositoryAllocatePageTest2.dpdb");

    Ishiko::Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.create(outputPath, error);

    ISHTF_ABORT_IF((bool)error);

    std::shared_ptr<DiplodocusDB::Page> page1 = repository.allocatePage(error);

    ISHTF_ABORT_IF((bool)error);
    ISHTF_ABORT_UNLESS(page1);
    
    page1->save(error);

    ISHTF_ABORT_IF((bool)error);
 
    std::shared_ptr<DiplodocusDB::Page> page2 = repository.allocatePage(error);

    ISHTF_ABORT_IF((bool)error);
    ISHTF_ABORT_UNLESS(page2);
                
    page2->save(error);

    ISHTF_FAIL_IF((bool)error);

    repository.close();

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory()
        / "PageFileRepositoryAllocatePageTest2.dpdb");

    ISHTF_PASS();
}
