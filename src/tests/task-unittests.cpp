// Project: Shared immutable, exclusive setup blog support code C++ library
/// @file task-unittests.cpp
/// @brief Tests for task class.
///
/// Code accompanying the 
/// "Comments on comments to Herb Sutter's updated GotW #6b solution" series of
/// Dibase blog postings.
///
/// @copyright Copyright (c) Dibase Limited 2013
/// @author Ralph E. McArdell

#include "task.h"
#include "catch.hpp"

using namespace dibase::blog::sies;

TEST_CASE("blog/sies/task/do nothing task function, no messages to log"
         , "A task that does nothing and logs nothing should write nothing"
           "to its log stream when destroyed"
         )
{
  std::ostringstream out;
  { task(out, [](logger&)->void{}); }
  CHECK(out.str()=="");
}

TEST_CASE("blog/sies/task/task function writes to log"
         , "A task function that writes to its log should have that data in"
           "its log stream when destroyed"
         )
{
  std::ostringstream out;
  { task(out, [](logger& log){ log<<"Hello "; log << "World" << std::endl;}); }
  CHECK(out.str()=="Hello World\n");
}

