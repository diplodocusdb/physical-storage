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

#include "PageTests.h"
#include "DiplodocusDB/PhysicalStorage/PageRepository/Page.h"

using namespace Ishiko::Tests;

PageTests::PageTests(const TestNumber& number, const TestEnvironment& environment)
    : TestSequence(number, "Page tests", environment)
{
    append<HeapAllocationErrorsTest>("ConstructorTest1 test 1", ConstructorTest1);
    append<HeapAllocationErrorsTest>("read test 1", ReadTest1);
    append<HeapAllocationErrorsTest>("read test 2", ReadTest2);
    append<HeapAllocationErrorsTest>("read test 3", ReadTest3);
    append<HeapAllocationErrorsTest>("read test 4", ReadTest4);
    append<HeapAllocationErrorsTest>("read test 5", ReadTest5);
    append<HeapAllocationErrorsTest>("get test 1", GetTest1);
    append<FileComparisonTest>("insert test 1", InsertTest1);
    append<FileComparisonTest>("insert test 2", InsertTest2);
    append<FileComparisonTest>("erase test 1", EraseTest1);
    append<FileComparisonTest>("erase test 2", EraseTest2);
    append<FileComparisonTest>("erase test 3", EraseTest3);
    append<FileComparisonTest>("moveTo test 1", MoveToTest1);
    append<FileComparisonTest>("moveTo test 2", MoveToTest2);
}

void PageTests::ConstructorTest1(Test& test)
{
    DiplodocusDB::Page page(0);

    ISHTF_PASS();
}

/// Tests reading an empty page.
void PageTests::ReadTest1(Test& test)
{
    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageTests_ReadTest1.dpdb");
    
    DiplodocusDB::Page page(0);

    Ishiko::Error error(0);
    std::ifstream input(inputPath.c_str(), std::fstream::binary);
    page.read(input, error);
    
    ISHTF_FAIL_IF((bool)error);
    ISHTF_FAIL_UNLESS(page.dataSize() == 0);
    ISHTF_FAIL_UNLESS(page.availableSpace() == 4080);
    ISHTF_PASS();
}

/// Tests reading a page containing 6 bytes of data.
void PageTests::ReadTest2(Test& test)
{
    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageTests_ReadTest2.dpdb");

    DiplodocusDB::Page page(0);

    Ishiko::Error error(0);
    std::ifstream input(inputPath.c_str(), std::fstream::binary);
    page.read(input, error);

    ISHTF_FAIL_IF((bool)error);
    ISHTF_FAIL_UNLESS(page.dataSize() == 6);
    ISHTF_FAIL_UNLESS(page.availableSpace() == 4074);
    ISHTF_PASS();
}

/// Tests reading the second page.
void PageTests::ReadTest3(Test& test)
{
    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageTests_ReadTest3.dpdb");

    DiplodocusDB::Page page(1);

    Ishiko::Error error(0);
    std::ifstream input(inputPath.c_str(), std::fstream::binary);
    page.read(input, error);

    ISHTF_FAIL_IF((bool)error);
    ISHTF_FAIL_UNLESS(page.dataSize() == 10);
    ISHTF_FAIL_UNLESS(page.availableSpace() == 4070);
    ISHTF_PASS();
}

/// Tests reading a page that doesn't exist.
void PageTests::ReadTest4(Test& test)
{
    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageTests_ReadTest4.dpdb");

    DiplodocusDB::Page page(1);

    Ishiko::Error error(0);
    std::ifstream input(inputPath.c_str(), std::fstream::binary);
    page.read(input, error);

    ISHTF_FAIL_UNLESS((bool)error);
    ISHTF_FAIL_UNLESS(page.dataSize() == 0);
    ISHTF_FAIL_UNLESS(page.availableSpace() == 4080);
    ISHTF_PASS();
}

/// Tests reading a page that is incomplete, this should be impossible unless the file has been truncated.
void PageTests::ReadTest5(Test& test)
{
    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageTests_ReadTest5.dpdb");

    DiplodocusDB::Page page(0);

    Ishiko::Error error(0);
    std::ifstream input(inputPath.c_str(), std::fstream::binary);
    page.read(input, error);

    ISHTF_FAIL_UNLESS((bool)error);
    ISHTF_FAIL_UNLESS(page.dataSize() == 0);
    ISHTF_FAIL_UNLESS(page.availableSpace() == 4080);
    ISHTF_PASS();
}

void PageTests::GetTest1(Test& test)
{
    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageTests_GetTest1.dpdb");

    DiplodocusDB::Page page(0);

    Ishiko::Error error(0);
    std::ifstream input(inputPath.c_str(), std::fstream::binary);
    page.read(input, error);

    ISHTF_ABORT_IF((bool)error);
    ISHTF_FAIL_UNLESS(page.dataSize() == 6);
    ISHTF_FAIL_UNLESS(page.availableSpace() == 4074);
       
    char buffer[6];
    page.get(buffer, 0, 6, error);

    ISHTF_FAIL_IF((bool)error);
    ISHTF_FAIL_UNLESS(strncmp(buffer, "value1", 6) == 0);
    ISHTF_PASS();
}

void PageTests::InsertTest1(FileComparisonTest& test)
{
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "PageTests_InsertTest1.dpdb");

    Ishiko::Error error(0);

    DiplodocusDB::Page page(0);
    page.init();
    page.insert("value1", 6, 0, error);

    ISHTF_FAIL_IF((bool)error);

    std::ofstream output(outputPath.c_str(), std::fstream::binary);
    page.write(output, error);
    
    ISHTF_FAIL_IF((bool)error);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PageTests_InsertTest1.dpdb");

    ISHTF_PASS();
}

