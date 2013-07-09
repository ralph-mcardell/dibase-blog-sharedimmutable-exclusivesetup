// Project: Shared immutable, exclusive setup blog support code C++ library
/// @file text_info-unittests.cpp
/// @brief Tests for text_info class and text_info::chunk_info.
///
/// Code accompanying the 
/// "Comments on comments to Herb Sutter's updated GotW #6b solution" series of
/// Dibase blog postings.
///
/// An example type that presents a set of information that may be added to 
/// (for use in the exclusive single threaded setup phase) and various
/// information on the added data asked for (as would be used in the shared
/// multi-threaded immutable phase).
///
/// @copyright Copyright (c) Dibase Limited 2013
/// @author Ralph E. McArdell

#include "text_info.h"
#include "catch.hpp"

using namespace dibase::blog::sies;

TEST_CASE("blog/sies/split_next_word/empty string"
         , "Split word from empty string start returns empty string & 0 position"
         )
{
  std::string::size_type pos{0};
  CHECK(split_next_word("",pos)=="");
  CHECK(pos==0);
}

TEST_CASE("blog/sies/split_next_word/single alpha string"
         ,"Split word from 1 alpha character string start"
          " returns character & position string length"
         )
{
  std::string::size_type pos{0};
  std::string all_alpha_string("c");
  CHECK(split_next_word(all_alpha_string,pos)==all_alpha_string);
  CHECK(pos==all_alpha_string.size());
}

TEST_CASE("blog/sies/split_next_word/single space"
         ,"Split word from single space string start"
          " returns empty string & position 0"
         )
{
  std::string::size_type pos{0};
  std::string all_spaces_string(" ");
  CHECK(split_next_word(all_spaces_string,pos)=="");
  CHECK(pos==0);
}

TEST_CASE("blog/sies/split_next_word/every seperator char"
         ,"Split word from every seperator character string start"
          " returns empty string & position 0"
         )
{
  std::string::size_type pos{0};
  std::string every_sep_string(" \t\n\r!\"£$%^&*()_-+={}[]:;@'#~?/>.<,\\|¬`");
  CHECK(split_next_word(every_sep_string,pos)=="");
  CHECK(pos==0);
}

TEST_CASE("blog/sies/split_next_word/every alphanumeric"
         ,"Split word from every alphanumeric character string start"
          " returns whole string & position string length"
         )
{
  std::string::size_type pos{0};
  std::string every_alphanum_string("abcdefghijklmnopqrstuvwxyz"
                                    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                    "1234567890");
  CHECK(split_next_word(every_alphanum_string,pos)==every_alphanum_string);
  CHECK(pos==every_alphanum_string.size());
}

TEST_CASE("blog/sies/split_next_word/beyond end alphanumeric string"
         ,"Split word from alphanumeric string from beyond string end"
          " returns empty string and no-change to position"
         )
{
  std::string alphanum_string("abc");
  std::string::size_type pos_string_size_plus_1{alphanum_string.size()+1};
  CHECK(split_next_word(alphanum_string,pos_string_size_plus_1)=="");
  CHECK(pos_string_size_plus_1==alphanum_string.size()+1);
}

TEST_CASE("blog/sies/split_next_word/npos pos with alphanumeric string"
         ,"Split word from alphanumeric string with pos of std::string::npos"
          " returns empty string and std::string::npos position"
         )
{
  std::string alphanum_string("abc");
  std::string::size_type pos_npos{std::string::npos};
  CHECK(split_next_word(alphanum_string,pos_npos)=="");
  CHECK(pos_npos==std::string::npos);
}

TEST_CASE("blog/sies/split_next_word/single alpha string at string end"
         ,"Split word from sep chars then alpha char string start"
          " returns character & position string length"
         )
{
  std::string::size_type pos{0};
  std::string alpha_at_end_string(" ,Z");
  CHECK( split_next_word(alpha_at_end_string,pos)
         ==
         alpha_at_end_string.substr(alpha_at_end_string.size()-1)
       );
  CHECK(pos==alpha_at_end_string.size());
}

TEST_CASE("blog/sies/split_next_word/single alpha string followed by seperators"
         ,"Split word from alpha char then sep chars string start"
          " returns character & position 1"
         )
{
  std::string::size_type pos{0};
  std::string alpha_at_start_string("X ,");
  CHECK(split_next_word(alpha_at_start_string,pos)==alpha_at_start_string.substr(0,1));
  CHECK(pos==1);
}

