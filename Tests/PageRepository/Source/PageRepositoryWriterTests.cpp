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

#include "PageRepositoryWriterTests.h"
#include "DiplodocusDB/PhysicalStorage/PageRepository/PageRepositoryWriter.h"
#include "DiplodocusDB/PhysicalStorage/PageRepository/PageFileRepository.h"
#include <boost/filesystem/operations.hpp>

using namespace Ishiko::Tests;

PageRepositoryWriterTests::PageRepositoryWriterTests(const TestNumber& number, const TestEnvironment& environment)
    : TestSequence(number, "PageRepositoryWriter tests", environment)
{
    append<HeapAllocationErrorsTest>("Creation test 1", CreationTest1);
    append<FileComparisonTest>("write test 1", WriteTest1);
    append<FileComparisonTest>("write test 2", WriteTest2);
    append<FileComparisonTest>("write test 3", WriteTest3);
    append<FileComparisonTest>("write test 4", WriteTest4);
    append<FileComparisonTest>("write test 5", WriteTest5);
    append<FileComparisonTest>("write test 6", WriteTest6);
    append<FileComparisonTest>("write test 7", WriteTest7);
    append<FileComparisonTest>("write test 8", WriteTest8);
    append<FileComparisonTest>("writeLEB128 test 1", WriteLEB128Test1);
    append<FileComparisonTest>("writeLEB128 test 2", WriteLEB128Test2);
    append<FileComparisonTest>("writeLEB128 test 3", WriteLEB128Test3);
    append<FileComparisonTest>("writeLEB128 test 4", WriteLEB128Test4);
    append<FileComparisonTest>("writeLEB128 test 5", WriteLEB128Test5);
}

void PageRepositoryWriterTests::CreationTest1(Test& test)
{
    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageRepositoryWriterCreationTest1.dpdb");

    Ishiko::Error error(0);

    DiplodocusDB::PageFileRepository repository;
    repository.open(inputPath, error);

    ISHTF_ABORT_IF((bool)error);
    
    std::shared_ptr<DiplodocusDB::Page> page = repository.page(0, error);

    ISHTF_ABORT_IF((bool)error);
    
    DiplodocusDB::PageRepositoryWriter writer(repository, page, 0);

    ISHTF_FAIL_UNLESS(writer.currentPageOffset() == 0);
    ISHTF_PASS();
}

void PageRepositoryWriterTests::WriteTest1(FileComparisonTest& test)
{
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory()
        / "PageRepositoryWriterWriteTest1.dpdb");

    Ishiko::Error error(0);

    DiplodocusDB::PageFileRepository repository;
    repository.create(outputPath, error);

    ISHTF_ABORT_IF((bool)error);
    
    std::shared_ptr<DiplodocusDB::Page> page = repository.allocatePage(error);

    ISHTF_ABORT_IF((bool)error);
    ISHTF_ABORT_UNLESS(page);
        
    DiplodocusDB::PageRepositoryWriter writer = repository.insert(page->index(), 0, error);

    ISHTF_ABORT_IF((bool)error);
            
    writer.write("value1", 6, error);

    ISHTF_FAIL_IF((bool)error);
    ISHTF_FAIL_UNLESS(writer.currentPageOffset() == 6);
    
    writer.save(error);
    
    ISHTF_FAIL_IF((bool)error);

    repository.close();

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PageRepositoryWriterWriteTest1.dpdb");

    ISHTF_PASS();
}

void PageRepositoryWriterTests::WriteTest2(FileComparisonTest& test)
{
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory()
        / "PageRepositoryWriterWriteTest2.dpdb");

    Ishiko::Error error(0);

    DiplodocusDB::PageFileRepository repository;
    repository.create(outputPath, error);

    ISHTF_ABORT_IF((bool)error);

    std::shared_ptr<DiplodocusDB::Page> page = repository.allocatePage(error);

    ISHTF_ABORT_IF((bool)error);
    ISHTF_ABORT_UNLESS(page);
    
    DiplodocusDB::PageRepositoryWriter writer = repository.insert(page->index(), 0, error);

    ISHTF_ABORT_IF((bool)error);
    
    writer.write("value1", 6, error);

    ISHTF_FAIL_IF((bool)error);
    
    writer.write("value2", 6, error);

    ISHTF_FAIL_IF((bool)error);
    ISHTF_FAIL_UNLESS(writer.currentPageOffset() == 12);
        
    writer.save(error);
        
    ISHTF_FAIL_IF((bool)error);

    repository.close();

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PageRepositoryWriterWriteTest2.dpdb");

    ISHTF_PASS();
}

