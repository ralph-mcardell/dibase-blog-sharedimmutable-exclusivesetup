// Project: Shared immutable, exclusive setup blog support code C++ library
/// @file random_in_range-unittests.cpp
/// @brief Tests for random_in_range class.
///
/// Code accompanying the 
/// "Comments on comments to Herb Sutter's updated GotW #6b solution" series of
/// Dibase blog postings.
///
/// @copyright Copyright (c) Dibase Limited 2013
/// @author Ralph E. McArdell

#include "random_in_range.h"
#include "catch.hpp"
#include <cmath>

using namespace dibase::blog::sies;

TEST_CASE("blog/sies/random_in_range/single value range"
         , "A random_in_range object with a range of a single value always"
           " returns that value"
         )
{
  random_in_range rir(10,10);
  CHECK(rir()==10);
  CHECK(rir()==10);
  CHECK(rir()==10);
  CHECK(rir()==10);
  CHECK(rir()==10);
}

TEST_CASE("blog/sies/random_in_range/two value range"
         , "A random_in_range object with a range of a two values always"
           " returns one or the other value with approx. even distribution"
         )
{
  auto const Min(100U);
  auto const Max(101U);
  auto const ExpectedAvg((Min+Max)/2.0);
  auto const NumSamples(1000U);
  auto const Tolerance((Max-Min)*0.09);

  random_in_range rir{Min,Max};
  auto sum(0U);

  for ( auto i=0U; i<NumSamples;++i)  
  { 
    auto random_value(rir());
    CHECK(random_value>=Min);
    CHECK(random_value<=Max);
    sum += random_value;
  }

  auto sampled_avg(double(sum)/NumSamples);

  CHECK(std::fabs(sampled_avg-ExpectedAvg)<=Tolerance);
}

TEST_CASE("blog/sies/random_in_range/many value range"
         , "A random_in_range object with a range of a many values always"
           " returns an in-range value with approx. even distribution"
         )
{
  auto const Min(10U);
  auto const Max(1000U);
  auto const ExpectedAvg((Min+Max)/2.0);
  auto const NumSamples(1000U);
  auto const Tolerance((Max-Min)*0.09);

  random_in_range rir{Min,Max};
  auto sum(0U);

  for ( auto i=0U; i<NumSamples;++i)  
  { 
    auto random_value(rir());
    CHECK(random_value>=Min);
    CHECK(random_value<=Max);
    sum += random_value;
  }

  auto sampled_avg(double(sum)/NumSamples);

  CHECK(std::fabs(sampled_avg-ExpectedAvg)<=Tolerance);
}

TEST_CASE("blog/sies/random_in_range/shared prng ctor"
         , "A random_in_range object constructed from an existing"
           " random_in_range object with different rnage functions similarly to"
           " a completely separate random_in_range object with the same range"
         )
{
  auto const Min(10U);
  auto const Max(1000U);
  auto const ExpectedAvg((Min+Max)/2.0);
  auto const NumSamples(1000U);
  auto const Tolerance((Max-Min)*0.09);

  random_in_range rir_orig{0,1};
  random_in_range rir{rir_orig, Min,Max};
  auto sum(0U);

  for ( auto i=0U; i<NumSamples;++i)  
  { 
    auto random_value(rir());
    CHECK(random_value>=Min);
    CHECK(random_value<=Max);
    sum += random_value;
  }

  auto sampled_avg(double(sum)/NumSamples);

  CHECK(std::fabs(sampled_avg-ExpectedAvg)<=Tolerance);
}

