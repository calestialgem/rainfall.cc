#pragma once

#include "debugger.cc"

#include <chrono>   // high_resolution_clock, duration
#include <cstddef>  // size_t
#include <format>   // format
#include <iostream> // cout
#include <ostream>  // endl
#include <ratio>    // milli

namespace rainfall {

/* Runs the unit tests and reports the results. */
class Tester {
  /* Clock type that is used for timing the tests. */
  using Clock                        = std::chrono::high_resolution_clock;
  /* Arithmetic duration type. */
  using Time                         = long double;
  /* Maximum duration where a passing test is reported because it takes too
   * much time. */
  static constexpr Time MAX_DURATION = 5.0;

  /* Number of tests that run. */
  std::size_t       runCount    = 0;
  /* Number of tests that failed. */
  std::size_t       failedCount = 0;
  /* Time where the tester is created. Used for measuring the total elapsed time
   * for all the tests. */
  Clock::time_point startTime   = Clock::now();

  /* Measures the time passed from the given time in milliseconds. */
  static Time MeasureMilliseconds(Clock::time_point startTime) {
    auto const                                    endTime = Clock::now();
    std::chrono::duration<Time, std::milli> const duration =
      endTime - startTime;
    return duration.count();
  }

public:

  /* Runs given unit test and reports the given file and line information if it
   * fails or runs too slow. File and line defaults to the caller location. */
  template<typename Test>
  void operator()(
    Test const test, char const* const file = __builtin_FILE(),
    unsigned const line = __builtin_LINE()) {
    // Count the run.
    runCount++;

    // Call it while measuring the time taken.
    auto const startTime = Clock::now();
    auto const result    = test();
    auto const duration  = MeasureMilliseconds(startTime);

    // If it passed in an acceptable time pass.
    if (result && duration < MAX_DURATION) return;

    // Count if it failed and report it.
    if (!result) failedCount++;
    std::cout << std::format(
                   "[{}] {}:{:04} ({:.3} ms)", result ? "TOO LONG" : "FAILED",
                   Debugger::SkipPathBeyondRoot(file), line, duration)
              << std::endl;
  }

  /* Report the results and returns whether all tests passed. */
  bool ReportAndReturnResults() const {
    // Report whether there were failed tests.
    if (runCount == 0) std::cout << "No test run!";
    else if (failedCount == 0) std::cout << "All tests passed.";
    else
      std::cout << std::format(
        "{}/{} of the tests failed!", failedCount, runCount);

    // Report the total time taken by all the unit tests.
    std::cout << std::format(" ({:.3} ms)", MeasureMilliseconds(startTime))
              << std::endl;

    return failedCount == 0;
  }

  /* Runs the unit tests in the this module using the given tester. */
  template<typename Tester>
  static void TestModule(Tester& tester) {
    // Test that always passed, just to run the test infrastructure.
    tester([]() { return true; });
  }
};

} // namespace rainfall