void PageRepositoryWriterTests::WriteTest3(FileComparisonTest& test)
{
    boost::filesystem::path inputPath(test.environment().getTestDataDirectory()
        / "PageRepositoryWriterWriteTest3.dpdb");
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory()
        / "PageRepositoryWriterWriteTest3.dpdb");

    boost::filesystem::copy_file(inputPath, outputPath, boost::filesystem::copy_option::overwrite_if_exists);

    Ishiko::Error error(0);

    DiplodocusDB::PageFileRepository repository;
    repository.open(outputPath, error);

    ISHTF_ABORT_IF((bool)error);

    DiplodocusDB::PageRepositoryWriter writer = repository.insert(0, 0, error);

    ISHTF_ABORT_IF((bool)error);
    
    writer.write("value0", 6, error);

    ISHTF_FAIL_IF((bool)error);
    ISHTF_FAIL_UNLESS(writer.currentPageOffset() == 6);
    
    writer.save(error);

    ISHTF_FAIL_IF((bool)error);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PageRepositoryWriterWriteTest3.dpdb");

    ISHTF_PASS();
}

void PageRepositoryWriterTests::WriteTest4(FileComparisonTest& test)
{
    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageRepositoryWriterWriteTest4.dpdb");
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "PageRepositoryWriterWriteTest4.dpdb");

    boost::filesystem::copy_file(inputPath, outputPath, boost::filesystem::copy_option::overwrite_if_exists);

    Ishiko::Error error(0);

    DiplodocusDB::PageFileRepository repository;
    repository.open(outputPath, error);

    ISHTF_ABORT_IF((bool)error);
    
    DiplodocusDB::PageRepositoryWriter writer = repository.insert(0, 6, error);

    ISHTF_ABORT_IF((bool)error);
    
    writer.write("value2", 6, error);

    ISHTF_FAIL_IF((bool)error);
    ISHTF_FAIL_UNLESS(writer.currentPageOffset() == 12);
    
    writer.save(error);
    
    ISHTF_FAIL_IF((bool)error);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PageRepositoryWriterWriteTest4.dpdb");

    ISHTF_PASS();
}

void PageRepositoryWriterTests::WriteTest5(FileComparisonTest& test)
{
    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageRepositoryWriterWriteTest5.dpdb");
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "PageRepositoryWriterWriteTest5.dpdb");

    boost::filesystem::copy_file(inputPath, outputPath, boost::filesystem::copy_option::overwrite_if_exists);

    Ishiko::Error error(0);

    DiplodocusDB::PageFileRepository repository;
    repository.open(outputPath, error);

    ISHTF_ABORT_IF((bool)error);
    
    DiplodocusDB::PageRepositoryWriter writer = repository.insert(0, 0, error);

    ISHTF_ABORT_IF((bool)error);
    
    for (size_t i = 0; i < 409; ++i)
    {
        writer.write("0123456789", 10, error);
    }

    ISHTF_FAIL_IF((bool)error);
    ISHTF_FAIL_UNLESS(writer.currentPageOffset() == 10);
    
    writer.save(error);

    ISHTF_FAIL_IF((bool)error);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PageRepositoryWriterWriteTest5.dpdb");

    ISHTF_PASS();
}

