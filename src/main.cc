// SPDX-FileCopyrightText: 2022 Cem Geçgel <gecgelcem@outlook.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "linear.cc"
#include "tester.cc"

/* Run the unit tests. Returns whether all tests passed. */
bool RunTests() {
  Tester tester;
  Tester::RegisterTests(tester);
  Linear::RegisterTests(tester);
  return tester.Run();
}

/* Entry to the compiler. */
int main(int argumentCount, char const* const* arguments) {
  // Run the unit tests.
  if (!RunTests()) return -1;

  // Load the current working directory if no arguments are given.
  char const* workspaceDirectory = ".";
  if (argumentCount == 2) {
    workspaceDirectory = std::span(arguments, argumentCount)[1];
  } else if (argumentCount > 2) {
    std::cerr << "Usage: rainfall [workspace-directory](default: .)"
              << std::endl;
    return -1;
  }

  auto workspace = Linear::LoadWorkspace(workspaceDirectory);

  if (workspace.packages.empty()) {
    std::cerr << "There are no packages in the workspace!" << std::endl;
    return -1;
  }

  std::cout << "Loaded packages: " << std::endl;
  for (auto const& [name, package] : workspace.packages)
    std::cout << std::format("- {}", name) << std::endl;
}
