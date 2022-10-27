#include "debugger.cc"
#include "tester.cc"

#include <iostream> // cout
#include <ostream>  // endl

/* Runs all the unit tests in the compiler and returns whether all passed. */
bool TestEverything() {
  rainfall::Tester tester;
  rainfall::Tester::TestModule(tester);
  rainfall::Debugger::TestModule(tester);
  return tester.ReportAndReturnResults();
}

/* Parses the command-line arguments and runs the compiler, if all the unit
 * tests pass. */
int main(int const argumentCount, char const* const* const argumentValues) {
  if (!TestEverything()) return -1;

  // Run the compiler.
  std::cout << "Hello!" << std::endl;
}
