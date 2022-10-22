// SPDX-FileCopyrightText: 2022 Cem Geçgel <gecgelcem@outlook.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "tester.cc"

/* Entry to the compiler. */
int main() {
  Tester tester;
  Tester::RegisterTests(tester);
  tester.Run();
}
