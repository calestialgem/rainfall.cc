// SPDX-FileCopyrightText: 2022 Cem Geçgel <gecgelcem@outlook.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "rainfall.cc"
#include "tester.cc"

/* Character by character representation of code. */
struct Linear {
  /* A Thrice source. */
  struct Source {
    /* Path to the source file. */
    std::filesystem::path path;
    /* Name of the source. */
    std::string           name;
    /* Contents of the source. */
    std::string           contents;
  };

  /* Register the tests in the linear module to the given tester. */
  static void RegisterTests(Tester& tester) {
    // Test checking empty source name.
    tester.Register([]() { return !CheckName(""); });
    // Test checking source name with a non-upper case start.
    tester.Register([]() { return !CheckName("hello"); });
    // Test checking source name with a non-alphabetic character.
    tester.Register([]() { return !CheckName("Hello+"); });
    // Test checking a valid source name.
    tester.Register([]() { return CheckName("Hello"); });

    // Test whether correct amount of packages are loaded.
    tester.Register([]() {
      auto workspace = LoadWorkspace(".");
      return workspace.packages.size() == 2;
    });

    // Test whether all packages have correct names.
    tester.Register([]() {
      auto workspace = LoadWorkspace(".");
      return std::ranges::all_of(
        workspace.packages,
        [](std::pair<std::string, Rainfall<Source>::Package> const& package) {
        return std::visit(
          Visitor{
            [&package](Rainfall<Source>::Module const& content) {
          return content.name == package.first;
            },
            [&package](Source const& content) {
          return content.name == package.first;
          }},
          package.second.contents);
        });
    });

    // Test whether all packages have a name.
    tester.Register([]() {
      auto workspace = LoadWorkspace(".");
      return std::ranges::all_of(
        workspace.packages,
        [](std::pair<std::string, Rainfall<Source>::Package> const& package) {
        return !package.first.empty();
        });
    });
  }

  /* Loads the workspace in the given directory. */
  static Rainfall<Source>::Workspace
  LoadWorkspace(std::filesystem::path const& directory) {
    Rainfall<Source>::Workspace result;
    for (auto const& entry : std::filesystem::directory_iterator(directory)) {
      if (entry.is_directory()) {
        auto packageModule = LoadModule(entry, true);
        if (packageModule)
          result.packages[packageModule->name] = {
            packageModule->name, *packageModule};
      } else {
        auto source = LoadSource(entry, false);
        if (source) result.packages[source->name] = {source->name, *source};
      }
    }
    return result;
  }

private:

  /* Whether the given string can be the name of a Thrice source. */
  static bool CheckName(std::string_view name) {
    auto CompareToLowerCase = [](char compared) {
      return compared >= 'a' && compared <= 'z';
    };
    auto CompareToUpperCase = [](char compared) {
      return compared >= 'A' && compared <= 'Z';
    };
    auto CompareToDecimalDigit = [](char compared) {
      return compared >= '0' && compared <= '9';
    };
    auto CompareToAlpha = [=](char compared) {
      return CompareToLowerCase(compared) || CompareToUpperCase(compared) ||
             CompareToDecimalDigit(compared);
    };
    return !name.empty() && CompareToUpperCase(name[0]) &&
           std::ranges::all_of(name, CompareToAlpha);
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
    return Source{
      .path = path, .name = path.stem().string(), .contents = contents};
  }

  /* Module at the given directory. */
  static std::optional<Rainfall<Source>::Module>
  LoadModule(std::filesystem::path const& directory, bool package) {
    // Check whether the name is valid.
    if (!directory.has_filename() || !CheckName(directory.filename().string()))
      return std::nullopt;

    Rainfall<Source>::Module result = {.name = directory.filename().string()};

    // Load all the entries in the directory.
    for (auto const& entry : std::filesystem::directory_iterator(directory)) {
      if (entry.is_directory()) {
        // Try loading the directory as a submodule.
        auto submodule = LoadModule(entry, false);
        if (submodule)
          result.submodules[submodule->name] = *std::move(submodule);
      } else {
        // Try loading the path as a source.
        auto source = LoadSource(entry, package);
        if (source) result.sources[source->name] = *std::move(source);
      }
    }

    // The directory is not a module if there are no Thrice source in it.
    if (Rainfall<Source>::CountContainedSources(result) == 0)
      return std::nullopt;

    return result;
  }
};
