// Project: Shared immutable, exclusive setup blog support code C++ library
/// @file rnd_text_info_maker-unittests.cpp
/// @brief Tests for rnd_text_info_maker class.
///
/// Code accompanying the 
/// "Comments on comments to Herb Sutter's updated GotW #6b solution" series of
/// Dibase blog postings.
///
/// @copyright Copyright (c) Dibase Limited 2013
/// @author Ralph E. McArdell

#include "rnd_text_info_maker.h"
#include "catch.hpp"

using namespace dibase::blog::sies;

TEST_CASE("blog/sies/rnd_text_info_maker/Zero chunks range"
         , "A rnd_text_info_maker making text_info object with chunks in the "
           "range [0,0] creates an empty text_info object."
         )
{
  CHECK((rnd_text_info_maker{0,0, 1,2, 3,4})()->number_of_chunks()==0);
}

TEST_CASE("blog/sies/rnd_text_info_maker/One chunk range"
         , "A rnd_text_info_maker making text_info object with chunks in the "
           "range [1,1] creates an text_info object with 1 chunk."
         )
{
  CHECK((rnd_text_info_maker{1,1, 1,2, 3,4})()->number_of_chunks()==1);
}

TEST_CASE("blog/sies/rnd_text_info_maker/Two chunks range"
         , "A rnd_text_info_maker making text_info object with chunks in the "
           "range [2,2] creates an text_info object with 2 chunks."
         )
{
  CHECK((rnd_text_info_maker{2,2, 1,2, 3,4})()->number_of_chunks()==2);
}

TEST_CASE("blog/sies/rnd_text_info_maker/chunks in range"
         , "A rnd_text_info_maker making text_info object with chunks in the "
           "range [3,4] creates an text_info object with 3 or 4 chunks."
         )
{
  auto random_text_info_object(rnd_text_info_maker{3,4, 1,2, 3,4}());
  CHECK(3<=random_text_info_object->number_of_chunks());
  CHECK(random_text_info_object->number_of_chunks()<=4);
}

TEST_CASE("blog/sies/rnd_text_info_maker/Zero words range - words"
         , "A rnd_text_info_maker making text_info object with chunks having a"
           " number of words in the range [0,0] creates chunks with no words"
         )
{
  CHECK((rnd_text_info_maker{1,1, 0,0, 3,4})()->chunk_word_count(0)==0);
}

TEST_CASE("blog/sies/rnd_text_info_maker/Zero words range - chars"
         , "A rnd_text_info_maker making text_info object with chunks having a"
           " number of words in the range [0,0] creates chunks with empty text"
         )
{
  CHECK((rnd_text_info_maker{1,1, 0,0, 3,4})()->chunk_char_count(0)==0);
}

TEST_CASE("blog/sies/rnd_text_info_maker/One word range"
         , "A rnd_text_info_maker making text_info object with chunks having a"
           " number of words in the range [1,1] creates chunks with 1 word"
         )
{
  CHECK((rnd_text_info_maker{1,1, 1,1, 3,4})()->chunk_word_count(0)==1);
}

TEST_CASE("blog/sies/rnd_text_info_maker/Two word range"
         , "A rnd_text_info_maker making text_info object with chunks having a"
           " number of words in the range [2,2] creates chunks with 2 words"
         )
{
  CHECK((rnd_text_info_maker{1,1, 2,2, 3,4})()->chunk_word_count(0)==2);
}

TEST_CASE("blog/sies/rnd_text_info_maker/words in range"
         , "A rnd_text_info_maker making text_info object with chunks having "
           "number of words in the range [5,6] creates chunks with 5 or 6 words"
         )
{
  auto random_text_info_object(rnd_text_info_maker{1,1, 5,6, 3,4}());
  CHECK(5<=random_text_info_object->chunk_word_count(0));
  CHECK(random_text_info_object->chunk_word_count(0)<=6);
}

TEST_CASE("blog/sies/rnd_text_info_maker/Zero word-size range - words"
         , "A rnd_text_info_maker making text_info object with chunks having"
           " words of size in the range [0,0] creates chunks with no words"
         )
{
  CHECK((rnd_text_info_maker{1,1, 5,5, 0,0})()->chunk_word_count(0)==0);
}

TEST_CASE("blog/sies/rnd_text_info_maker/Zero word-size range - chars"
         , "A rnd_text_info_maker making text_info object with chunks having"
           " words of size in the range [0,0] creates chunks with just spaces"
         )
{
  CHECK((rnd_text_info_maker{1,1, 5,5, 0,0})()->chunk_char_count(0)==5);
  CHECK((rnd_text_info_maker{1,1, 5,5, 0,0})()->chunk_text(0)=="     ");
}

TEST_CASE("blog/sies/rnd_text_info_maker/One word-size range"
         , "A rnd_text_info_maker making text_info object with chunks having"
           " words of size in the range [1,1] creates chunks with 1 char words"
         )
{
  CHECK((rnd_text_info_maker{1,1, 1,1, 1,1})()->chunk_char_count(0)==1+1);
}

TEST_CASE("blog/sies/rnd_text_info_maker/Two word-size range"
         , "A rnd_text_info_maker making text_info object with chunks having"
           " words of size in the range [2,2] creates chunks with 2 char words"
         )
{
  CHECK((rnd_text_info_maker{1,1, 1,1, 2,2})()->chunk_char_count(0)==2+1);
}

TEST_CASE("blog/sies/rnd_text_info_maker/word-size in range"
         , "A rnd_text_info_maker making text_info obj. with chunks with words"
           " of size in the range [7,8] creates chunks with 7 or 8 char words"
         )
{
  auto random_text_info_object(rnd_text_info_maker{1,1, 1,1, 7,8}());
  CHECK((7+1)<=random_text_info_object->chunk_char_count(0));
  CHECK(random_text_info_object->chunk_char_count(0)<=(8+1));
}