TEST_CASE("blog/sies/split_next_word/single alpha string in single quotes"
         ,"Split word from alpha char between quotes string start"
          " returns character & position 2"
         )
{
  std::string::size_type pos{0};
  std::string alpha_in_quotes_string("'q'");
  CHECK(split_next_word(alpha_in_quotes_string,pos)==alpha_in_quotes_string.substr(1,1));
  CHECK(pos==2);
}

TEST_CASE("blog/sies/split_next_word/multiple single alpha in single quotes"
         ,"Split word from alpha char between quotes string mid"
          " returns character & position after selected character"
         )
{
  unsigned const word_index{1U};  // do not use 0!
  unsigned const field_width{3U};
  unsigned const word_offset{1};
  std::string::size_type pos{word_index*field_width-1};
  std::string three_quoted_alpha_string("'p''q''r'");
  CHECK(split_next_word(three_quoted_alpha_string,pos)
        ==
        three_quoted_alpha_string.substr(word_index*field_width+word_offset,1)
       );
  CHECK(pos==word_index*field_width+word_offset+1);
}


TEST_CASE("blog/sies/inplace_tolower/empty string"
         ,"Converting empty string to lowercase yields empty string"
         )
{
  std::string const empty_string;
  REQUIRE(empty_string=="");
  auto lc_string(empty_string);
  inplace_tolower(lc_string);
  CHECK(lc_string==empty_string);
}

TEST_CASE("blog/sies/inplace_tolower/every non-alpha character string"
         ,"Converting string of every non-alpha character to lowercase"
          " yields no change in the string"
         )
{
  std::string const every_nonalpha_char_string
                    (" \t\n\r!\"£$%^&*()+={}[]:;@'#~?/>.<,\\|¬`1234567890");
  auto lc_string(every_nonalpha_char_string);
  inplace_tolower(lc_string);
  CHECK(lc_string==every_nonalpha_char_string);
}

TEST_CASE("blog/sies/inplace_tolower/every lowercase alpha character string"
         ,"Converting string of every lowercase letter character to lowercase"
          " yields no change in the string"
         )
{
  std::string const every_lc_letter_char_string("abcdefghijklmnopqrstuvwxyz");
  auto lc_string(every_lc_letter_char_string);
  inplace_tolower(lc_string);
  CHECK(lc_string==every_lc_letter_char_string);
}