void PageTests::InsertTest2(FileComparisonTest& test)
{
    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageTests_InsertTest2.dpdb");
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "PageTests_InsertTest2.dpdb");

    DiplodocusDB::Page page(0);

    Ishiko::Error error(0);
    std::ifstream input(inputPath.c_str(), std::fstream::binary);
    page.read(input, error);

    ISHTF_ABORT_IF((bool)error);
       
    page.insert("value0", 6, 0, error);
            
    ISHTF_FAIL_IF((bool)error);
    
    std::ofstream output(outputPath.c_str(), std::fstream::binary);
    page.write(output, error);
 
    ISHTF_FAIL_IF((bool)error);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PageTests_InsertTest2.dpdb");

    ISHTF_PASS();
}

void PageTests::EraseTest1(FileComparisonTest& test)
{
    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageTests_EraseTest1.dpdb");
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "PageTests_EraseTest1.dpdb");

    DiplodocusDB::Page page(0);

    Ishiko::Error error(0);
    std::ifstream input(inputPath.c_str(), std::fstream::binary);
    page.read(input, error);

    ISHTF_ABORT_IF((bool)error);
    
    page.erase(0, 6, error);

    ISHTF_FAIL_IF((bool)error);
    ISHTF_FAIL_UNLESS(page.dataSize() == 0);
    ISHTF_FAIL_UNLESS(page.availableSpace() == 4080);
        
    std::ofstream output(outputPath.c_str(), std::fstream::binary);
    page.write(output, error);

    ISHTF_FAIL_IF((bool)error);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PageTests_EraseTest1.dpdb");

    ISHTF_PASS();
}

void PageTests::EraseTest2(FileComparisonTest& test)
{
    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageTests_EraseTest2.dpdb");
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "PageTests_EraseTest2.dpdb");

    DiplodocusDB::Page page(0);

    Ishiko::Error error(0);
    std::ifstream input(inputPath.c_str(), std::fstream::binary);
    page.read(input, error);

    ISHTF_ABORT_IF((bool)error);
        
    page.erase(5, 1, error);

    ISHTF_FAIL_IF((bool)error);
    ISHTF_FAIL_UNLESS(page.dataSize() == 5);
    ISHTF_FAIL_UNLESS(page.availableSpace() == 4075);
                
    std::ofstream output(outputPath.c_str(), std::fstream::binary);
    page.write(output, error);

    ISHTF_FAIL_IF((bool)error);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PageTests_EraseTest2.dpdb");

    ISHTF_PASS();
}

void PageTests::EraseTest3(FileComparisonTest& test)
{
    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageTests_EraseTest3.dpdb");
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "PageTests_EraseTest3.dpdb");

    DiplodocusDB::Page page(0);

    Ishiko::Error error(0);
    std::ifstream input(inputPath.c_str(), std::fstream::binary);
    page.read(input, error);

    ISHTF_ABORT_IF((bool)error);
    
    page.erase(2, 10, error);

    ISHTF_FAIL_IF((bool)error);
    ISHTF_FAIL_UNLESS(page.dataSize() == 2);
    ISHTF_FAIL_UNLESS(page.availableSpace() == 4078);
    
    std::ofstream output(outputPath.c_str(), std::fstream::binary);
    page.write(output, error);

    ISHTF_FAIL_IF((bool)error);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PageTests_EraseTest3.dpdb");

    ISHTF_PASS();
}

void PageTests::MoveToTest1(FileComparisonTest& test)
{
    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageTests_MoveToTest1.dpdb");
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "PageTests_MoveToTest1.dpdb");

    DiplodocusDB::Page page1(0);

    Ishiko::Error error(0);
    std::ifstream input(inputPath.c_str(), std::fstream::binary);
    page1.read(input, error);

    ISHTF_ABORT_IF((bool)error);
    
    DiplodocusDB::Page page2(1);
    page2.read(input, error);
    
    page1.moveTo(0, 6, page2, error);

    ISHTF_FAIL_IF((bool)error);
    ISHTF_FAIL_UNLESS(page1.dataSize() == 0);
    ISHTF_FAIL_UNLESS(page2.dataSize() == 6);
    
    std::ofstream output(outputPath.c_str(), std::fstream::binary);
    page1.write(output, error);

    ISHTF_FAIL_IF((bool)error);
    
    page2.write(output, error);
    
    ISHTF_FAIL_IF((bool)error);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PageTests_MoveToTest1.dpdb");

    ISHTF_PASS();
}

void PageTests::MoveToTest2(FileComparisonTest& test)
{
    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageTests_MoveToTest2.dpdb");
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "PageTests_MoveToTest2.dpdb");

    DiplodocusDB::Page page1(0);

    Ishiko::Error error(0);
    std::ifstream input(inputPath.c_str(), std::fstream::binary);
    page1.read(input, error);

    ISHTF_ABORT_IF((bool)error);

    DiplodocusDB::Page page2(1);
    page2.read(input, error);
    
    page1.moveTo(0, 6, page2, error);

    ISHTF_FAIL_IF((bool)error);
    ISHTF_FAIL_UNLESS(page1.dataSize() == 0);
    ISHTF_FAIL_UNLESS(page2.dataSize() == 12);
    
    std::ofstream output(outputPath.c_str(), std::fstream::binary);
    page1.write(output, error);

    ISHTF_FAIL_IF((bool)error);
            
    page2.write(output, error);
                
    ISHTF_FAIL_IF((bool)error);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PageTests_MoveToTest2.dpdb");

    ISHTF_PASS();
}
