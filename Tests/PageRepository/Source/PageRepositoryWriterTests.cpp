/*
    Copyright (c) 2018-2022 Xavier Leclercq

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

#include "PageRepositoryWriterTests.h"
#include "DiplodocusDB/PhysicalStorage/PageRepository/PageRepositoryWriter.h"
#include "DiplodocusDB/PhysicalStorage/PageRepository/PageFileRepository.h"
#include <boost/filesystem/operations.hpp>

using namespace Ishiko;

PageRepositoryWriterTests::PageRepositoryWriterTests(const TestNumber& number, const TestContext& context)
    : TestSequence(number, "PageRepositoryWriter tests", context)
{
    append<HeapAllocationErrorsTest>("Creation test 1", CreationTest1);
    append<HeapAllocationErrorsTest>("write test 1", WriteTest1);
    append<HeapAllocationErrorsTest>("write test 2", WriteTest2);
    append<HeapAllocationErrorsTest>("write test 3", WriteTest3);
    append<HeapAllocationErrorsTest>("write test 4", WriteTest4);
    append<HeapAllocationErrorsTest>("write test 5", WriteTest5);
    append<HeapAllocationErrorsTest>("write test 6", WriteTest6);
    append<HeapAllocationErrorsTest>("write test 7", WriteTest7);
    append<HeapAllocationErrorsTest>("write test 8", WriteTest8);
    append<HeapAllocationErrorsTest>("writeLEB128 test 1", WriteLEB128Test1);
    append<HeapAllocationErrorsTest>("writeLEB128 test 2", WriteLEB128Test2);
    append<HeapAllocationErrorsTest>("writeLEB128 test 3", WriteLEB128Test3);
    append<HeapAllocationErrorsTest>("writeLEB128 test 4", WriteLEB128Test4);
    append<HeapAllocationErrorsTest>("writeLEB128 test 5", WriteLEB128Test5);
}

void PageRepositoryWriterTests::CreationTest1(Test& test)
{
    boost::filesystem::path inputPath = test.context().getDataPath("PageRepositoryWriterCreationTest1.dpdb");

    Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.open(inputPath, error);

    ISHTF_ABORT_IF((bool)error);
    
    std::shared_ptr<DiplodocusDB::Page> page = repository.page(0, error);

    ISHTF_ABORT_IF((bool)error);
    
    DiplodocusDB::PageRepositoryWriter writer(repository, page, 0);

    ISHTF_FAIL_UNLESS(writer.currentPosition().page() == 0);
    ISHTF_FAIL_UNLESS(writer.currentPosition().offset() == 0);
    ISHIKO_TEST_PASS();
}

void PageRepositoryWriterTests::WriteTest1(Test& test)
{
    const char* outputName = "PageRepositoryWriterWriteTest1.dpdb";
    
    Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.create(test.context().getOutputPath(outputName), error);

    ISHIKO_TEST_ABORT_IF(error);
    
    std::shared_ptr<DiplodocusDB::Page> page = repository.allocatePage(error);

    ISHIKO_TEST_ABORT_IF(error);
    ISHIKO_TEST_ABORT_IF_NOT(page);
        
    DiplodocusDB::PageRepositoryWriter writer = repository.insert(page->index(), 0, error);

    ISHIKO_TEST_ABORT_IF(error);
            
    writer.write("value1", 6, error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(writer.currentPosition().page(), 0);
    ISHIKO_TEST_FAIL_IF_NEQ(writer.currentPosition().offset(), 6);
    
    writer.save(error);
    
    ISHIKO_TEST_FAIL_IF(error);

    repository.close();
    
    ISHIKO_TEST_FAIL_IF_OUTPUT_AND_REFERENCE_FILES_NEQ(outputName);
    ISHIKO_TEST_PASS();
}

void PageRepositoryWriterTests::WriteTest2(Test& test)
{
    const char* outputName = "PageRepositoryWriterWriteTest2.dpdb";
    
    Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.create(test.context().getOutputPath(outputName), error);

    ISHIKO_TEST_ABORT_IF(error);

    std::shared_ptr<DiplodocusDB::Page> page = repository.allocatePage(error);

    ISHIKO_TEST_ABORT_IF(error);
    ISHIKO_TEST_ABORT_IF_NOT(page);
    
    DiplodocusDB::PageRepositoryWriter writer = repository.insert(page->index(), 0, error);

    ISHIKO_TEST_ABORT_IF(error);
    
    writer.write("value1", 6, error);

    ISHIKO_TEST_FAIL_IF(error);
    
    writer.write("value2", 6, error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(writer.currentPosition().page(), 0);
    ISHIKO_TEST_FAIL_IF_NEQ(writer.currentPosition().offset(), 12);
        
    writer.save(error);
        
    ISHIKO_TEST_FAIL_IF(error);

    repository.close();

    ISHIKO_TEST_FAIL_IF_OUTPUT_AND_REFERENCE_FILES_NEQ(outputName);
    ISHIKO_TEST_PASS();
}

void PageRepositoryWriterTests::WriteTest3(Test& test)
{
    boost::filesystem::path inputPath(test.context().getTestDataDirectory()
        / "PageRepositoryWriterWriteTest3.dpdb");
    boost::filesystem::path outputPath(test.context().getTestOutputDirectory()
        / "PageRepositoryWriterWriteTest3.dpdb");

    boost::filesystem::copy_file(inputPath, outputPath, boost::filesystem::copy_option::overwrite_if_exists);

    Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.open(outputPath, error);

    ISHIKO_TEST_ABORT_IF(error);

    DiplodocusDB::PageRepositoryWriter writer = repository.insert(0, 0, error);

    ISHIKO_TEST_ABORT_IF(error);
    
    writer.write("value0", 6, error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(writer.currentPosition().page(), 0);
    ISHIKO_TEST_FAIL_IF_NEQ(writer.currentPosition().offset(), 6);
    
    writer.save(error);

    ISHIKO_TEST_FAIL_IF(error);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PageRepositoryWriterWriteTest3.dpdb");

    ISHIKO_TEST_PASS();
}

void PageRepositoryWriterTests::WriteTest4(Test& test)
{
    boost::filesystem::path inputPath(test.context().getTestDataDirectory() / "PageRepositoryWriterWriteTest4.dpdb");
    boost::filesystem::path outputPath(test.context().getTestOutputDirectory() / "PageRepositoryWriterWriteTest4.dpdb");

    boost::filesystem::copy_file(inputPath, outputPath, boost::filesystem::copy_option::overwrite_if_exists);

    Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.open(outputPath, error);

    ISHTF_ABORT_IF((bool)error);
    
    DiplodocusDB::PageRepositoryWriter writer = repository.insert(0, 6, error);

    ISHTF_ABORT_IF((bool)error);
    
    writer.write("value2", 6, error);

    ISHTF_FAIL_IF((bool)error);
    ISHTF_FAIL_UNLESS(writer.currentPosition().page() == 0);
    ISHTF_FAIL_UNLESS(writer.currentPosition().offset() == 12);

    writer.save(error);
    
    ISHTF_FAIL_IF((bool)error);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.context().getReferenceDataDirectory() / "PageRepositoryWriterWriteTest4.dpdb");

    ISHIKO_TEST_PASS();
}

void PageRepositoryWriterTests::WriteTest5(Test& test)
{
    boost::filesystem::path inputPath(test.context().getTestDataDirectory() / "PageRepositoryWriterWriteTest5.dpdb");
    boost::filesystem::path outputPath(test.context().getTestOutputDirectory() / "PageRepositoryWriterWriteTest5.dpdb");

    boost::filesystem::copy_file(inputPath, outputPath, boost::filesystem::copy_option::overwrite_if_exists);

    Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.open(outputPath, error);

    ISHIKO_TEST_ABORT_IF(error);
    
    DiplodocusDB::PageRepositoryWriter writer = repository.insert(0, 0, error);

    ISHIKO_TEST_ABORT_IF(error);
    
    for (size_t i = 0; i < 409; ++i)
    {
        writer.write("0123456789", 10, error);
    }

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(writer.currentPosition().page(), 1);
    ISHIKO_TEST_FAIL_IF_NEQ(writer.currentPosition().offset(), 10);

    writer.save(error);

    ISHIKO_TEST_FAIL_IF(error);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.context().getReferenceDataDirectory() / "PageRepositoryWriterWriteTest5.dpdb");

    ISHIKO_TEST_PASS();
}

void PageRepositoryWriterTests::WriteTest6(Test& test)
{
    boost::filesystem::path inputPath(test.context().getTestDataDirectory() / "PageRepositoryWriterWriteTest6.dpdb");
    boost::filesystem::path outputPath(test.context().getTestOutputDirectory() / "PageRepositoryWriterWriteTest6.dpdb");

    boost::filesystem::copy_file(inputPath, outputPath, boost::filesystem::copy_option::overwrite_if_exists);

    Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.open(outputPath, error);

    ISHIKO_TEST_ABORT_IF(error);
    
    DiplodocusDB::PageRepositoryWriter writer = repository.insert(0, 0, error);

    ISHIKO_TEST_ABORT_IF(error);
    
    for (size_t i = 0; i < 409; ++i)
    {
        writer.write("01234567890123456789", 20, error);
    }

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(writer.currentPosition().page(), 2);
    ISHIKO_TEST_FAIL_IF_NEQ(writer.currentPosition().offset(), 20);

    writer.save(error);
    
    ISHIKO_TEST_FAIL_IF(error);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PageRepositoryWriterWriteTest6.dpdb");

    ISHIKO_TEST_PASS();
}

void PageRepositoryWriterTests::WriteTest7(Test& test)
{
    const char* outputName = "PageRepositoryWriterWriteTest7.dpdb";

    boost::filesystem::copy_file(test.context().getDataPath("PageRepositoryWriterWriteTest7.dpdb"), outputPath,
        boost::filesystem::copy_option::overwrite_if_exists);

    Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.open(test.context().getOutputPath(outputName), error);

    ISHIKO_TEST_ABORT_IF(error);

    DiplodocusDB::PageRepositoryWriter writer = repository.insert(0, 0, error);

    ISHIKO_TEST_ABORT_IF(error);

    for (size_t i = 0; i < 407; ++i)
    {
        writer.write("0123456789", 10, error);
    }
    writer.write("01234567890", 11, error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(writer.currentPosition().page(), 1);
    ISHIKO_TEST_FAIL_IF_NEQ(writer.currentPosition().offset(), 1);

    writer.save(error);
    
    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_OUTPUT_AND_REFERENCE_FILES_NEQ(outputName);
    ISHIKO_TEST_PASS();
}

void PageRepositoryWriterTests::WriteTest8(Test& test)
{
    const char* outputName = "PageRepositoryWriterWriteTest8.dpdb";

    boost::filesystem::copy_file(test.context().getDataPath("PageRepositoryWriterWriteTest8.dpdb"), outputPath,
        boost::filesystem::copy_option::overwrite_if_exists);

    Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.open(test.context().getOutputPath(outputName), error);

    ISHIKO_TEST_ABORT_IF(error);

    DiplodocusDB::PageRepositoryWriter writer = repository.insert(0, 0, error);

    ISHIKO_TEST_ABORT_IF(error);
    
    for (size_t i = 0; i < 409; ++i)
    {
        writer.write("0123456789", 10, error);
    }

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(writer.currentPosition().page(), 1);
    ISHIKO_TEST_FAIL_IF_NEQ(writer.currentPosition().offset(), 10);

    writer.save(error);
    
    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_OUTPUT_AND_REFERENCE_FILES_NEQ(outputName);
    ISHIKO_TEST_PASS();
}

void PageRepositoryWriterTests::WriteLEB128Test1(Test& test)
{
    const char* outputName = "PageRepositoryWriterWriteLEB128Test1.dpdb";
   
    Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.create(test.context().getOutputPath(outputName), error);

    ISHIKO_TEST_ABORT_IF(error);

    std::shared_ptr<DiplodocusDB::Page> page = repository.allocatePage(error);

    ISHIKO_TEST_ABORT_IF(error);
    ISHIKO_TEST_ABORT_IF_NOT(page);

    DiplodocusDB::PageRepositoryWriter writer = repository.insert(page->index(), 0, error);

    ISHIKO_TEST_ABORT_IF(error);

    writer.writeLEB128(0, error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(writer.currentPosition().page(), 0);
    ISHIKO_TEST_FAIL_IF_NEQ(writer.currentPosition().offset(), 1);

    writer.save(error);

    ISHIKO_TEST_FAIL_IF(error);

    repository.close();

    ISHIKO_TEST_FAIL_IF_OUTPUT_AND_REFERENCE_FILES_NEQ(outputName);
    ISHIKO_TEST_PASS();
}

void PageRepositoryWriterTests::WriteLEB128Test2(Test& test)
{
    boost::filesystem::path outputPath(test.context().getTestOutputDirectory()
        / "PageRepositoryWriterWriteLEB128Test2.dpdb");

    Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.create(outputPath, error);

    ISHIKO_TEST_ABORT_IF(error);

    std::shared_ptr<DiplodocusDB::Page> page = repository.allocatePage(error);

    ISHIKO_TEST_ABORT_IF(error);
    ISHIKO_TEST_ABORT_IF_NOT(page);

    DiplodocusDB::PageRepositoryWriter writer = repository.insert(page->index(), 0, error);

    ISHIKO_TEST_ABORT_IF(error);

    writer.writeLEB128(1, error);

    ISHIKO_TEST_FAIL_IF((bool)error);
    ISHIKO_TEST_FAIL_IF_NEQ(writer.currentPosition().page(), 0);
    ISHIKO_TEST_FAIL_IF_NEQ(writer.currentPosition().offset(), 1);

    writer.save(error);

    ISHIKO_TEST_FAIL_IF(error);

    repository.close();

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.context().getReferenceDataDirectory()
        / "PageRepositoryWriterWriteLEB128Test2.dpdb");

    ISHIKO_TEST_PASS();
}

void PageRepositoryWriterTests::WriteLEB128Test3(Test& test)
{
    boost::filesystem::path outputPath(test.context().getTestOutputDirectory()
        / "PageRepositoryWriterWriteLEB128Test3.dpdb");

    Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.create(outputPath, error);

    ISHIKO_TEST_ABORT_IF(error);

    std::shared_ptr<DiplodocusDB::Page> page = repository.allocatePage(error);

    ISHIKO_TEST_ABORT_IF(error);
    ISHIKO_TEST_ABORT_IF_NOT(page);

    DiplodocusDB::PageRepositoryWriter writer = repository.insert(page->index(), 0, error);

    ISHIKO_TEST_ABORT_IF(error);

    writer.writeLEB128(127, error);

    ISHIKO_TEST_FAIL_IF(error);
    ISHIKO_TEST_FAIL_IF_NEQ(writer.currentPosition().page(), 0);
    ISHIKO_TEST_FAIL_IF_NEQ(writer.currentPosition().offset(), 1);

    writer.save(error);

    ISHTF_FAIL_IF((bool)error);

    repository.close();

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory()
        / "PageRepositoryWriterWriteLEB128Test3.dpdb");

    ISHIKO_TEST_PASS();
}

void PageRepositoryWriterTests::WriteLEB128Test4(Test& test)
{
    boost::filesystem::path outputPath(test.context().getTestOutputDirectory()
        / "PageRepositoryWriterWriteLEB128Test4.dpdb");

    Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.create(outputPath, error);

    ISHIKO_TEST_ABORT_IF(error);

    std::shared_ptr<DiplodocusDB::Page> page = repository.allocatePage(error);

    ISHIKO_TEST_ABORT_IF(error);
    ISHIKO_TEST_ABORT_IF_NOT(page);

    DiplodocusDB::PageRepositoryWriter writer = repository.insert(page->index(), 0, error);

    ISHTF_ABORT_IF((bool)error);

    writer.writeLEB128(128, error);

    ISHTF_FAIL_IF((bool)error);
    ISHTF_FAIL_UNLESS(writer.currentPosition().page() == 0);
    ISHTF_FAIL_UNLESS(writer.currentPosition().offset() == 2);

    writer.save(error);

    ISHTF_FAIL_IF((bool)error);

    repository.close();

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory()
        / "PageRepositoryWriterWriteLEB128Test4.dpdb");

    ISHIKO_TEST_PASS();
}

void PageRepositoryWriterTests::WriteLEB128Test5(Test& test)
{
    boost::filesystem::path outputPath(test.context().getTestOutputDirectory()
        / "PageRepositoryWriterWriteLEB128Test5.dpdb");

    Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.create(outputPath, error);

    ISHTF_ABORT_IF((bool)error);

    std::shared_ptr<DiplodocusDB::Page> page = repository.allocatePage(error);

    ISHTF_ABORT_IF((bool)error);
    ISHTF_ABORT_UNLESS(page);

    DiplodocusDB::PageRepositoryWriter writer = repository.insert(page->index(), 0, error);

    ISHTF_ABORT_IF((bool)error);

    writer.writeLEB128(16384, error);

    ISHTF_FAIL_IF((bool)error);
    ISHTF_FAIL_UNLESS(writer.currentPosition().page() == 0);
    ISHTF_FAIL_UNLESS(writer.currentPosition().offset() == 3);

    writer.save(error);

    ISHTF_FAIL_IF((bool)error);

    repository.close();

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory()
        / "PageRepositoryWriterWriteLEB128Test5.dpdb");

    ISHIKO_TEST_PASS();
}
