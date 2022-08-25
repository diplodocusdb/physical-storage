/*
    Copyright (c) 2018-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/diplodocusdb/physical-storage/blob/main/LICENSE.txt
*/

#include "PageTests.hpp"
#include "DiplodocusDB/PhysicalStorage/Page.hpp"
#include "DiplodocusDB/PhysicalStorage/PageFile.hpp"

using namespace DiplodocusDB::PhysicalStorage;
using namespace Ishiko;

PageTests::PageTests(const TestNumber& number, const TestContext& context)
    : TestSequence(number, "Page tests", context)
{
    append<HeapAllocationErrorsTest>("ConstructorTest1 test 1", ConstructorTest1);
}

void PageTests::ConstructorTest1(Test& test)
{
    Page page{0};

    ISHIKO_TEST_PASS();
}