void PageRepositoryWriterTests::WriteTest6(FileComparisonTest& test)
{
    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageRepositoryWriterWriteTest6.dpdb");
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "PageRepositoryWriterWriteTest6.dpdb");

    boost::filesystem::copy_file(inputPath, outputPath, boost::filesystem::copy_option::overwrite_if_exists);

    Ishiko::Error error(0);

    DiplodocusDB::PageFileRepository repository;
    repository.open(outputPath, error);

    ISHTF_ABORT_IF((bool)error);
    
    DiplodocusDB::PageRepositoryWriter writer = repository.insert(0, 0, error);

    ISHTF_ABORT_IF((bool)error);
    
    for (size_t i = 0; i < 409; ++i)
    {
        writer.write("01234567890123456789", 20, error);
    }

    ISHTF_FAIL_IF((bool)error);
    ISHTF_FAIL_UNLESS(writer.currentPageOffset() == 20);
    
    writer.save(error);
    
    ISHTF_FAIL_IF((bool)error);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PageRepositoryWriterWriteTest6.dpdb");

    ISHTF_PASS();
}

void PageRepositoryWriterTests::WriteTest7(FileComparisonTest& test)
{
    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() 
        / "PageRepositoryWriterWriteTest7.dpdb");
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory()
        / "PageRepositoryWriterWriteTest7.dpdb");

    boost::filesystem::copy_file(inputPath, outputPath, boost::filesystem::copy_option::overwrite_if_exists);

    Ishiko::Error error(0);

    DiplodocusDB::PageFileRepository repository;
    repository.open(outputPath, error);

    ISHTF_ABORT_IF((bool)error);

    DiplodocusDB::PageRepositoryWriter writer = repository.insert(0, 0, error);

    ISHTF_ABORT_IF((bool)error);

    for (size_t i = 0; i < 407; ++i)
    {
        writer.write("0123456789", 10, error);
    }
    writer.write("01234567890", 11, error);

    ISHTF_FAIL_IF((bool)error);
    ISHTF_FAIL_UNLESS(writer.currentPageOffset() == 1);
    
    writer.save(error);
    
    ISHTF_FAIL_IF((bool)error);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PageRepositoryWriterWriteTest7.dpdb");

    ISHTF_PASS();
}

void PageRepositoryWriterTests::WriteTest8(FileComparisonTest& test)
{
    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageRepositoryWriterWriteTest8.dpdb");
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "PageRepositoryWriterWriteTest8.dpdb");

    boost::filesystem::copy_file(inputPath, outputPath, boost::filesystem::copy_option::overwrite_if_exists);

    Ishiko::Error error(0);

    DiplodocusDB::PageFileRepository repository;
    repository.open(outputPath, error);

    ISHTF_ABORT_IF((bool)error);

    DiplodocusDB::PageRepositoryWriter writer = repository.insert(0, 0, error);

    ISHTF_ABORT_IF((bool)error);
    
    for (size_t i = 0; i < 409; ++i)
    {
        writer.write("0123456789", 10, error);
    }

    ISHTF_FAIL_IF((bool)error);
    ISHTF_FAIL_UNLESS(writer.currentPageOffset() == 10);
    
    writer.save(error);
    
    ISHTF_FAIL_IF((bool)error);

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PageRepositoryWriterWriteTest8.dpdb");

    ISHTF_PASS();
}

void PageRepositoryWriterTests::WriteLEB128Test1(FileComparisonTest& test)
{
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory()
        / "PageRepositoryWriterWriteLEB128Test1.dpdb");

    Ishiko::Error error(0);

    DiplodocusDB::PageFileRepository repository;
    repository.create(outputPath, error);

    ISHTF_ABORT_IF((bool)error);

    std::shared_ptr<DiplodocusDB::Page> page = repository.allocatePage(error);

    ISHTF_ABORT_IF((bool)error);
    ISHTF_ABORT_UNLESS(page);

    DiplodocusDB::PageRepositoryWriter writer = repository.insert(page->index(), 0, error);

    ISHTF_ABORT_IF((bool)error);

    writer.writeLEB128(0, error);

    ISHTF_FAIL_IF((bool)error);
    ISHTF_FAIL_UNLESS(writer.currentPageOffset() == 1);

    writer.save(error);

    ISHTF_FAIL_IF((bool)error);

    repository.close();

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory()
        / "PageRepositoryWriterWriteLEB128Test1.dpdb");

    ISHTF_PASS();
}

