/*
    Copyright (c) 2018-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/diplodocusdb/physical-storage/blob/main/LICENSE.txt
*/

#include "PageFileTests.hpp"
#include "DiplodocusDB/PhysicalStorage/PageFile.hpp"
#include <boost/filesystem/operations.hpp>

using namespace DiplodocusDB::PhysicalStorage;
using namespace Ishiko;

PageFileTests::PageFileTests(const TestNumber& number, const TestContext& context)
    : TestSequence(number, "PageFile tests", context)
{
    append<HeapAllocationErrorsTest>("Creation test 1", CreationTest1);
    append<HeapAllocationErrorsTest>("create test 1", CreateTest1);
    append<HeapAllocationErrorsTest>("open test 1", OpenTest1);
    append<HeapAllocationErrorsTest>("open test 2", OpenTest2);
    append<HeapAllocationErrorsTest>("allocatePage test 1", AllocatePageTest1);
    append<HeapAllocationErrorsTest>("allocatePage test 2", AllocatePageTest2);
}

void PageFileTests::CreationTest1(Test& test)
{
    PageFile repository;

    ISHIKO_TEST_PASS();
}

void PageFileTests::CreateTest1(Test& test)
{
    const char* outputName = "PageFileRepositoryCreateTest1.dpdb";

    Error error;

    PageFile repository;
    repository.create(test.context().getOutputPath(outputName), error);

    ISHIKO_TEST_FAIL_IF(error);

    repository.close();

    ISHIKO_TEST_FAIL_IF_OUTPUT_AND_REFERENCE_FILES_NEQ(outputName);
    ISHIKO_TEST_PASS();
}

void PageFileTests::OpenTest1(Test& test)
{
    boost::filesystem::path inputPath = test.context().getDataPath("PageFileRepositoryOpenTest1.dpdb");

    Error error;

    PageFile repository;
    repository.open(inputPath, error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(repository.pageCount(), 0);
    ISHIKO_TEST_PASS();
}

void PageFileTests::OpenTest2(Test& test)
{
    boost::filesystem::path inputPath = test.context().getDataPath("PageFileRepositoryOpenTest2.dpdb");

    Error error;

    PageFile repository;
    repository.open(inputPath, error);
    
    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(repository.pageCount(), 1);
    ISHIKO_TEST_PASS();
}

void PageFileTests::AllocatePageTest1(Test& test)
{
    const char* outputName = "PageFileRepositoryTests_AllocatePageTest1.dpdb";

    Error error;

    PageFile repository;
    repository.create(test.context().getOutputPath(outputName), error);

    ISHIKO_TEST_ABORT_IF(error);

    Page page = repository.allocatePage(error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(page.number, 0);

    repository.close();

    ISHIKO_TEST_FAIL_IF_OUTPUT_AND_REFERENCE_FILES_NEQ(outputName);
    ISHIKO_TEST_PASS();
}

void PageFileTests::AllocatePageTest2(Test& test)
{
    const char* outputName = "PageFileRepositoryTests_AllocatePageTest2.dpdb";

    Error error;

    PageFile repository;
    repository.create(test.context().getOutputPath(outputName), error);

    ISHIKO_TEST_ABORT_IF(error);

    Page page1 = repository.allocatePage(error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(page1.number, 0);
    
    Page page2 = repository.allocatePage(error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(page2.number, 1);
                
    repository.close();

    ISHIKO_TEST_FAIL_IF_OUTPUT_AND_REFERENCE_FILES_NEQ(outputName);
    ISHIKO_TEST_PASS();
}
