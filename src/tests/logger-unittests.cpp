// Project: Shared immutable, exclusive setup blog support code C++ library
/// @file logger-unittests.cpp
/// @brief Tests for logger class.
///
/// Code accompanying the 
/// "Comments on comments to Herb Sutter's updated GotW #6b solution" series of
/// Dibase blog postings.
///
/// @copyright Copyright (c) Dibase Limited 2013
/// @author Ralph E. McArdell

#include "logger.h"
#include "catch.hpp"

using namespace dibase::blog::sies;

TEST_CASE("blog/sies/logger/no messages to log"
         , "A logger to which nothing has been logged should write nothing"
           " when destroyed"
         )
{
  std::ostringstream out;
  { logger log(out); }
  CHECK(out.str()=="");
}

TEST_CASE("blog/sies/logger/message ending in endline writes endline"
         , "A logger to which text ending in a newline should write text"
           " ending in a newline when destroyed"
         )
{
  std::ostringstream out;
  {
    logger log(out);
    log << '\n';
  }
  CHECK(out.str()=="\n");
}

TEST_CASE("blog/sies/logger/message not ending in endline writes endline"
         , "A logger to which text not ending in a newline should write text"
           " ending in a newline when destroyed"
         )
{
  std::ostringstream out;
  {
    logger log(out);
    log << 'a';
  }
  CHECK(out.str()=="a\n");
}

TEST_CASE("blog/sies/logger/message formatted from various datatypes"
         , "A logger will format text from values of various data types"
         )
{
  std::ostringstream out;
  {
    logger log(out);
    log << 'a';
    log << "bc";
    log << std::string("de");
    log << 0;
    log << 1U;
    log << 2L;
    log << 3UL;
    log << 4LL;
    log << 5ULL;
    log << 6.7f;
    log << 8.9;
  }
  CHECK(out.str()=="abcde0123456.78.9\n");
}

TEST_CASE("blog/sies/logger/message formatted from multiple values"
         , "A logger will allow sequences of operator<< and values"
         )
{
  std::ostringstream out;
  {
    logger log(out);
    log <<'a'<<"bc"<<std::string("de")<<0<<1U<<2L<<3UL<<4LL<<5ULL<<6.7f<<8.9;
  }
  CHECK(out.str()=="abcde0123456.78.9\n");
}

