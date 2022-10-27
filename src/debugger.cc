#pragma once

#include <cstring> // strlen

namespace rainfall {

/* Helps to debug the Rainfall source files. */
class Debugger {
public:

  /* Compares the given range of characters to "src", which is the name of the
   * root source directory of Rainfall files. */
  static bool
  CompareRangeToRoot(char const* const first, char const* const after) {
    // Check the length, then the contents.
    return after - first == 3 && first[0] == 's' && first[1] == 'r' &&
           first[2] == 'c';
  }

  /* Removes the part of the given full Rainfall source file path upto the root
   * source directory of the project. */
  static char const* SkipPathBeyondRoot(char const* const fullPath) {
    /* Pointer to a path separator just after the name of the last checked
     * directory. */
    char const* afterDirectory = nullptr;

    // Last directory named "src" is the root source directory of the project.
    // Go from the end, because there could be other directories named "src" in
    // the path. It is guaranteed that there are no directories named "src"
    // inside the root source directory.
    for (char const* beforeDirectory = fullPath + std::strlen(fullPath) - 1;
         beforeDirectory >= fullPath; beforeDirectory--) {
      // Pass until just after a directory name.
      if (*beforeDirectory != '/') continue;

      // If there is no previous position, the name that was just passed over
      // was the name of the source file. Otherwise, it was a directory.
      if (
        afterDirectory != nullptr &&
        CompareRangeToRoot(beforeDirectory + 1, afterDirectory))
        // Return the path after the path separator that comes after the
        // directory whose name was just checked.
        return afterDirectory + 1;

      // Save the position for the next iteration if the passed portion was the
      // name of the source file or a directory that is not named "src".
      afterDirectory = beforeDirectory;
    }

    // Check whether the path starts with "src", which is not checked by the
    // above function if the path is not formed as "/src/...".
    if (
      afterDirectory != nullptr && fullPath[0] != '/' &&
      CompareRangeToRoot(fullPath, afterDirectory))
      return afterDirectory + 1;

    // Did not found the root folder, return the full path.
    return fullPath;
  }

  /* Runs the unit tests in the this module using the given tester. */
  template<typename Tester>
  static void TestModule(Tester& tester) {
    // Test `CompareRangeToRoot`.
    // Check with a correct name.
    tester([]() {
      char const* const checked = "src";
      return CompareRangeToRoot(checked, checked + std::strlen(checked));
    });
    // Check with a different length range.
    tester([]() {
      char const* const checked = "directory";
      return !CompareRangeToRoot(checked, checked + std::strlen(checked));
    });
    // Check with a different first character.
    tester([]() {
      char const* const checked = "arc";
      return !CompareRangeToRoot(checked, checked + std::strlen(checked));
    });
    // Check with a different second character.
    tester([]() {
      char const* const checked = "sec";
      return !CompareRangeToRoot(checked, checked + std::strlen(checked));
    });
    // Check with a different third character.
    tester([]() {
      char const* const checked = "sri";
      return !CompareRangeToRoot(checked, checked + std::strlen(checked));
    });

    // Test `SkipPathBeyondRoot`
    // Test with empty path.
    tester([]() {
      char const* const path = "";
      return SkipPathBeyondRoot(path) == path;
    });
    // Test with path without parent any directories.
    tester([]() {
      char const* const path = "source.cc";
      return SkipPathBeyondRoot(path) == path;
    });
    // Test with path without parent any directories from the root.
    tester([]() {
      char const* const path = "/source.cc";
      return SkipPathBeyondRoot(path) == path;
    });
    // Test with path that has the root source directory at the middle.
    tester([]() {
      char const* const path = "/rainfall/src/source.cc";
      return SkipPathBeyondRoot(path) == path + std::strlen("/rainfall/src/");
    });
    // Test with path that has the root source directory at beginning.
    tester([]() {
      char const* const path = "src/source.cc";
      return SkipPathBeyondRoot(path) == path + std::strlen("src/");
    });
  }
};

} // namespace rainfall
