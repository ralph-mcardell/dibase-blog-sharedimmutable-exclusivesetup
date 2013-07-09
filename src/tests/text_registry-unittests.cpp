// Project: Shared immutable, exclusive setup blog support code C++ library
/// @file text_registry-unittests.cpp
/// @brief Tests for text_registery.
///
/// Code accompanying the 
/// "Comments on comments to Herb Sutter's updated GotW #6b solution" series of
/// Dibase blog postings.
///
/// @copyright Copyright (c) Dibase Limited 2013
/// @author Ralph E. McArdell

#include "text_registry.h"
#include "catch.hpp"
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

TEST_CASE("blog/sies/text_registry::number_of_chunks/default constructed object"
         ,"A default constructed text_registry object has no chunks"
         )
{
  text_registry<no_sync> tr;
  CHECK(tr.number_of_chunks()==0U);
  CHECK_THROWS_AS(tr.chunk_text(0U), std::out_of_range); 
  CHECK_THROWS_AS(tr.chunk_char_count(0U), std::out_of_range); 
  CHECK_THROWS_AS(tr.chunk_word_count(0U), std::out_of_range); 
  CHECK_THROWS_AS(tr.chunk_char_occurrence(0U,'\0'), std::out_of_range); 
  CHECK_THROWS_AS(tr.chunk_word_occurrence(0U,""), std::out_of_range); 
  CHECK(tr.text()==""); 
  CHECK(tr.char_count()==0U); 
  CHECK(tr.word_count()==0U); 
  CHECK(tr.char_occurrence(' ')==0U); 
  CHECK(tr.word_occurrence("hello")==0U); 
}

TEST_CASE("blog/sies/text_registry::number_of_chunks/creator access during setup"
         ,"Calling operations during setup OK on creating thread"
         )
{
  text_registry<no_sync> tr;
  std::string const chunk0("Hello!");
  tr.add_text_chunk(chunk0);

  CHECK(tr.number_of_chunks()==1U);
  CHECK(tr.chunk_text(0U)==chunk0); 
  CHECK(tr.chunk_char_count(0U)==chunk0.size()); 
  CHECK(tr.chunk_word_count(0U)==1);
  CHECK(tr.chunk_char_occurrence(0U,'!')==1); 
  CHECK(tr.chunk_word_occurrence(0U,"HELLO")==1); 
  CHECK(tr.text()==chunk0); 
  CHECK(tr.char_count()==chunk0.size()); 
  CHECK(tr.word_count()==1); 
  CHECK(tr.char_occurrence('!')==1U); 
  CHECK(tr.word_occurrence("hello")==1U); 
}

TEST_CASE("blog/sies/text_registry::number_of_chunks/noncreator access during setup"
         ,"Calling operations during setup invalid from non-creator threads"
         )
{
  text_registry<no_sync> tr;
  tr.add_text_chunk("Hello");

  std::thread([&tr](){CHECK_THROWS_AS(tr.add_text_chunk("oops!"), call_context_violation);}).join();
  std::thread([&tr](){CHECK_THROWS_AS(tr.number_of_chunks(), call_context_violation);}).join();
  std::thread([&tr](){CHECK_THROWS_AS(tr.chunk_text(0U), call_context_violation);}).join();
  std::thread([&tr](){CHECK_THROWS_AS(tr.chunk_char_count(0U), call_context_violation);}).join();
  std::thread([&tr](){CHECK_THROWS_AS(tr.chunk_word_count(0U), call_context_violation);}).join();
  std::thread([&tr](){CHECK_THROWS_AS(tr.chunk_char_occurrence(0U,'!'), call_context_violation);}).join();
  std::thread([&tr](){CHECK_THROWS_AS(tr.chunk_word_occurrence(0U,"HELLO"), call_context_violation);}).join();
  std::thread([&tr](){CHECK_THROWS_AS(tr.text(), call_context_violation);}).join();
  std::thread([&tr](){CHECK_THROWS_AS(tr.char_count(), call_context_violation);}).join();
  std::thread([&tr](){CHECK_THROWS_AS(tr.word_count(), call_context_violation);}).join();
  std::thread([&tr](){CHECK_THROWS_AS(tr.char_occurrence('!'), call_context_violation);}).join();
  std::thread([&tr](){CHECK_THROWS_AS(tr.word_occurrence("hello"), call_context_violation);}).join();
}

TEST_CASE("blog/sies/text_registry::number_of_chunks/creator access after setup"
         ,"Only read, const, non-mutating operations OK on creating threads"
          " AFTER setup complete."
         )
{
  text_registry<no_sync> tr;
  std::string const chunk0("Hello!");
  std::string const chunk1("hELLO.");
  auto expected_char_size(chunk0.size()+chunk1.size());
  tr.add_text_chunk(chunk0);
  tr.add_text_chunk(chunk1);

  tr.setup_complete();  

  CHECK_THROWS_AS(tr.add_text_chunk("oops!"), call_context_violation);
  CHECK(tr.number_of_chunks()==2U);
  CHECK(tr.chunk_text(0U)==chunk0); 
  CHECK(tr.chunk_char_count(0U)==chunk0.size()); 
  CHECK(tr.chunk_word_count(0U)==1U);
  CHECK(tr.chunk_char_occurrence(0U,'!')==1U); 
  CHECK(tr.chunk_word_occurrence(0U,"HELLO")==1U); 
  CHECK(tr.text()==(chunk0+chunk1)); 
  CHECK(tr.char_count()==expected_char_size); 
  CHECK(tr.word_count()==2U); 
  CHECK(tr.char_occurrence('!')==1U); 
  CHECK(tr.word_occurrence("hello")==2U);
}

TEST_CASE("blog/sies/text_registry::number_of_chunks/noncreator access after setup"
         ,"Only read, const, non-mutating operations OK on non-creator threads"
          " AFTER setup complete."
         )
{
  text_registry<no_sync> tr;
  std::string const chunk0("Hello!");
  tr.add_text_chunk(chunk0);

  tr.setup_complete();  

  std::thread([&tr](){CHECK_THROWS_AS(tr.add_text_chunk("oops!"), call_context_violation);}).join();
  std::thread([&tr](){CHECK(tr.number_of_chunks()==1U);}).join();
  std::thread([&tr,&chunk0](){CHECK(tr.chunk_text(0U)==chunk0);}).join(); 
  std::thread([&tr,&chunk0](){CHECK(tr.chunk_char_count(0U)==chunk0.size());}).join(); 
  std::thread([&tr](){CHECK(tr.chunk_word_count(0U)==1);}).join();
  std::thread([&tr](){CHECK(tr.chunk_char_occurrence(0U,'!')==1);}).join();
  std::thread([&tr](){CHECK(tr.chunk_word_occurrence(0U,"HELLO")==1);}).join(); 
  std::thread([&tr,&chunk0](){CHECK(tr.text()==chunk0);}).join(); 
  std::thread([&tr,&chunk0](){CHECK(tr.char_count()==chunk0.size());}).join(); 
  std::thread([&tr](){CHECK(tr.word_count()==1);}).join(); 
  std::thread([&tr](){CHECK(tr.char_occurrence('!')==1U);}).join(); 
  std::thread([&tr](){CHECK(tr.word_occurrence("hello")==1U);}).join();
}

