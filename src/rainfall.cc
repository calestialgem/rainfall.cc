// SPDX-FileCopyrightText: 2022 Cem Geçgel <gecgelcem@outlook.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

/* Rainfall's representation of files. */
template<typename Source>
struct Rainfall {
  /* A Thrice module. */
  struct Module {
    /* Name of the module. */
    std::string                             name;
    /* Contained sources that are directly under the module. This does not
     * include the sources that are contained by the submodules of the
     * module. */
    std::unordered_map<std::string, Source> sources;
    /* Contained modules that are directly under the module. This does not
     * include the modules that are contained by the submodules of the
     * module. */
    std::unordered_map<std::string, Module> submodules;
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
    std::unordered_map<std::string, Package> packages;
  };

  /* Total amount of sources in the module including the ones in the
   * submodules. */
  static std::size_t CountContainedSources(Module const& counted) {
    return std::transform_reduce(
      counted.submodules.begin(), counted.submodules.end(),
      counted.sources.size(), std::plus{},
      [](std::pair<std::string, Module> const& submodule) {
      return CountContainedSources(submodule.second);
      });
  }
};
