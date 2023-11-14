# DiplodocusDB Physical Storage

This repository contains components that provide an abstraction layer
over the persistent storage used to store database contents.

For a typical application there should be no need to use the contents of
this repository directly. They are building blocks meant to be used by 
the other DiplodocusDB projects.

# Contents

- Include - Directory containing the public headers for the project.
- PageRepository - Source code for a repository that stores data as a series
of fixed size pages. This allows random access to the pages.
- StreamRepository - Source for a repository that stores data as streams. This
doesn't allow random access but allows additional features like compression and
encryption.
- Tests - The tests for the project.

# Project Status

[![diplodocusdb](https://circleci.com/gh/diplodocusdb/physical-storage.svg?style=shield)](https://circleci.com/gh/diplodocusdb/physical-storage)

This project is in early development and not ready for public use yet.

# Usage

No documentation available as of yet.

# Support

None.

## License

Copyright (c) 2018-2023 Xavier Leclercq\
Released under the MIT License\
See https://github.com/diplodocusdb/physical-storage/blob/main/LICENSE.txt
