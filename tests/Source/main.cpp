/*
    Copyright (c) 2018-2022 Xavier Leclercq
    Released under the MIT License
    See https://github.com/diplodocusdb/physical-storage/blob/main/LICENSE.txt
*/

#include "PageTests.h"
#include "PageFileRepositoryTests.h"
#include "DiplodocusDB/PhysicalStorage/linkoptions.hpp"
#include <boost/filesystem/operations.hpp>
#include <Ishiko/TestFramework.hpp>

using namespace Ishiko;

int main(int argc, char* argv[])
{
    TestHarness theTestHarness("DiplodocusDBPageRepository");

    theTestHarness.context().setDataDirectory("../../TestData");
    theTestHarness.context().setOutputDirectory("../../TestOutput");
    boost::filesystem::create_directories("../../TestOutput");
    theTestHarness.context().setReferenceDirectory("../../ReferenceData");

    TestSequence& theTests = theTestHarness.tests();
    theTests.append<PageTests>();
    theTests.append<PageFileRepositoryTests>();

    return theTestHarness.run();
}
