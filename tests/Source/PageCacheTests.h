/*
    Copyright (c) 2019-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/diplodocusdb/physical-storage/blob/main/LICENSE.txt
*/

#ifndef _DIPLODOCUSDB_TEST_PHYSICALSTORAGE_PAGEREPOSITORY_PAGECACHETESTS_H_
#define _DIPLODOCUSDB_TEST_PHYSICALSTORAGE_PAGEREPOSITORY_PAGECACHETESTS_H_

#include <Ishiko/TestFramework.hpp>

class PageCacheTests : public Ishiko::TestSequence
{
public:
    PageCacheTests(const Ishiko::TestNumber& number, const Ishiko::TestContext& context);

private:
    static void ConstructorTest1(Ishiko::Test& test);
};

#endif
