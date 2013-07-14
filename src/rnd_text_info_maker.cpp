// Project: Shared immutable, exclusive setup blog support code C++ library
/// @file rnd_text_info_maker.cpp
/// @brief Type that creates text_info objects with random chunks of text.
///
/// Code accompanying the 
/// "Comments on comments to Herb Sutter's updated GotW #6b solution" series of
/// Dibase blog postings.
///
/// @copyright Copyright (c) Dibase Limited 2013
/// @author Ralph E. McArdell

#include "rnd_text_info_maker.h"

namespace dibase { namespace blog {
  namespace sies // Shared Immutable, Exclusive Setup
  {
    static char const Alphabet[]="aaaaaaaaaaaaaaaabbbccccccdddddddd"
                                 "eeeeeeeeeeeeeeeeeeeeeeeefffffgggghhhhhhhhhhhh"
                                 "iiiiiiiiiiiiiijkkllllllllmmmmmnnnnnnnnnnnnnn"
                                 "oooooooooooooopppqrrrrrrrrrrrrsssssssssssss"
                                 "ttttttttttttttttttuuuuuuvvwwwwxyyyyyz";
    static unsigned int const AlphabetSize{sizeof(Alphabet)};

    rnd_text_info_maker::rnd_text_info_maker
    ( unsigned min_chk, unsigned max_chk
    , unsigned min_wds, unsigned max_wds
    , unsigned min_chrs, unsigned max_chrs
    )
    : rnd_num_text_chunks{min_chk, max_chk}
    , rnd_num_words{rnd_num_text_chunks, min_wds, max_wds}
    , rnd_num_chrs_per_word{rnd_num_text_chunks, min_chrs, max_chrs}
    , rnd_word_letter{rnd_num_text_chunks, 0U,AlphabetSize-1U}
    {}

    std::unique_ptr<text_info> rnd_text_info_maker::operator()()
    {
      std::unique_ptr<text_info> pti{new text_info};
      for (auto c = rnd_num_text_chunks(); c!=0; --c)
        {
          std::string chunk;
          for (auto w = rnd_num_words(); w!=0; --w)
            {
              for (auto l = rnd_num_chrs_per_word(); l!=0; --l)
                {
                  chunk += Alphabet[rnd_word_letter()];
                }
              chunk += ' ';
            }
          pti->add_text_chunk(chunk);
        }
      return pti;
    }
  } // namespace sies
}} // namespaces dibase::blog


