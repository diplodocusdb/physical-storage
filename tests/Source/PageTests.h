/*
    Copyright (c) 2018-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/diplodocusdb/physical-storage/blob/main/LICENSE.txt
*/

#ifndef _DIPLODOCUSDB_TEST_PHYSICALSTORAGE_PAGEREPOSITORY_PAGETESTS_H_
#define _DIPLODOCUSDB_TEST_PHYSICALSTORAGE_PAGEREPOSITORY_PAGETESTS_H_

#include <Ishiko/TestFramework.hpp>

class PageTests : public Ishiko::TestSequence
{
public:
    PageTests(const Ishiko::TestNumber& number, const Ishiko::TestContext& context);

private:
    static void ConstructorTest1(Ishiko::Test& test);
    static void ReadTest1(Ishiko::Test& test);
    static void ReadTest2(Ishiko::Test& test);
    static void ReadTest3(Ishiko::Test& test);
    static void ReadTest4(Ishiko::Test& test);
    static void ReadTest5(Ishiko::Test& test);
    static void WriteTest1(Ishiko::Test& test);
    static void GetTest1(Ishiko::Test& test);
    static void InsertTest1(Ishiko::Test& test);
    static void InsertTest2(Ishiko::Test& test);
    static void InsertTest3(Ishiko::Test& test);
    static void EraseTest1(Ishiko::Test& test);
    static void EraseTest2(Ishiko::Test& test);
    static void EraseTest3(Ishiko::Test& test);
    static void MoveToTest1(Ishiko::Test& test);
    static void MoveToTest2(Ishiko::Test& test);
};

#endif
