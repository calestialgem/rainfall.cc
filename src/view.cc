// SPDX-FileCopyrightText: 2022 Cem Geçgel <gecgelcem@outlook.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <algorithm>
#include <cstddef>

/* Immutable view of a linear array of elements. */
template<typename Element>
class View {
  /* Pointer to the first element. */
  Element const* first;
  /* Pointer after the last element. */
  Element const* after;

public:

  /* View to nowhere, without elements. */
  static constexpr View EMPTY = {nullptr, nullptr};

  /* View of a null-terminated array. */
  [[nodiscard]] static constexpr View
  ofTerminated(Element const* terminated) noexcept {
    Element const* end = terminated;
    while (*end) end++;
    return {terminated, end};
  }

  /* Whether the given view equals to this one. */
  [[nodiscard]] bool constexpr operator==(View const& other) const noexcept {
    return std::ranges::equal(this, other);
  }

  /* Amount of elements in the view. */
  [[nodiscard]] constexpr std::size_t size() const noexcept {
    return after - first;
  }

  /* Iterator to the begining of the view. */
  [[nodiscard]] constexpr Element const* begin() const noexcept {
    return first;
  }

  /* Iterator to the end of the view. */
  [[nodiscard]] constexpr Element const* end() const noexcept { return after; }

  /* Immutable iterator to the begining of the view. */
  [[nodiscard]] constexpr Element const* cbegin() const noexcept {
    return first;
  }

  /* Immutable iterator to the end of the view. */
  [[nodiscard]] constexpr Element const* cend() const noexcept { return after; }
};
