// Project: Shared immutable, exclusive setup blog support code C++ library
/// @file call_context_validator-unittests.cpp
/// @brief Tests for call_context_validator types.
///
/// Code accompanying the 
/// "Comments on comments to Herb Sutter's updated GotW #6b solution" series of
/// Dibase blog postings.
///
/// @copyright Copyright (c) Dibase Limited 2013
/// @author Ralph E. McArdell

#include "call_context_validator.h"
#include "catch.hpp"
#include <string>
#include <thread>

using namespace dibase::blog::sies;

// Using no synchronisation safe in tests as other threads used by tests
// created after publishing if test is on published object.
template <class T>
struct no_sync
{
  T data;
  explicit no_sync(T d) : data{d} {}
  void store(T d) { data = d; }
  T load() const { return data; }
};

class test_type
{
  call_context_validator<no_sync, test_type> validate_usage;
  std::string data;

public:
  test_type() = default;
  test_type(test_type const &) = delete;
  test_type & operator=(test_type const &) = delete;
  test_type(test_type &&) = delete;
  test_type & operator=(test_type &&) = delete;

  void set(std::string const & d) 
  {
    validate_usage(this);
    data = d;
  }

  std::string get() const
  {
    validate_usage(this);
    return data;
  }

  void setup_complete()
  {
    validate_usage.publish(this);
  }
};

TEST_CASE("blog/sies/call_context_validator/create, write, read same thread unpublished"
         , "Should be allowed to create, write & read object on same thread"
         )
{
  test_type v;
  v.set("hello");
  CHECK(v.get()=="hello");
}

TEST_CASE("blog/sies/call_context_validator/create, publish, read same thread"
         , "Should be allowed to create, publish & read object on same thread"
         )
{
  test_type v;
  v.set("hello");
  v.setup_complete();
  CHECK(v.get()=="hello");
}

TEST_CASE("blog/sies/call_context_validator/create, publish, write same thread"
         , "Should NOT be allowed to create, publish then write object even on same thread"
         )
{
  test_type v;
  v.setup_complete();
  CHECK_THROWS_AS(v.set("hi!"),call_context_violation);
}

TEST_CASE("blog/sies/call_context_validator/create, one thread, read other thread unpublished"
         , "Should NOT be allowed to read from thread other than creation "
           "thread if the object is unpublished."
         )
{
  test_type v;
  v.set("hello");
  std::thread([&v](){CHECK_THROWS_AS(v.get(),call_context_violation);}).join();
  CHECK(v.get()=="hello");
}

TEST_CASE("blog/sies/call_context_validator/create one thread, write other thread unpublished"
         , "Should NOT be allowed to write from thread other than creation "
           "thread if the object is unpublished."
         )
{
  test_type v;
  v.set("hello");
  std::thread([&v](){CHECK_THROWS_AS(v.set(""),call_context_violation);}).join();
  CHECK(v.get()=="hello");
}

TEST_CASE("blog/sies/call_context_validator/create, publish, one thread, read other thread"
         , "CAN read from thread other than creation "
           "thread if the object is published."
         )
{
  test_type v;
  v.set("hello");
  v.setup_complete();
  std::thread([&v](){CHECK(v.get()=="hello");}).join();
  CHECK(v.get()=="hello");
}

TEST_CASE("blog/sies/call_context_validator/create, publish one thread write other thread"
         , "Should NOT be allowed to write from thread other than creation "
           "thread if the object is published."
         )
{
  test_type v;
  v.set("hello");
  v.setup_complete();
  std::thread([&v](){CHECK_THROWS_AS(v.set(""),call_context_violation);}).join();
  CHECK(v.get()=="hello");
}

