// SPDX-FileCopyrightText: 2022 Cem Geçgel <gecgelcem@outlook.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <cstdio>

/* Entry to the compiler. */
int main(int argc, char const* const* argv) {
  if (argc > 2) {
    std::fprintf(stderr, "Usage: rainfall [workspace-directory](default:.)\n");
    return -1;
  }
  char const* workspaceDirectory = argc == 2 ? argv[1] : ".";
  std::printf("Workspace: %s\n", workspaceDirectory);
}
