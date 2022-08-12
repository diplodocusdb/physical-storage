/*
    Copyright (c) 2018-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/diplodocusdb/physical-storage/blob/main/LICENSE.txt
*/

#include "PageRepositoryReaderTests.h"
#include "DiplodocusDB/PhysicalStorage/PageRepositoryReader.h"
#include "DiplodocusDB/PhysicalStorage/PageFileRepository2.hpp"

using namespace DiplodocusDB::PhysicalStorage;
using namespace Ishiko;

PageRepositoryReaderTests::PageRepositoryReaderTests(const TestNumber& number, const TestContext& context)
    : TestSequence(number, "PageRepositoryReader tests", context)
{
    append<HeapAllocationErrorsTest>("Creation test 1", CreationTest1);
    append<HeapAllocationErrorsTest>("read test 1", ReadTest1);
    append<HeapAllocationErrorsTest>("read test 2", ReadTest2);
    append<HeapAllocationErrorsTest>("read test 3", ReadTest3);
    append<HeapAllocationErrorsTest>("read test 4", ReadTest4);
    append<HeapAllocationErrorsTest>("read test 5", ReadTest5);
    append<HeapAllocationErrorsTest>("readLEB128 test 1", ReadLEB128Test1);
    append<HeapAllocationErrorsTest>("readLEB128 test 2", ReadLEB128Test2);
    append<HeapAllocationErrorsTest>("readLEB128 test 3", ReadLEB128Test3);
    append<HeapAllocationErrorsTest>("readLEB128 test 4", ReadLEB128Test4);
    append<HeapAllocationErrorsTest>("readLEB128 test 5", ReadLEB128Test5);
}

void PageRepositoryReaderTests::CreationTest1(Test& test)
{
    Error error;

    PageFileRepository2 repository;
    repository.open(test.context().getDataPath("PageRepositoryReaderCreationTest1.dpdb"), error);

    ISHIKO_TEST_ABORT_IF(error);

    std::shared_ptr<Page2> page = repository.page(0, error);

    ISHIKO_TEST_ABORT_IF(error);
    
    PageRepositoryReader reader(repository, page, 0);

    ISHIKO_TEST_FAIL_IF_NEQ(reader.currentPosition().page(), 0);
    ISHIKO_TEST_FAIL_IF_NEQ(reader.currentPosition().offset(), 0);
    ISHIKO_TEST_PASS();
}

void PageRepositoryReaderTests::ReadTest1(Test& test)
{
    Error error;

    PageFileRepository2 repository;
    repository.open(test.context().getDataPath("PageRepositoryReaderReadTest1.dpdb"), error);
    
    ISHIKO_TEST_ABORT_IF(error);

    PageRepositoryReader reader = repository.read(0, 0, error);

    ISHIKO_TEST_ABORT_IF(error);

    char buffer[6];
    reader.read(buffer, 6, error);
    
    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NOT(strncmp(buffer, "value1", 6) == 0);
    ISHIKO_TEST_FAIL_IF_NEQ(reader.currentPosition().page(), 0);
    ISHIKO_TEST_FAIL_IF_NEQ(reader.currentPosition().offset(), 6);
    ISHIKO_TEST_PASS();
}

