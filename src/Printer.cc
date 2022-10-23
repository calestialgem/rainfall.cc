// SPDX-FileCopyrightText: 2022 Cem Geçgel <gecgelcem@outlook.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <exception>
#include <iostream>

/* Write the given formatted string. */
void Write(char const* format) {
  // For all the characters.
  while (*format) {
    // If it is a format specifier, format string is wrong.
    if (*format == '%') {
      std::cerr << "More format specifiers than formatted objects!"
                << std::endl;
      std::terminate();
    }

    // Print the character.
    std::cout << *format++;
  }
}

/* Write the given formatted string with the given formatted objects. */
template<typename First, typename... Rest>
void Write(char const* format, First&& first, Rest&&... rest) {
  // For all the characters upto a format specifier.
  while (*format != '%') {
    // If its the null-terminator, there are too many objects.
    if (!*format) {
      std::cerr << "More formatted objects than format specifiers!"
                << std::endl;
      std::terminate();
    }

    // Print the character.
    std::cout << *format++;
  }

  // Print the first object.
  std::cout << first;

  // Print the rest.
  Write(format + 1, rest...);
}

/* Print a new line. */
void Print() { std::cout << std::endl; }

/* Print the given formatted message before a new line. */
template<typename... Formatted>
void Print(char const* format, Formatted&&... formatted) {
  Write(format, formatted...);
  Print();
}
