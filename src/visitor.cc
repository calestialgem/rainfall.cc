// SPDX-FileCopyrightText: 2022 Cem Geçgel <gecgelcem@outlook.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

/* For visiting `std::variant`s. */
template<class... Types>
struct Visitor : Types... {
  using Types::operator()...;
};

// Deduction guide that is supposed to be unnecessary for C++20 but the visitor
// does not work without it.
template<class... Types>
Visitor(Types...) -> Visitor<Types...>;
