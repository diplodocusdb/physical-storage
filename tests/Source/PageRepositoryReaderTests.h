/*
    Copyright (c) 2018-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/diplodocusdb/physical-storage/blob/main/LICENSE.txt
*/

#ifndef _DIPLODOCUSDB_TEST_PHYSICALSTORAGE_PAGEREPOSITORY_PAGEREPOSITORYREADERTESTS_H_
#define _DIPLODOCUSDB_TEST_PHYSICALSTORAGE_PAGEREPOSITORY_PAGEREPOSITORYREADERTESTS_H_

#include <Ishiko/TestFramework.hpp>

class PageRepositoryReaderTests : public Ishiko::TestSequence
{
public:
    PageRepositoryReaderTests(const Ishiko::TestNumber& number, const Ishiko::TestContext& context);

private:
    static void CreationTest1(Ishiko::Test& test);
    static void ReadTest1(Ishiko::Test& test);
    static void ReadTest2(Ishiko::Test& test);
    static void ReadTest3(Ishiko::Test& test);
    static void ReadTest4(Ishiko::Test& test);
    static void ReadTest5(Ishiko::Test& test);
    static void ReadLEB128Test1(Ishiko::Test& test);
    static void ReadLEB128Test2(Ishiko::Test& test);
    static void ReadLEB128Test3(Ishiko::Test& test);
    static void ReadLEB128Test4(Ishiko::Test& test);
    static void ReadLEB128Test5(Ishiko::Test& test);
};

#endif
