/*
    Copyright (c) 2018-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/diplodocusdb/physical-storage/blob/main/LICENSE.txt
*/

#ifndef GUARD_DIPLODOCUSDB_PHYSICALSTORAGE_TESTS_PAGEFILETESTS_HPP
#define GUARD_DIPLODOCUSDB_PHYSICALSTORAGE_TESTS_PAGEFILETESTS_HPP

#include <Ishiko/TestFramework.hpp>

class PageFileTests : public Ishiko::TestSequence
{
public:
    PageFileTests(const Ishiko::TestNumber& number, const Ishiko::TestContext& context);

private:
    static void CreationTest1(Ishiko::Test& test);
    static void CreateTest1(Ishiko::Test& test);
    static void OpenTest1(Ishiko::Test& test);
    static void OpenTest2(Ishiko::Test& test);
    static void AllocatePageTest1(Ishiko::Test& test);
    static void AllocatePageTest2(Ishiko::Test& test);
};

#endif
