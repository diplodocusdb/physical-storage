# DiplodocusDB - Physical Storage

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

This project is intended for use by its author only.

# Usage

No documentation available as of yet.

# Support

None.

## License

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
