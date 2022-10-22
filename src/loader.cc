// SPDX-FileCopyrightText: 2022 Cem Geçgel <gecgelcem@outlook.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "tester.cc"

/* Loads files of a package in to the memory. */
class Loader {
public:

  /* A Thrice source. */
  struct Source {
    /* Name of the source. */
    std::string name;
    /* Contents of the source. */
    std::string contents;
  };

  /* A Thrice module. */
  struct Module {
    /* Name of the module. */
    std::string         name;
    /* Contained sources that are directly under the module. This does not
     * include the sources that are contained by the submodules of the
     * module. */
    std::vector<Source> sources;
    /* Contained modules that are directly under the module. This does not
     * include the modules that are contained by the submodules of the
     * module. */
    std::vector<Module> submodules;
  };

  /* A Thrice package. */
  struct Package {
    /* Name of the package. */
    std::string                  name;
    /* Contents of the package as a module or as a single source package. */
    std::variant<Module, Source> contents;
  };

  /* A Rainfall workspace, which contains Thrice packages. */
  struct Workspace {
    /* Packages in the workspace. */
    std::vector<Package> packages;
  };

private:

  /* Whether the given string can be the name of a Thrice source. */
  static bool CheckName(std::string_view name) {
    auto CompareLowerCase = [](char compared) {
      return compared >= 'a' && compared <= 'z';
    };
    auto CompareUpperCase = [](char compared) {
      return compared >= 'A' && compared <= 'Z';
    };
    auto CompareDecimalDigit = [](char compared) {
      return compared >= '0' && compared <= '9';
    };
    auto CompareAlpha = [=](char compared) {
      return CompareLowerCase(compared) || CompareUpperCase(compared) ||
             CompareDecimalDigit(compared);
    };
    return !name.empty() && CompareUpperCase(name[0]) &&
           std::ranges::all_of(name, CompareAlpha);
  }

  /* Source at the given path. */
  static std::optional<Source>
  LoadSource(std::filesystem::path const& path, bool package) {
    // Check whether it is a file.
    if (!std::filesystem::is_regular_file(path)) return std::nullopt;

    // Check whether the extension is valid.
    if (!path.has_extension() || path.extension() != ".tr") return std::nullopt;

    // Check whether the name is valid or its at package level and the name is
    // "main".
    if (
      !path.has_filename() ||
      !(CheckName(path.stem().string()) || (package && path.stem() == "main")))
      return std::nullopt;

    // Try to open the file.
    std::ifstream inputStream(path);
    if (!inputStream.is_open()) return std::nullopt;

    // Read the contents.
    std::string contents;
    inputStream >> contents;
    return Source{.name = path.stem().string(), .contents = contents};
  }

  /* Total amount of sources in the module including the ones in the
   * submodules. */
  static std::size_t CountContainedSources(Module const& counted) {
    return std::transform_reduce(
      counted.submodules.begin(), counted.submodules.end(),
      counted.sources.size(), std::plus{},
      [](Module const& submodule) { return CountContainedSources(submodule); });
  }

  /* Module at the given directory. */
  static std::optional<Module>
  LoadModule(std::filesystem::path const& directory, bool package) {
    // Check whether the name is valid.
    if (!directory.has_filename() || !CheckName(directory.filename().string()))
      return std::nullopt;

    Module result;

    // Load all the entries in the directory.
    for (auto const& entry : std::filesystem::directory_iterator(directory)) {
      if (entry.is_directory()) {
        // Try loading the directory as a submodule.
        auto submodule = LoadModule(entry, false);
        if (submodule) result.submodules.emplace_back(*std::move(submodule));
      } else {
        // Try loading the path as a source.
        auto source = LoadSource(entry, package);
        if (source) result.sources.emplace_back(*std::move(source));
      }
    }

    // The directory is not a module if there are no Thrice source in it.
    if (CountContainedSources(result) == 0) return std::nullopt;

    return result;
  }

public:

  /* Loads the workspace in the given directory. */
  static Workspace LoadWorkspace(std::filesystem::path const& directory) {
    Workspace result;
    for (auto const& entry : std::filesystem::directory_iterator(directory)) {
      if (entry.is_directory()) {
        auto packageModule = LoadModule(entry, true);
        if (packageModule)
          result.packages.push_back({packageModule->name, *packageModule});
      } else {
        auto source = LoadSource(entry, false);
        if (source) result.packages.push_back({source->name, *source});
      }
    }
    return result;
  }

  /* Register the tests in the loader module to the given tester. */
  static void RegisterTests(Tester& tester) {
    // Test checking empty source name.
    tester.Register([]() { return !CheckName(""); });
    // Test checking source name with a non-upper case start.
    tester.Register([]() { return !CheckName("hello"); });
    // Test checking source name with a non-alphabetic character.
    tester.Register([]() { return !CheckName("Hello+"); });
    // Test checking a valid source name.
    tester.Register([]() { return CheckName("Hello"); });

    // Test loading the Test package.
    tester.Register([]() {
      auto workspace = LoadWorkspace(".");
      return workspace.packages.size() == 2;
    });
  }
};