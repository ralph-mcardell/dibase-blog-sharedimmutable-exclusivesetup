// Project: Shared immutable, exclusive setup blog support code C++ library
/// @file text_info.cpp
/// @brief Type used as common core for various pattern examples.
///
/// Code accompanying the 
/// "Comments on comments to Herb Sutter's updated GotW #6b solution" series of
/// Dibase blog postings/
///
/// An example type that presents a set of information that may be added to 
/// (for use in the exclusive single threaded setup phase) and various
/// information on the added data asked for (as would be used in the shared
/// multi-threaded immutable phase).
///
/// @copyright Copyright (c) Dibase Limited 2013
/// @author Ralph E. McArdell

#include "text_info.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

namespace dibase { namespace blog {
  namespace sies // Shared Immutable, Exclusive Setup
  {
    std::string 
        split_next_word(std::string const & text, std::string::size_type & pos)
    {
      char sep_chars[] = " \t\n\r!\"£$%^&*()_-+={}[]:;@'#~?/>.<,\\|¬`";
      std::string::size_type start_pos{text.find_first_not_of(sep_chars,pos)};
      if (start_pos==std::string::npos)
        {
          return "";
        }
      std::string::size_type end_pos{text.find_first_of(sep_chars,start_pos)};
      if (end_pos==std::string::npos)
        {
          end_pos = text.size();
        }
      std::string::size_type len{end_pos - start_pos};
      pos = start_pos + len;
      return text.substr(start_pos, len);    
    }

    static_assert('a'-'A' > 0, "Require 'a'>'A' in character set." );
    void inplace_tolower(std::string & word)
    {
      for (auto & chref : word)
        {
          if ('A'<=chref && chref<='Z')
            {
              chref += 'a'-'A';
            }
        }
    }

bool DEBUG_SHOW{false};
    text_info::chunk_info::chunk_info(std::string chunk_text)
    : chunk{chunk_text}
    , char_count{chunk_text.size()}
    , word_count{0U}
    {
      for(auto chr : chunk_text)
        {
          ++char_occ_map[chr];
        }
      std::string word;
      std::string::size_type pos{0U};
      do
        {
          word = split_next_word(chunk, pos);
          if (!word.empty())
            {
              ++word_count;
              inplace_tolower(word);
              ++word_occ_map[word];
            }
if (DEBUG_SHOW)
{
std::clog << "text_info::chunk_info::chunk_info: word="<<word
          <<", word_count="<<word_count<<std::endl;
}
        }
      while (!word.empty());
    }

    bool text_info::chunk_info::operator==(text_info::chunk_info const & other)
    {
      return    this->char_count==other.char_count
            &&  this->word_count==other.word_count
            &&  this->chunk==other.chunk
            &&  this->char_occ_map==other.char_occ_map
            &&  this->word_occ_map==other.word_occ_map
            ;
    }
  } // namespace sies
}} // namespaces dibase::blog

