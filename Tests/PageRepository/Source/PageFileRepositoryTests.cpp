/*
    Copyright (c) 2018-2022 Xavier Leclercq

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
#include <boost/filesystem/operations.hpp>

using namespace Ishiko;

PageFileRepositoryTests::PageFileRepositoryTests(const TestNumber& number, const TestContext& context)
    : TestSequence(number, "PageFileRepository tests", context)
{
    append<HeapAllocationErrorsTest>("Creation test 1", CreationTest1);
    append<HeapAllocationErrorsTest>("create test 1", CreateTest1);
    append<HeapAllocationErrorsTest>("open test 1", OpenTest1);
    append<HeapAllocationErrorsTest>("open test 2", OpenTest2);
    append<HeapAllocationErrorsTest>("allocatePage test 1", AllocatePageTest1);
    append<HeapAllocationErrorsTest>("allocatePage test 2", AllocatePageTest2);
    append<HeapAllocationErrorsTest>("insertPageAfter test 1", InsertPageAfterTest1);
}

void PageFileRepositoryTests::CreationTest1(Test& test)
{
    DiplodocusDB::PageFileRepository repository;

    ISHIKO_TEST_PASS();
}

void PageFileRepositoryTests::CreateTest1(Test& test)
{
    const char* outputName = "PageFileRepositoryCreateTest1.dpdb";

    Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.create(test.context().getOutputPath(outputName), error);

    ISHIKO_TEST_FAIL_IF(error);

    repository.close();

    ISHIKO_TEST_FAIL_IF_OUTPUT_AND_REFERENCE_FILES_NEQ(outputName);
    ISHIKO_TEST_PASS();
}

void PageFileRepositoryTests::OpenTest1(Test& test)
{
    boost::filesystem::path inputPath = test.context().getDataPath("PageFileRepositoryOpenTest1.dpdb");

    Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.open(inputPath, error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(repository.pageCount(), 0);
    ISHIKO_TEST_PASS();
}

void PageFileRepositoryTests::OpenTest2(Test& test)
{
    boost::filesystem::path inputPath = test.context().getDataPath("PageFileRepositoryOpenTest2.dpdb");

    Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.open(inputPath, error);
    
    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(repository.pageCount(), 1);
    ISHIKO_TEST_PASS();
}

void PageFileRepositoryTests::AllocatePageTest1(Test& test)
{
    const char* outputName = "PageFileRepositoryTests_AllocatePageTest1.dpdb";

    Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.create(test.context().getOutputPath(outputName), error);

    ISHIKO_TEST_ABORT_IF(error);

    std::shared_ptr<DiplodocusDB::Page> page = repository.allocatePage(error);

    ISHIKO_TEST_ABORT_IF(error);
    ISHIKO_TEST_ABORT_IF_NOT(page);

    repository.save(*page, error);

    ISHIKO_TEST_FAIL_IF(error);

    repository.close();

    ISHIKO_TEST_FAIL_IF_OUTPUT_AND_REFERENCE_FILES_NEQ(outputName);
    ISHIKO_TEST_PASS();
}

void PageFileRepositoryTests::AllocatePageTest2(Test& test)
{
    const char* outputName = "PageFileRepositoryTests_AllocatePageTest2.dpdb";

    Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.create(test.context().getOutputPath(outputName), error);

    ISHIKO_TEST_ABORT_IF(error);

    std::shared_ptr<DiplodocusDB::Page> page1 = repository.allocatePage(error);

    ISHIKO_TEST_ABORT_IF(error);
    ISHIKO_TEST_ABORT_IF_NOT(page1);
    
    repository.save(*page1, error);

    ISHIKO_TEST_ABORT_IF(error);
 
    std::shared_ptr<DiplodocusDB::Page> page2 = repository.allocatePage(error);

    ISHIKO_TEST_ABORT_IF(error);
    ISHIKO_TEST_ABORT_IF_NOT(page2);
                
    repository.save(*page2, error);

    ISHIKO_TEST_FAIL_IF(error);

    repository.close();

    ISHIKO_TEST_FAIL_IF_OUTPUT_AND_REFERENCE_FILES_NEQ(outputName);
    ISHIKO_TEST_PASS();
}

void PageFileRepositoryTests::InsertPageAfterTest1(Test& test)
{
    boost::filesystem::path inputPath(test.context().getTestDataDirectory()
        / "PageFileRepositoryTests_InsertPageAfterTest1.dpdb");
    boost::filesystem::path outputPath(test.context().getTestOutputDirectory()
        / "PageFileRepositoryTests_InsertPageAfterTest1.dpdb");

    boost::filesystem::copy_file(inputPath, outputPath, boost::filesystem::copy_option::overwrite_if_exists);

    Ishiko::Error error(0);

    DiplodocusDB::PageFileRepository repository;
    repository.open(outputPath, error);

    ISHIKO_TEST_ABORT_IF(error);

    std::shared_ptr<DiplodocusDB::Page> page1 = repository.page(0, error);

    ISHIKO_TEST_ABORT_IF(error);
    ISHIKO_TEST_ABORT_IF_NOT(page1);

    std::shared_ptr<DiplodocusDB::Page> page2 = repository.insertPageAfter(*page1, error);

    ISHIKO_TEST_ABORT_IF(error);

    repository.save(*page1, error);

    ISHIKO_TEST_FAIL_IF(error);

    repository.save(*page2, error);

    ISHIKO_TEST_FAIL_IF(error);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.context().getReferenceDataDirectory()
        / "PageFileRepositoryTests_InsertPageAfterTest1.dpdb");

    ISHIKO_TEST_PASS();
}
