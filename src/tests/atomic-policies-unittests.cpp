// Project: Shared immutable, exclusive setup blog support code C++ library
/// @file atomic-policies-unittests.cpp
/// @brief Tests for atomic-policies types.
///
/// Code accompanying the 
/// "Comments on comments to Herb Sutter's updated GotW #6b solution" series of
/// Dibase blog postings.
///
/// @copyright Copyright (c) Dibase Limited 2013
/// @author Ralph E. McArdell

#include "atomic-policies.h"
#include "catch.hpp"

using namespace dibase::blog::sies;

typedef int test_type;
typedef test_type* test_type_ptr;

TEST_CASE("blog/sies/atomic-policies/non_atomic/create,store,load"
         , "Should be able to create, store values to and load values from"
         )
{
  test_type orig;
  non_atomic<test_type_ptr> v{&orig};
  CHECK(v.load()==&orig);
  test_type other;
  v.store(&other);
  CHECK(v.load()==&other);
}

TEST_CASE("blog/sies/atomic-policies/atomic/relaxed/create,store,load"
         , "Should be able to create, store values to and load values from"
         )
{
  test_type orig;
  memory_order<std::memory_order_relaxed>::atomic<test_type_ptr> v{&orig};
  CHECK(v.load()==&orig);
  test_type other;
  v.store(&other);
  CHECK(v.load()==&other);
}

TEST_CASE("blog/sies/atomic-policies/atomic/seq_cst/create,store,load"
         , "Should be able to create, store values to and load values from"
         )
{
  test_type orig;
  memory_order<std::memory_order_seq_cst>::atomic<test_type_ptr> v{&orig};
  CHECK(v.load()==&orig);
  test_type other;
  v.store(&other);
  CHECK(v.load()==&other);
}

TEST_CASE("blog/sies/atomic-policies/atomic/release_acquire/create,store,load"
         , "Should be able to create, store values to and load values from"
         )
{
  test_type orig;
  memory_order<std::memory_order_release,std::memory_order_acquire>::atomic<test_type_ptr> v{&orig};
  CHECK(v.load()==&orig);
  test_type other;
  v.store(&other);
  CHECK(v.load()==&other);
}

TEST_CASE("blog/sies/atomic-policies/atomic/release_consume/create,store,load"
         , "Should be able to create, store values to and load values from"
         )
{
  test_type orig;
  memory_order<std::memory_order_release,std::memory_order_consume>::atomic<test_type_ptr> v{&orig};
  CHECK(v.load()==&orig);
  test_type other;
  v.store(&other);
  CHECK(v.load()==&other);
}

