# <img src="assets/icon.png" alt="icon" width="4%"/> pac-man - A Clone in CPP

[![Linux](https://svgshare.com/i/Zhy.svg)](https://docs.microsoft.com/en-us/windows/wsl/tutorials/gui-apps)
[![GitHub license](https://img.shields.io/github/license/ThomasByr/pac-man)](https://github.com/ThomasByr/pac-man/blob/master/LICENSE)
[![GitHub commits](https://badgen.net/github/commits/ThomasByr/pac-man)](https://GitHub.com/ThomasByr/pac-man/commit/)
[![GitHub latest commit](https://badgen.net/github/last-commit/ThomasByr/pac-man)](https://gitHub.com/ThomasByr/pac-man/commit/)
[![Maintenance](https://img.shields.io/badge/maintained%3F-yes-green.svg)](https://GitHub.com/ThomasByr/pac-man/graphs/commit-activity)

[![C/C++ CI](https://github.com/ThomasByr/pac-man/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/ThomasByr/pac-man/actions/workflows/c-cpp.yml)
[![CodeQL](https://github.com/ThomasByr/pac-man/actions/workflows/codeql.yml/badge.svg)](https://github.com/ThomasByr/pac-man/actions/workflows/codeql.yml)
[![Publish](https://github.com/ThomasByr/pac-man/actions/workflows/publish.yml/badge.svg)](https://github.com/ThomasByr/pac-man/actions/workflows/publish.yml)

[![GitHub version](https://badge.fury.io/gh/ThomasByr%2Fpac-man.svg)](https://github.com/ThomasByr/pac-man)
[![Author](https://img.shields.io/badge/author-@ThomasByr-blue)](https://github.com/ThomasByr)
[![Author](https://img.shields.io/badge/author-@Arnaud-blue)](https://github.com/Loussiaul)

1. [✏️ Setup](#️-setup)
2. [💁 More infos and Usage](#-more-infos-and-usage)
3. [🧪 Testing](#-testing)
4. [🧑‍🏫 Contributing](#-contributing)
5. [⚖️ License](#️-license)
6. [🔄 Changelog](#-changelog)
7. [🐛 Bugs \& TODO](#-bugs--todo)

## ✏️ Setup

> **Note**
> This project is part of a M1 study done at the University of Strasbourg, France.

Please make sure you have `libsdl2` installed :

```bash
sudo apt install libsdl2-dev
```

<details>
  <summary>  For Windows 11 WSLg users (click here to expand) </summary>
  Please make sure you have graphical x11 support installed:

  ```bash
  sudo apt-get install libgl1 libxkbcommon-x11-0
  ```

</details>

Then compile a release version of the program with :

```bash
make release
```

The produced executable binary is to be found inside of the `bin` folder.

## 💁 More infos and Usage

> **Warning**
> We use SDL2 as the only external dependency. Others dependencies are compiled from source (please see [lib](lib) folder). Note that if not specified, all files are written by @ThomasByr.
>
> Notable ones are :
>
> - ctpl.hpp, a C++11 header-only library for thread pools
> - ini.hpp, a single-file header-only library for INI files
> - json.hpp, a single-file header-only library for JSON (by Niels Lohmann)

## 🧪 Testing

Oh god... please don't.

Still, make sure you have `valgrind` up and ready and then run :

```bash
cd tests && make check_quiet
```

## 🧑‍🏫 Contributing

If you ever want to contribute, please begin by reading our [Contributing Guidelines](.github/CONTRIBUTING.md).

> The standard procedure is :
>
> ```txt
> fork -> git branch -> push -> pull request
> ```
>
> Note that we won't accept any PR :
>
> - that does not follow our Contributing Guidelines
> - that is not sufficiently commented or isn't well formated
> - without any proper test suite
> - with a failing or incomplete test suite

Happy coding ! 🙂

## ⚖️ License

This project is licensed under the AGPL-3.0 new or revised license. Please read the [LICENSE](LICENSE) file.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
- Neither the name of the pac-man authors nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

## 🔄 Changelog

<details>
  <summary>  Beta first minor release (click here to expand) </summary>

**v0.1.0** first release

- brought back `ctpl.hpp` (thread pool)
- added `ini.hpp` (INI files) and `json.hpp` (JSON files)
- made some header files for game logic

</details>

## 🐛 Bugs & TODO

**known bugs** (final correction patch version) [see Issues](https://github.com/ThomasByr/pac-man/issues)

**todo** (first implementation version)
