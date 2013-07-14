// Project: Shared immutable, exclusive setup blog support code C++ library
/// @file rnd_text_info_maker.h
/// @brief Type that creates text_info objects with random chunks of text.
///
/// Code accompanying the 
/// "Comments on comments to Herb Sutter's updated GotW #6b solution" series of
/// Dibase blog postings.
///
/// @copyright Copyright (c) Dibase Limited 2013
/// @author Ralph E. McArdell

#ifndef DIBASE_BLOG_SIES_RND_TEXT_INFO_MAKER_H
# define DIBASE_BLOG_SIES_RND_TEXT_INFO_MAKER_H
# include "random_in_range.h"
# include "text_info.h"

namespace dibase { namespace blog {
  namespace sies // Shared Immutable, Exclusive Setup
  {
  /// @brief Class to make text_info objects with random chunks of text.
  ///
  /// Each instance is constructed from value ranges for number of chunks, words
  /// per chunk and characters per word. Words are made up of charaters choosen
  /// from an alphabet string of lowercase English letters [a-z] with repeats
  /// of letters based roughly on letter distribution in English words.
  ///
  /// Made text_info objects will have number of chunks, words per chunk and
  /// characters per word selected randomly from these ranges and letters
  /// within each word randomly choosen from the alphabet string.
    class rnd_text_info_maker
    {
      random_in_range rnd_num_text_chunks;
      random_in_range rnd_num_words;
      random_in_range rnd_num_chrs_per_word;
      random_in_range rnd_word_letter;

    public:
    /// @brief Construct from number of chunks, words and characters ranges.
    /// @param min_chk  Minimum chunks in created text_info objects
    /// @param max_chk  Maximum chunks in created text_info objects
    /// @param min_wds  Minimum words in created text_info objects' chunks
    /// @param max_wds  Maximum words in created text_info objects' chunks
    /// @param min_chrs Minimum characters in text_info objects' chunks' words
    /// @param max_chrs Minimum characters in text_info objects' chunks' words
      rnd_text_info_maker ( unsigned min_chk, unsigned max_chk
                          , unsigned min_wds, unsigned max_wds
                          , unsigned min_chrs, unsigned max_chrs
                          );

      rnd_text_info_maker(rnd_text_info_maker const &) = delete;
      rnd_text_info_maker & operator=(rnd_text_info_maker const &) = delete;
      rnd_text_info_maker(rnd_text_info_maker &&) = delete;
      rnd_text_info_maker & operator=(rnd_text_info_maker &&) = delete;

    /// @brief Maker call operator: create, setup and return text_info object.
    /// @return Managed unique pointer to new, randomly setup text_info object.
      std::unique_ptr<text_info> operator()();
    };
  } // namespace sies
}} // namespaces dibase::blog
#endif // DIBASE_BLOG_SIES_RND_TEXT_INFO_MAKER_H

