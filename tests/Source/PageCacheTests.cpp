/*
    Copyright (c) 2019-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/diplodocusdb/physical-storage/blob/main/LICENSE.txt
*/

#include "PageCacheTests.h"
#include "DiplodocusDB/PhysicalStorage/PageCache.h"

using namespace DiplodocusDB::PhysicalStorage;
using namespace Ishiko;

PageCacheTests::PageCacheTests(const TestNumber& number, const TestContext& context)
    : TestSequence(number, "PageCache tests", context)
{
    append<HeapAllocationErrorsTest>("ConstructorTest1 test 1", ConstructorTest1);
}

void PageCacheTests::ConstructorTest1(Test& test)
{
    PageCache pageCache;

    ISHIKO_TEST_PASS();
}
