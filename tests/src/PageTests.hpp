/*
    Copyright (c) 2018-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/diplodocusdb/physical-storage/blob/main/LICENSE.txt
*/

#ifndef GUARD_DIPLODOCUSDB_PHYSICALSTORAGE_TESTS_PAGETESTS_HPP
#define GUARD_DIPLODOCUSDB_PHYSICALSTORAGE_TESTS_PAGETESTS_HPP

#include <Ishiko/TestFramework.hpp>

class PageTests : public Ishiko::TestSequence
{
public:
    PageTests(const Ishiko::TestNumber& number, const Ishiko::TestContext& context);

private:
    static void ConstructorTest1(Ishiko::Test& test);
};

#endif
