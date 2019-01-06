# Measurement Kit data/UTF-8 helper library

[![GitHub license](https://img.shields.io/github/license/measurement-kit/mkdata.svg)](https://raw.githubusercontent.com/measurement-kit/mkdata/master/LICENSE) [![Github Releases](https://img.shields.io/github/release/measurement-kit/mkdata.svg)](https://github.com/measurement-kit/mkdata/releases) [![Build Status](https://img.shields.io/travis/measurement-kit/mkdata/master.svg?label=travis)](https://travis-ci.org/measurement-kit/mkdata) [![codecov](https://codecov.io/gh/measurement-kit/mkdata/branch/master/graph/badge.svg)](https://codecov.io/gh/measurement-kit/mkdata) [![Build status](https://img.shields.io/appveyor/ci/bassosimone/mkdata/master.svg?label=appveyor)](https://ci.appveyor.com/project/bassosimone/mkdata/branch/master)

This library contains helper code to detect whether a specific string is
binary data or UTF-8 data, and to base64 the string accordingly in case
it does not contain UTF-8 data. Since this library is a basic building block
of Measurement Kit, we don't plan on making any stable API guarantees for
the code in here (i.e. we'll never release `v1.0.0`).
