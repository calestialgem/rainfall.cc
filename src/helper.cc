#pragma once

#include <iostream>

namespace rainfall {

/* Blanked `Discard` implementation for any object. It does not do anything,
 * assumes the object does not have any resources. If it has, implement it for
 * that object. */
template<typename Object>
void Discard(Object&) {
  std::cout << "Discarding object!" << std::endl;
}

/* Represents a value that might not be there. */
template<typename Value>
class Box {
  /* Contained value. */
  Value value;
  /* Whether there is a value. */
  bool  full;

public:

  /* Creates a box with the given value. */
  static Box Full(Value value) {
    Box created;
    created.value = value;
    created.full  = true;
    return created;
  }

  /* Creates a valueless box. */
  static Box Empty() {
    Box created;
    created.full = false;
    return created;
  }

  /* Destructs the result. */
  friend void Discard(Box& box) {
    if (box.full) Discard(box.value);
  }

  /* Returns whether the box has a value. */
  operator bool() const { return full; }

  /* Returns a mutable reference to the contained value. */
  Value& operator*() { return value; }

  /* Returns a mutable pointer to the contained value. */
  Value* operator->() { return &value; }

  /* Returns an immutable reference to the contained value. */
  Value const& operator*() const { return value; }

  /* Returns an immutable pointer to the contained value. */
  Value const* operator->() const { return &value; }
};

/* Represents the outcome of a computation that can fail. */
template<typename Value, typename Error>
class Result {
  union {
    /* Resulted value. */
    Value value;
    /* Resulted error. */
    Error error;
  };

  /* Whether there is a value. */
  bool success;

public:

  /* Creates a result with the given value. */
  static Result Success(Value value) {
    Result created;
    created.value   = value;
    created.success = true;
    return created;
  }

  /* Creates a result with the given error. */
  static Result Failure(Error error) {
    Result created;
    created.error   = error;
    created.success = false;
    return created;
  }

  /* Returns whether the result has a value. */
  operator bool() const { return success; }

  /* Returns a mutable reference to the resulted value. */
  Value& operator*() { return value; }

  /* Returns a mutable pointer to the resulted value. */
  Value* operator->() { return &value; }

  /* Returns an immutable reference to the resulted value. */
  Value const& operator*() const { return value; }

  /* Returns an immutable pointer to the resulted value. */
  Value const* operator->() const { return &value; }

  /* Returns a mutable reference to the resulted error. */
  Error& GetError() { return error; }

  /* Returns an immutable reference to the resulted error. */
  Error const& GetError() const { return error; }
};

/* Destructs the result. */
template<typename Value, typename Error>
void Discard(Result<Value, Error>& result) {
  std::cout << "Discarding result!" << std::endl;
  if (result) Discard(*result);
  else Discard(result.GetError());
}

class Helper {
public:

  /* Runs the unit tests in the this module using the given tester. */
  template<typename Tester>
  static void TestModule(Tester& tester) {
    // Test `Box`.
    // Test `Box::Full` and `Box::operator bool`.
    tester([]() {
      auto const full = Box<int>::Full(5);
      return !!full;
    });
    // Test `Box::Empty` and `Box::operator bool`.
    tester([]() {
      auto const empty = Box<int>::Empty();
      return !empty;
    });
    // Test mutable `Box::operator*`.
    tester([]() {
      auto full = Box<int>::Full(5);
      *full     = 2;
      return *full == 2;
    });
    // Test immutable `Box::operator*`.
    tester([]() {
      auto const full = Box<int>::Full(5);
      return *full == 5;
    });

    // Test `Result`.
    // Test `Result::Success`.
    tester([]() {
      auto const success = Result<int, int>::Success(5);
      Discard(success);
      return !!success;
    });
    // Test `Result::Failure`.
    tester([]() {
      auto const failure = Result<int, int>::Failure(5);
      return !failure;
    });
  }
};

} // namespace rainfall
