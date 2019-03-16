/*
    Copyright (c) 2018-2019 Xavier Leclercq

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

#ifndef _DIPLODOCUSDB_TEST_PHYSICALSTORAGE_PAGEREPOSITORY_PAGETESTS_H_
#define _DIPLODOCUSDB_TEST_PHYSICALSTORAGE_PAGEREPOSITORY_PAGETESTS_H_

#include "Ishiko/TestFramework/TestFrameworkCore.h"

class PageTests : public Ishiko::Tests::TestSequence
{
public:
    PageTests(const Ishiko::Tests::TestNumber& number, const Ishiko::Tests::TestEnvironment& environment);

private:
    static void CreationTest1(Ishiko::Tests::Test& test);
    static void LoadTest1(Ishiko::Tests::Test& test);
    static void LoadTest2(Ishiko::Tests::Test& test);
    static void GetTest1(Ishiko::Tests::Test& test);
    static void InsertNextPageTest1(Ishiko::Tests::FileComparisonTest& test);
    static void InsertTest1(Ishiko::Tests::FileComparisonTest& test);
    static void InsertTest2(Ishiko::Tests::FileComparisonTest& test);
    static void EraseTest1(Ishiko::Tests::FileComparisonTest& test);
    static void EraseTest2(Ishiko::Tests::FileComparisonTest& test);
    static void EraseTest3(Ishiko::Tests::FileComparisonTest& test);
    static void MoveToTest1(Ishiko::Tests::FileComparisonTest& test);
    static void MoveToTest2(Ishiko::Tests::FileComparisonTest& test);
};

#endif