void PageRepositoryReaderTests::ReadTest2(Test& test)
{
    Error error;

    PageFileRepository2 repository;
    repository.open(test.context().getDataPath("PageRepositoryReaderReadTest2.dpdb"), error);

    ISHIKO_TEST_ABORT_IF(error);
    
    PageRepositoryReader reader = repository.read(0, 0, error);

    ISHIKO_TEST_ABORT_IF(error);
        
    char buffer[6];
    reader.read(buffer, 6, error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NOT(strncmp(buffer, "value1", 6) == 0);
                
    reader.read(buffer, 6, error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NOT(strncmp(buffer, "value2", 6) == 0);
    ISHIKO_TEST_FAIL_IF_NEQ(reader.currentPosition().page(), 0);
    ISHIKO_TEST_FAIL_IF_NEQ(reader.currentPosition().offset(), 12);
    ISHIKO_TEST_PASS();
}

void PageRepositoryReaderTests::ReadTest3(Test& test)
{
    Error error;

    PageFileRepository2 repository;
    repository.open(test.context().getDataPath("PageRepositoryReaderReadTest3.dpdb"), error);

    ISHIKO_TEST_ABORT_IF(error);

    PageRepositoryReader reader = repository.read(0, 6, error);

    ISHIKO_TEST_ABORT_IF(error);
        
    char buffer[6];
    reader.read(buffer, 6, error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NOT(strncmp(buffer, "value2", 6) == 0);
    ISHIKO_TEST_FAIL_IF_NEQ(reader.currentPosition().page(), 0);
    ISHIKO_TEST_FAIL_IF_NEQ(reader.currentPosition().offset(), 12);
    ISHIKO_TEST_PASS();
}

/// Checks the behavior when we read the very last byte of a page and then read the first bytes of the next page in 
/// two consecutive calls to the read function.
void PageRepositoryReaderTests::ReadTest4(Test& test)
{
    Error error;

    PageFileRepository2 repository;
    repository.open(test.context().getDataPath("PageRepositoryReaderReadTest4.dpdb"), error);

    ISHIKO_TEST_ABORT_IF(error);

    PageRepositoryReader reader = repository.read(0, 0, error);

    ISHIKO_TEST_ABORT_IF(error);

    for (size_t i = 0; i < 409; ++i)
    {
        char buffer[10];
        reader.read(buffer, 10, error);

        if (strncmp(buffer, "0123456789", 10) != 0)
        {
            ISHIKO_TEST_FAIL();
        }
    }

    ISHIKO_TEST_FAIL_IF_NEQ(reader.currentPosition().page(), 1);
    ISHIKO_TEST_FAIL_IF_NEQ(reader.currentPosition().offset(), 10);
    ISHIKO_TEST_PASS();
}

/// Checks the behavior when a single call to the read function crosses a single page boundary.
void PageRepositoryReaderTests::ReadTest5(Test& test)
{
    Error error;

    PageFileRepository2 repository;
    repository.open(test.context().getDataPath("PageRepositoryReaderReadTest5.dpdb"), error);

    ISHIKO_TEST_ABORT_IF(error);

    PageRepositoryReader reader = repository.read(0, 4070, error);

    ISHIKO_TEST_ABORT_IF(error);

    char buffer[15];
    reader.read(buffer, 15, error);

    ISHIKO_TEST_FAIL_IF_NOT(strncmp(buffer, "012345678901234", 15) == 0);
    ISHIKO_TEST_FAIL_IF_NEQ(reader.currentPosition().page(), 1);
    ISHIKO_TEST_FAIL_IF_NEQ(reader.currentPosition().offset(), 5);
    ISHIKO_TEST_PASS();
}

void PageRepositoryReaderTests::ReadLEB128Test1(Test& test)
{
    Error error;

    PageFileRepository2 repository;
    repository.open(test.context().getDataPath("PageRepositoryReaderReadLEB128Test1.dpdb"), error);

    ISHIKO_TEST_ABORT_IF(error);

    PageRepositoryReader reader = repository.read(0, 0, error);

    ISHIKO_TEST_ABORT_IF(error);

    size_t n = reader.readLEB128(error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(n, 0);
    ISHIKO_TEST_PASS();
}

void PageRepositoryReaderTests::ReadLEB128Test2(Test& test)
{
    Error error;

    PageFileRepository2 repository;
    repository.open(test.context().getDataPath("PageRepositoryReaderReadLEB128Test2.dpdb"), error);

    ISHIKO_TEST_ABORT_IF(error);

    PageRepositoryReader reader = repository.read(0, 0, error);

    ISHIKO_TEST_ABORT_IF(error);

    size_t n = reader.readLEB128(error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(n, 1);
    ISHIKO_TEST_PASS();
}

void PageRepositoryReaderTests::ReadLEB128Test3(Test& test)
{
    Error error;

    PageFileRepository2 repository;
    repository.open(test.context().getDataPath("PageRepositoryReaderReadLEB128Test3.dpdb"), error);

    ISHIKO_TEST_ABORT_IF(error);

    PageRepositoryReader reader = repository.read(0, 0, error);

    ISHIKO_TEST_ABORT_IF(error);

    size_t n = reader.readLEB128(error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(n, 127);
    ISHIKO_TEST_PASS();
}

void PageRepositoryReaderTests::ReadLEB128Test4(Test& test)
{
    Error error;

    PageFileRepository2 repository;
    repository.open(test.context().getDataPath("PageRepositoryReaderReadLEB128Test4.dpdb"), error);

    ISHIKO_TEST_ABORT_IF(error);

    PageRepositoryReader reader = repository.read(0, 0, error);

    ISHIKO_TEST_ABORT_IF(error);

    size_t n = reader.readLEB128(error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(n, 128);
    ISHIKO_TEST_PASS();
}

void PageRepositoryReaderTests::ReadLEB128Test5(Test& test)
{
    Error error;

    PageFileRepository2 repository;
    repository.open(test.context().getDataPath("PageRepositoryReaderReadLEB128Test5.dpdb"), error);

    ISHIKO_TEST_ABORT_IF(error);

    PageRepositoryReader reader = repository.read(0, 0, error);

    ISHIKO_TEST_ABORT_IF(error);

    size_t n = reader.readLEB128(error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(n, 16384);
    ISHIKO_TEST_PASS();
}