TEST_CASE("blog/sies/inplace_tolower/every uppercase alpha character string"
         ,"Converting string of every uppercase letter character to lowercase"
          " yields string of every uppercase character"
         )
{
  std::string const every_uc_letter_char_string("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  auto lc_string(every_uc_letter_char_string);
  inplace_tolower(lc_string);
  std::string const every_lc_letter_char_string("abcdefghijklmnopqrstuvwxyz");
  CHECK(lc_string==every_lc_letter_char_string);
}


TEST_CASE("blog/sies/text_info::chunk_info/construct with empty string"
         ,"Constructing a text_info::chunk_info object with an empty string"
          " is equivalent to a default constructed text_info::chunk_info object"
         )
{
  CHECK(text_info::chunk_info{""}==text_info::chunk_info{});
}

TEST_CASE("blog/sies/text_info::chunk_info/construct with wordy string"
         ,"Constructing a text_info::chunk_info object with an wordy string"
          " collects correct values for that string"
         )
{
  auto expected(text_info::chunk_info{});
  expected.chunk = "The quick brownie crossed the road.";
  expected.char_count = expected.chunk.size();
  expected.word_count = 6;
  expected.char_occ_map[' '] = 5;
  expected.char_occ_map['.'] = 1;
  expected.char_occ_map['T'] = 1;
  expected.char_occ_map['a'] = 1;
  expected.char_occ_map['b'] = 1;
  expected.char_occ_map['c'] = 2;
  expected.char_occ_map['d'] = 2;
  expected.char_occ_map['e'] = 4;
  expected.char_occ_map['h'] = 2;
  expected.char_occ_map['i'] = 2;
  expected.char_occ_map['k'] = 1;
  expected.char_occ_map['n'] = 1;
  expected.char_occ_map['o'] = 3;
  expected.char_occ_map['q'] = 1;
  expected.char_occ_map['r'] = 3;
  expected.char_occ_map['s'] = 2;
  expected.char_occ_map['t'] = 1;
  expected.char_occ_map['u'] = 1;
  expected.char_occ_map['w'] = 1;
  expected.word_occ_map["brownie"] = 1;
  expected.word_occ_map["crossed"] = 1;
  expected.word_occ_map["quick"] = 1;
  expected.word_occ_map["road"] = 1;
  expected.word_occ_map["the"] = 2;
  CHECK(text_info::chunk_info{expected.chunk}==expected);
}


TEST_CASE("blog/sies/text_info::number_of_chunks/default constructed object"
         ,"A default constructed text_info object has no chunks"
         )
{
  text_info ti;
  CHECK(ti.number_of_chunks()==0U);
}

TEST_CASE("blog/sies/text_info::number_of_chunks/one chunk added"
         ,"A default constructed text_info object with one chunk added"
          "reports 1 from call to number_of_chunks"
         )
{
  text_info ti;
  ti.add_text_chunk("chunk1");
  CHECK(ti.number_of_chunks()==1U);
}

TEST_CASE("blog/sies/text_info::number_of_chunks/two chunks added"
         ,"A default constructed text_info object with two chunks added"
          "reports 2 from call to number_of_chunks"
         )
{
  text_info ti;
  ti.add_text_chunk("chunk1");
  ti.add_text_chunk("chunk2");
  CHECK(ti.number_of_chunks()==2U);
}

TEST_CASE("blog/sies/text_info::chunk_text/out of range chunk"
         ,"Asking for the chunk_text of an out of range chunk"
          " throws a std::out_of_range exception."
         )
{
  text_info ti; // no chunks hence any chunk index value is invalid!
  CHECK_THROWS_AS(ti.chunk_text(0), std::out_of_range); 
}

TEST_CASE("blog/sies/text_info::chunk_text/text of single chunk"
         ,"Asking for the text of the one and only chunk"
          " returns that chunk's text."
         )
{
  text_info ti;
  std::string const chunk0{"Chunk0"};
  ti.add_text_chunk(chunk0);
  CHECK(ti.chunk_text(0)==chunk0); 
}

TEST_CASE("blog/sies/text_info::chunk_text/text of 2nd chunk"
         ,"Asking for the chunk_text of a 2nd chunk"
          " returns that chunk's text."
         )
{
  text_info ti;
  std::string const chunk1{"Chunk1"};
  ti.add_text_chunk("Not what we want");
  ti.add_text_chunk(chunk1);
  CHECK(ti.chunk_text(1)==chunk1); 
}

TEST_CASE("blog/sies/text_info::chunk_char_count/out of range chunk"
         ,"Asking for the chunk_char_count of an out of range chunk"
          " throws a std::out_of_range exception."
         )
{
  text_info ti; // no chunks hence any chunk index value is invalid!
  CHECK_THROWS_AS(ti.chunk_char_count(0), std::out_of_range); 
}

TEST_CASE("blog/sies/text_info::chunk_char_count/count of single chunk"
         ,"Asking for the chunk_char_count of the one and only chunk"
          " returns that chunk's size."
         )
{
  text_info ti;
  auto const expected_count(10U);
  ti.add_text_chunk(std::string(expected_count,'a'));
  CHECK(ti.chunk_char_count(0)==expected_count); 
}

TEST_CASE("blog/sies/text_info::chunk_char_count/count of 2nd chunk"
         ,"Asking for the chunk_char_count of a 2nd chunk"
          " returns that chunk's size."
         )
{
  text_info ti;
  auto const expected_count(10U);
  ti.add_text_chunk(std::string(expected_count*2,'a'));
  ti.add_text_chunk(std::string(expected_count,' '));
  CHECK(ti.chunk_char_count(1)==expected_count); 
}

TEST_CASE("blog/sies/text_info::chunk_word_count/out of range chunk"
         ,"Asking for the chunk_word_count of an out of range chunk"
          " throws a std::out_of_range exception."
         )
{
  text_info ti; // no chunks hence any chunk index value is invalid!
  CHECK_THROWS_AS(ti.chunk_word_count(0), std::out_of_range); 
}

TEST_CASE("blog/sies/text_info::chunk_word_count/count of single chunk"
         ,"Asking for the chunk_word_count of the one and only chunk"
          " returns the number of words in that chunk."
         )
{
  text_info ti;
  ti.add_text_chunk(std::string(10,'a'));
  CHECK(ti.chunk_word_count(0)==1); 
}

TEST_CASE("blog/sies/text_info::chunk_word_count/count of 2nd chunk"
         ,"Asking for the chunk_word_count of the 2nd chunk"
          " returns the number of words in that chunk."
         )
{
  text_info ti;
  ti.add_text_chunk(std::string(10,'a'));
  ti.add_text_chunk(std::string(10,' '));
  CHECK(ti.chunk_word_count(1)==0); 
}

TEST_CASE("blog/sies/text_info::chunk_word_count/count of wordy text"
         ,"Asking for the chunk_word_count in a wordy text chunk"
          " returns the number of words in that chunk."
         )
{
  text_info ti;
  ti.add_text_chunk("  Word1, word2\n:word3.");
  CHECK(ti.chunk_word_count(0)==3); 
}

TEST_CASE("blog/sies/text_info::chunk_char_occurrence/out of range chunk"
         ,"Asking for the chunk_char_occurrence of an out of range chunk"
          " throws a std::out_of_range exception."
         )
{
  text_info ti; // no chunks hence any chunk index value is invalid!
  CHECK_THROWS_AS(ti.chunk_char_occurrence(0,'\0'), std::out_of_range); 
}

TEST_CASE("blog/sies/text_info::chunk_char_occurrence/single chunk char occurrence"
         ,"Asking for the chunk_char_frequencies of the only and other char in"
          " the only chunk returns count of chars and zero respectfully."
         )
{
  text_info ti;
  auto const expected_count(10U);
  ti.add_text_chunk(std::string(expected_count,'a'));
  CHECK(ti.chunk_char_occurrence(0,'a')==expected_count); 
  CHECK(ti.chunk_char_occurrence(0,'b')==0); 
}

TEST_CASE("blog/sies/text_info::chunk_char_occurrence/2nd chunk various chars"
         ,"Asking for the chunk_char_frequencies of the 2nd chunk returns"
          " char occurrence count for each char correctly.."
         )
{
  text_info ti;
  ti.add_text_chunk(std::string(3,'a'));
  ti.add_text_chunk("z yy xxx wwww");
  CHECK(ti.chunk_char_occurrence(1,'?')==0);
  CHECK(ti.chunk_char_occurrence(1,'z')==1);
  CHECK(ti.chunk_char_occurrence(1,'y')==2);
  CHECK(ti.chunk_char_occurrence(1,'x')==3);
  CHECK(ti.chunk_char_occurrence(1,'w')==4);
}

TEST_CASE("blog/sies/text_info::chunk_word_occurrence/out of range chunk"
         ,"Asking for the chunk_word_occurrence of an out of range chunk"
          " throws a std::out_of_range exception."
         )
{
  text_info ti; // no chunks hence any chunk index value is invalid!
  CHECK_THROWS_AS(ti.chunk_word_occurrence(0,""), std::out_of_range); 
}

TEST_CASE("blog/sies/text_info::chunk_word_occurrence/single chunk word frequencies"
         ,"Asking for the chunk_word_occurrence of the single word and"
          "  some other word returns 1` and 0 respectfully"
         )
{
  text_info ti;
  std::string const the_word("the");
  std::string const other_word("other");
  ti.add_text_chunk(the_word);
  CHECK(ti.chunk_word_occurrence(0,the_word)==1); 
  CHECK(ti.chunk_word_occurrence(0,other_word)==0);
}

TEST_CASE("blog/sies/text_info::chunk_word_occurrence/2nd chunk word frequencies"
         ,"Asking for the chunk_word_occurrence of various words of a 2nd chunk"
          "  returns the expected values."
         )
{
  text_info ti;
  ti.add_text_chunk("tra la la");
  ti.add_text_chunk("'123'>.|z-@123?y*xx:y+\n; 123 xx;/123     xx::<<<<");
  CHECK(ti.chunk_word_occurrence(1,"nosuch")==0);
  CHECK(ti.chunk_word_occurrence(1,"z")==1);
  CHECK(ti.chunk_word_occurrence(1,"y")==2);
  CHECK(ti.chunk_word_occurrence(1,"Xx")==3);
  CHECK(ti.chunk_word_occurrence(1,"123")==4);
}

TEST_CASE("blog/sies/text_info::chunk_word_occurrence/case insensitive"
         ,"The chunk_word_occurrence of a word in a chunk is case insensive"
         )
{
  text_info ti;
  ti.add_text_chunk("CaSe");
  CHECK(ti.chunk_word_occurrence(0,"CaSe")==1); 
  CHECK(ti.chunk_word_occurrence(0,"case")==1); 
  CHECK(ti.chunk_word_occurrence(0,"CASE")==1); 
  CHECK(ti.chunk_word_occurrence(0,"cAsE")==1); 
}

TEST_CASE("blog/sies/text_info::text/no chunks"
         ,"Asking for the text of a text_info object with no chunks"
          " should yield an empty string."
         )
{
  text_info ti;
  CHECK(ti.text()==""); 
}

TEST_CASE("blog/sies/text_info::text/one chunk"
         ,"Asking for the text of a text_info object with a single chunk"
          " should yield a string containing the text of that chunk."
         )
{
  text_info ti;
  std::string the_chunk_text(7,'*');
  ti.add_text_chunk(the_chunk_text);
  CHECK(ti.text()==the_chunk_text); 
}

TEST_CASE("blog/sies/text_info::text/many chunks"
         ,"Asking for the char_count of a text_info object with a several chunks"
          " should yield the sum of the size of each chunk."
         )
{
  text_info ti;
  std::string const chunk_text[]={"HHHH", "$!&%", "___", "some text"};
  std::string expected_text;
  for (auto const & ct : chunk_text)
    {
       expected_text += ct;
       ti.add_text_chunk(ct);
    }
  CHECK(ti.text()==expected_text); 
}

TEST_CASE("blog/sies/text_info::char_count/no chunks"
         ,"Asking for the char_count of a text_info object with no chunks"
          " should yield zero."
         )
{
  text_info ti;
  CHECK(ti.char_count()==0); 
}

TEST_CASE("blog/sies/text_info::char_count/one chunk"
         ,"Asking for the char_count of a text_info object with a single chunk"
          " should yield the size of that chunk."
         )
{
  text_info ti;
  std::string the_chunk_text(7,'*');
  ti.add_text_chunk(the_chunk_text);
  CHECK(ti.char_count()==the_chunk_text.size()); 
}

TEST_CASE("blog/sies/text_info::char_count/many chunks"
         ,"Asking for the char_count of a text_info object with a several chunks"
          " should yield the sum of the size of each chunk."
         )
{
  text_info ti;
  std::string const chunk_text[]={"HHHH", "$!&%", "___", "some text"};
  auto expected_count(0U);
  for (auto const & ct : chunk_text)
    {
       expected_count += ct.size();
       ti.add_text_chunk(ct);
    }
  CHECK(ti.char_count()==expected_count); 
}

TEST_CASE("blog/sies/text_info::word_count/no chunks"
         ,"Asking for the word_count of a text_info object with no chunks"
          " should yield zero."
         )
{
  text_info ti;
  CHECK(ti.word_count()==0); 
}

TEST_CASE("blog/sies/text_info::word_count/single chunk"
         ,"Asking for the word_count of a text_info object with a single chunk"
          " should yield the number of words in that chunk."
         )
{
  text_info ti;
  ti.add_text_chunk("Hello world!");
  CHECK(ti.word_count()==ti.chunk_word_count(0)); 
}

TEST_CASE("blog/sies/text_info::word_count/many chunks"
         ,"Asking for the word_count of a text_info object with a several chunks"
          " should yield the sum of the words in each chunk."
         )
{
  text_info ti;
  std::string const chunk_text[]={"a 123", "$!it&%se", "__var_", "some text"};
  auto expected_count(0U);
  for (unsigned i{0}; i<sizeof(chunk_text)/sizeof(std::string); ++i)
    {
       ti.add_text_chunk(chunk_text[i]);
       expected_count += ti.chunk_word_count(i);
    }
  CHECK(ti.word_count()==expected_count); 
}

TEST_CASE("blog/sies/text_info::char_occurrence/no chunks"
         ,"Asking for the char_occurrence of character in a a text_info object"
          " with no chunks should yield zero."
         )
{
  text_info ti;
  for (unsigned char c; c<255; ++c)
    {
      CHECK(ti.char_occurrence(c)==0); 
    }
}

TEST_CASE("blog/sies/text_info::char_occurrence/one chunks"
         ,"Asking for the char_occurrence of character in a a text_info object"
          " with one chunk should yield value for that chunk."
         )
{
  text_info ti;
  ti.add_text_chunk("122333");
  CHECK(ti.char_occurrence('0')==ti.chunk_char_occurrence(0, '0'));
  CHECK(ti.char_occurrence('X')==ti.chunk_char_occurrence(0, 'X'));
  CHECK(ti.char_occurrence('1')==ti.chunk_char_occurrence(0, '1'));
  CHECK(ti.char_occurrence('2')==ti.chunk_char_occurrence(0, '2'));
  CHECK(ti.char_occurrence('3')==ti.chunk_char_occurrence(0, '3'));
}

TEST_CASE("blog/sies/text_info::char_occurrence/many chunks"
         ,"Asking for the char_occurrence of character in a a text_info object"
          " with many chunks should yield the cumulative value across all chunks."
         )
{
  text_info ti;
  std::string const chunk_text[]={"01234", "13579", "02468", "2357", "112358"};
  unsigned expected_values[10]={0U};
  for (unsigned i{0}; i<sizeof(chunk_text)/sizeof(std::string); ++i)
    {
      ti.add_text_chunk(chunk_text[i]);
      for (unsigned digit{0}; digit<10; ++digit)
        {
          expected_values[digit] += ti.chunk_char_occurrence(i,'0'+digit);          
        }
    }
  for (unsigned digit{0}; digit<10; ++digit)
    {
      CHECK(ti.char_occurrence('0'+digit)==expected_values[digit]);          
    }
  CHECK(ti.char_occurrence('@')==0);
}

TEST_CASE("blog/sies/text_info::word_occurrence/no chunks"
         ,"Asking for the word_occurrence of character in a a text_info object"
          " with no chunks should yield zero."
         )
{
  text_info ti;
  CHECK(ti.word_occurrence("word")==0); 
}

TEST_CASE("blog/sies/text_info::word_occurrence/one chunk"
         ,"Asking for the word_occurrence of word in a a text_info object"
          " with one chunk should yield value for that chunk."
         )
{
  text_info ti;
  ti.add_text_chunk("The the were a band. The band the-the awareness!");
  CHECK(ti.word_occurrence("nosuch")==ti.chunk_word_occurrence(0, "nosuch"));
  CHECK(ti.word_occurrence("the")==ti.chunk_word_occurrence(0, "The"));
  CHECK(ti.word_occurrence("WERE")==ti.chunk_word_occurrence(0, "were"));
  CHECK(ti.word_occurrence("a")==ti.chunk_word_occurrence(0, "a"));
  CHECK(ti.word_occurrence("band")==ti.chunk_word_occurrence(0, "band"));
  CHECK(ti.word_occurrence("awareness")==ti.chunk_word_occurrence(0, "awareness"));
}

TEST_CASE("blog/sies/text_info::word_occurrence/many chunks"
         ,"Asking for the word_occurrence of character in a a text_info object"
          " with many chunks should yield the cumulative value across all chunks."
         )
{
  text_info ti;
  std::string const chunk_text[]={ "0 1 2 3 4", "1 3 5 7 9"
                                 , "0 2 4 6 8", "2 3 5 7"
                                 , "1;1;2;3;5;8", "0,2,4,8"
                                 };
  unsigned expected_values[10]={0U};
  for (unsigned i{0}; i<sizeof(chunk_text)/sizeof(std::string); ++i)
    {
      ti.add_text_chunk(chunk_text[i]);
      for (unsigned digit{0}; digit<10; ++digit)
        {
          expected_values[digit] 
                        += ti.chunk_word_occurrence(i,std::string(1,'0'+digit));          
        }
    }
  for (unsigned digit{0}; digit<10; ++digit)
    {
      CHECK(ti.word_occurrence(std::string(1,'0'+digit))==expected_values[digit]);          
    }
  CHECK(ti.word_occurrence("notthere")==0);
}


