// SPDX-FileCopyrightText: 2022 Cem Geçgel <gecgelcem@outlook.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

/* Tests the units in the compiler. */
class Tester {
  /* A function that returns the result of a unit test. */
  using Unit = std::function<bool()>;

  /* Information on a unit test. */
  struct Info {
    /* Callable of the test. */
    Unit             unit;
    /* File the test is in. */
    std::string_view file;
    /* Line number in the file the test is at. */
    unsigned         line;
  };

  /* Registered tests. */
  std::vector<Info> registered;

public:

  /* Register the given unit test with the given file and line information. If
   * the location is not given, it is defaulted to the callers location. */
  void Register(
    Unit unit, char const* file = __builtin_FILE(),
    unsigned line = __builtin_LINE()) {
    using namespace std;

    // Remove the parent folders from the file path upto the Rainfall source
    // folder if it could be found.
    string_view path        = file;
    string_view folder      = "/src/";
    auto        folderStart = path.rfind(folder);
    if (folderStart != string_view::npos)
      path = path.substr(folderStart + folder.length());

    registered.push_back({move(unit), path, line});
  }

  /* Run all the unit tests. */
  void Run() const {
    using namespace std;
    using namespace std::chrono;
    using milliseconds = duration<long double, milli>;

    // Store the time for measuring the total time taken.
    auto start = high_resolution_clock::now();

    // Run all the tests and count the failed ones.
    size_t failedTests = 0;
    for (auto const& tested : registered) {
      // Store the time for measuring the time taken by the currently tested
      // unit.
      auto unitStart = high_resolution_clock::now();

      // Test and report.
      if (tested.unit()) {
        cout << "[PASSED]";
      } else {
        cout << "[FAILED]";
        failedTests++;
      }

      // Find the elapsed time for the currently tested unit.
      auto elapsed     = high_resolution_clock::now() - unitStart;
      auto elapsedTime = duration_cast<milliseconds>(elapsed).count();
      cout << format(
                " {}:{:04} ({:.3f} ms)", tested.file, tested.line, elapsedTime)
           << endl;
    }
    cout << endl;

    // Print whether there were failed tests.
    if (registered.empty()) cout << "There are no registered tests!";
    else if (failedTests == 0) cout << "All tests passed.";
    else if (failedTests == 1) cout << "A test failed!";
    else cout << format(" {} tests failed!", failedTests);

    // Find the total elapsed time.
    auto elapsed     = high_resolution_clock::now() - start;
    auto elapsedTime = duration_cast<milliseconds>(elapsed).count();
    cout << format(" ({:.3f} ms)", elapsedTime) << endl;
  }
};
