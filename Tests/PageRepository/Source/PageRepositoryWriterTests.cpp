/*
    Copyright (c) 2018 Xavier Leclercq

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

void AddPageRepositoryWriterTests(TestHarness& theTestHarness)
{
    TestSequence& writerTestSequence = theTestHarness.appendTestSequence("PageRepositoryWriter tests");

    new HeapAllocationErrorsTest("Creation test 1", PageRepositoryWriterCreationTest1, writerTestSequence);

    new FileComparisonTest("write test 1", PageRepositoryWriterWriteTest1, writerTestSequence);
    new FileComparisonTest("write test 2", PageRepositoryWriterWriteTest2, writerTestSequence);
    new FileComparisonTest("write test 3", PageRepositoryWriterWriteTest3, writerTestSequence);
    new FileComparisonTest("write test 4", PageRepositoryWriterWriteTest4, writerTestSequence);
    new FileComparisonTest("write test 5", PageRepositoryWriterWriteTest5, writerTestSequence);
    new FileComparisonTest("write test 6", PageRepositoryWriterWriteTest6, writerTestSequence);
    new FileComparisonTest("write test 7", PageRepositoryWriterWriteTest7, writerTestSequence);
    new FileComparisonTest("write test 8", PageRepositoryWriterWriteTest8, writerTestSequence);
}

TestResult::EOutcome PageRepositoryWriterCreationTest1(Test& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageRepositoryWriterCreationTest1.dpdb");

    Ishiko::Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.open(inputPath, error);
    if (!error)
    {
        std::shared_ptr<DiplodocusDB::Page> page = repository.page(0, error);
        if (!error)
        {
            DiplodocusDB::PageRepositoryWriter writer(repository, page, 0);
            if (writer.currentPageOffset() == 0)
            {
                result = TestResult::ePassed;
            }
        }
    }

    return result;
}

TestResult::EOutcome PageRepositoryWriterWriteTest1(FileComparisonTest& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "PageRepositoryWriterWriteTest1.dpdb");

    Ishiko::Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.create(outputPath, error);
    if (!error)
    {
        std::shared_ptr<DiplodocusDB::Page> page = repository.allocatePage(error);
        if (!error)
        {
            DiplodocusDB::PageRepositoryWriter writer = repository.insert(page->index(), 0, error);
            if (!error)
            {
                writer.write("value1", 6, error);
                if (writer.currentPageOffset() == 6)
                {
                    if (!error)
                    {
                        writer.save(error);
                        if (!error)
                        {
                            result = TestResult::ePassed;
                        }
                    }
                }
            }
        }
    }
    repository.close();

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PageRepositoryWriterWriteTest1.dpdb");

    return result;
}

TestResult::EOutcome PageRepositoryWriterWriteTest2(FileComparisonTest& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "PageRepositoryWriterWriteTest2.dpdb");

    Ishiko::Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.create(outputPath, error);
    if (!error)
    {
        std::shared_ptr<DiplodocusDB::Page> page = repository.allocatePage(error);
        if (!error)
        {
            DiplodocusDB::PageRepositoryWriter writer = repository.insert(page->index(), 0, error);
            if (!error)
            {
                writer.write("value1", 6, error);
                if (!error)
                {
                    writer.write("value2", 6, error);
                    if (!error)
                    {
                        if (writer.currentPageOffset() == 12)
                        {
                            writer.save(error);
                            if (!error)
                            {
                                result = TestResult::ePassed;
                            }
                        }
                    }
                }
            }
        }
    }
    repository.close();

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PageRepositoryWriterWriteTest2.dpdb");

    return result;
}

TestResult::EOutcome PageRepositoryWriterWriteTest3(FileComparisonTest& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageRepositoryWriterWriteTest3.dpdb");
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "PageRepositoryWriterWriteTest3.dpdb");

    boost::filesystem::copy_file(inputPath, outputPath, boost::filesystem::copy_option::overwrite_if_exists);

    Ishiko::Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.open(outputPath, error);
    if (!error)
    {
        DiplodocusDB::PageRepositoryWriter writer = repository.insert(0, 0, error);
        if (!error)
        {
            writer.write("value0", 6, error);
            if (!error)
            {
                if (writer.currentPageOffset() == 6)
                {
                    writer.save(error);
                    if (!error)
                    {
                        result = TestResult::ePassed;
                    }
                }
            }
        }
    }

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PageRepositoryWriterWriteTest3.dpdb");

    return result;
}

TestResult::EOutcome PageRepositoryWriterWriteTest4(FileComparisonTest& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageRepositoryWriterWriteTest4.dpdb");
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "PageRepositoryWriterWriteTest4.dpdb");

    boost::filesystem::copy_file(inputPath, outputPath, boost::filesystem::copy_option::overwrite_if_exists);

    Ishiko::Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.open(outputPath, error);
    if (!error)
    {
        DiplodocusDB::PageRepositoryWriter writer = repository.insert(0, 6, error);
        if (!error)
        {
            writer.write("value2", 6, error);
            if (!error)
            {
                if (writer.currentPageOffset() == 12)
                {
                    writer.save(error);
                    if (!error)
                    {
                        result = TestResult::ePassed;
                    }
                }
            }
        }
    }

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PageRepositoryWriterWriteTest4.dpdb");

    return result;
}

TestResult::EOutcome PageRepositoryWriterWriteTest5(FileComparisonTest& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageRepositoryWriterWriteTest5.dpdb");
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "PageRepositoryWriterWriteTest5.dpdb");

    boost::filesystem::copy_file(inputPath, outputPath, boost::filesystem::copy_option::overwrite_if_exists);

    Ishiko::Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.open(outputPath, error);
    if (!error)
    {
        DiplodocusDB::PageRepositoryWriter writer = repository.insert(0, 0, error);
        if (!error)
        {
            for (size_t i = 0; i < 409; ++i)
            {
                writer.write("0123456789", 10, error);
            }
            if (!error)
            {
                if (writer.currentPageOffset() == 10)
                {
                    writer.save(error);
                    if (!error)
                    {
                        result = TestResult::ePassed;
                    }
                }
            }
        }
    }

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PageRepositoryWriterWriteTest5.dpdb");

    return result;
}

TestResult::EOutcome PageRepositoryWriterWriteTest6(FileComparisonTest& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageRepositoryWriterWriteTest6.dpdb");
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "PageRepositoryWriterWriteTest6.dpdb");

    boost::filesystem::copy_file(inputPath, outputPath, boost::filesystem::copy_option::overwrite_if_exists);

    Ishiko::Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.open(outputPath, error);
    if (!error)
    {
        DiplodocusDB::PageRepositoryWriter writer = repository.insert(0, 0, error);
        if (!error)
        {
            for (size_t i = 0; i < 409; ++i)
            {
                writer.write("01234567890123456789", 20, error);
            }
            if (!error)
            {
                if (writer.currentPageOffset() == 20)
                {
                    writer.save(error);
                    if (!error)
                    {
                        result = TestResult::ePassed;
                    }
                }
            }
        }
    }

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PageRepositoryWriterWriteTest6.dpdb");

    return result;
}

TestResult::EOutcome PageRepositoryWriterWriteTest7(FileComparisonTest& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageRepositoryWriterWriteTest7.dpdb");
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "PageRepositoryWriterWriteTest7.dpdb");

    boost::filesystem::copy_file(inputPath, outputPath, boost::filesystem::copy_option::overwrite_if_exists);

    Ishiko::Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.open(outputPath, error);
    if (!error)
    {
        DiplodocusDB::PageRepositoryWriter writer = repository.insert(0, 0, error);
        if (!error)
        {
            for (size_t i = 0; i < 407; ++i)
            {
                writer.write("0123456789", 10, error);
            }
            writer.write("01234567890", 11, error);
            if (!error)
            {
                if (writer.currentPageOffset() == 1)
                {
                    writer.save(error);
                    if (!error)
                    {
                        result = TestResult::ePassed;
                    }
                }
            }
        }
    }

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PageRepositoryWriterWriteTest7.dpdb");

    return result;
}

TestResult::EOutcome PageRepositoryWriterWriteTest8(FileComparisonTest& test)
{
    TestResult::EOutcome result = TestResult::eFailed;

    boost::filesystem::path inputPath(test.environment().getTestDataDirectory() / "PageRepositoryWriterWriteTest8.dpdb");
    boost::filesystem::path outputPath(test.environment().getTestOutputDirectory() / "PageRepositoryWriterWriteTest8.dpdb");

    boost::filesystem::copy_file(inputPath, outputPath, boost::filesystem::copy_option::overwrite_if_exists);

    Ishiko::Error error;

    DiplodocusDB::PageFileRepository repository;
    repository.open(outputPath, error);
    if (!error)
    {
        DiplodocusDB::PageRepositoryWriter writer = repository.insert(0, 0, error);
        if (!error)
        {
            for (size_t i = 0; i < 409; ++i)
            {
                writer.write("0123456789", 10, error);
            }
            if (!error)
            {
                if (writer.currentPageOffset() == 10)
                {
                    writer.save(error);
                    if (!error)
                    {
                        result = TestResult::ePassed;
                    }
                }
            }
        }
    }

    test.setOutputFilePath(outputPath);
    test.setReferenceFilePath(test.environment().getReferenceDataDirectory() / "PageRepositoryWriterWriteTest8.dpdb");

    return result;
}