void PageRepositoryWriterTests::WriteLEB128Test2(FileComparisonTest& test)
{
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory()
        / "PageRepositoryWriterWriteLEB128Test2.dpdb");

    Ishiko::Error error(0);

    DiplodocusDB::PageFileRepository repository;
    repository.create(outputPath, error);

    ISHTF_ABORT_IF((bool)error);

    std::shared_ptr<DiplodocusDB::Page> page = repository.allocatePage(error);

    ISHTF_ABORT_IF((bool)error);
    ISHTF_ABORT_UNLESS(page);

    DiplodocusDB::PageRepositoryWriter writer = repository.insert(page->index(), 0, error);

    ISHTF_ABORT_IF((bool)error);

    writer.writeLEB128(1, error);

    ISHTF_FAIL_IF((bool)error);
    ISHTF_FAIL_UNLESS(writer.currentPageOffset() == 1);

    writer.save(error);

    ISHTF_FAIL_IF((bool)error);

    repository.close();

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory()
        / "PageRepositoryWriterWriteLEB128Test2.dpdb");

    ISHTF_PASS();
}

void PageRepositoryWriterTests::WriteLEB128Test3(FileComparisonTest& test)
{
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory()
        / "PageRepositoryWriterWriteLEB128Test3.dpdb");

    Ishiko::Error error(0);

    DiplodocusDB::PageFileRepository repository;
    repository.create(outputPath, error);

    ISHTF_ABORT_IF((bool)error);

    std::shared_ptr<DiplodocusDB::Page> page = repository.allocatePage(error);

    ISHTF_ABORT_IF((bool)error);
    ISHTF_ABORT_UNLESS(page);

    DiplodocusDB::PageRepositoryWriter writer = repository.insert(page->index(), 0, error);

    ISHTF_ABORT_IF((bool)error);

    writer.writeLEB128(127, error);

    ISHTF_FAIL_IF((bool)error);
    ISHTF_FAIL_UNLESS(writer.currentPageOffset() == 1);

    writer.save(error);

    ISHTF_FAIL_IF((bool)error);

    repository.close();

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory()
        / "PageRepositoryWriterWriteLEB128Test3.dpdb");

    ISHTF_PASS();
}

void PageRepositoryWriterTests::WriteLEB128Test4(FileComparisonTest& test)
{
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory()
        / "PageRepositoryWriterWriteLEB128Test4.dpdb");

    Ishiko::Error error(0);

    DiplodocusDB::PageFileRepository repository;
    repository.create(outputPath, error);

    ISHTF_ABORT_IF((bool)error);

    std::shared_ptr<DiplodocusDB::Page> page = repository.allocatePage(error);

    ISHTF_ABORT_IF((bool)error);
    ISHTF_ABORT_UNLESS(page);

    DiplodocusDB::PageRepositoryWriter writer = repository.insert(page->index(), 0, error);

    ISHTF_ABORT_IF((bool)error);

    writer.writeLEB128(128, error);

    ISHTF_FAIL_IF((bool)error);
    ISHTF_FAIL_UNLESS(writer.currentPageOffset() == 2);

    writer.save(error);

    ISHTF_FAIL_IF((bool)error);

    repository.close();

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory()
        / "PageRepositoryWriterWriteLEB128Test4.dpdb");

    ISHTF_PASS();
}

void PageRepositoryWriterTests::WriteLEB128Test5(FileComparisonTest& test)
{
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory()
        / "PageRepositoryWriterWriteLEB128Test5.dpdb");

    Ishiko::Error error(0);

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
    ISHTF_FAIL_UNLESS(writer.currentPageOffset() == 3);

    writer.save(error);

    ISHTF_FAIL_IF((bool)error);

    repository.close();

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory()
        / "PageRepositoryWriterWriteLEB128Test5.dpdb");

    ISHTF_PASS();
}
