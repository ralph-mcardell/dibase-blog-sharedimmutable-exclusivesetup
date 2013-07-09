// Project: Shared immutable, exclusive setup blog support code C++ library
/// @file text_registry.h
/// @brief Shared-immutable, exclusive setup type using call context validation.
///
/// Code accompanying the 
/// "Comments on comments to Herb Sutter's updated GotW #6b solution" series of
/// Dibase blog postings.
///
/// An example type template that uses call context validation to enforce the 
/// Shared Immutable Exclusive Setup pattern that can be used to create classes
/// using various atomic and memory order synchronisation policies.
///
/// @copyright Copyright (c) Dibase Limited 2013
/// @author Ralph E. McArdell

#ifndef DIBASE_BLOG_SIES_TEXT_REGISRTY_H
# define DIBASE_BLOG_SIES_TEXT_REGISRTY_H
# include "call_context_validator.h"
# include "text_info.h"

namespace dibase { namespace blog {
  namespace sies // Shared Immutable, Exclusive Setup
  {
    template <template <class T> class SyncPolicy>
    class text_registry
    {
      call_context_validator<text_registry, SyncPolicy>
                  validate_usage;
      text_info   data;
      
    public:
      typedef text_info::chunk_size_type      chunk_size_type;
      typedef text_info::chunk_count_type     chunk_count_type;
      typedef text_info::chunk_index_type     chunk_index_type;

  private:
  // Cached data values - only valid once object setup complete
      chunk_size_type  final_char_count;
      chunk_size_type  final_word_count;

  public:
      text_registry() = default;
      text_registry(text_registry const &) = delete;
      text_registry & operator=(text_registry const &) = delete;
      text_registry(text_registry &&) = delete;
      text_registry & operator=(text_registry &&) = delete;

    /// @brief Called when all muatating calls setting up the object are done.
    /// Completing setting is considered a mutable operation as it publishes
    /// all updates and may perform final cached value calculating operations.
    /// @throws dibase::blog::sies::call_context_violation if called by
    ///         thread other than the creator thread or if the object
    ///         has completed its setup and has become immutable.
      void setup_complete()
      {
        final_char_count = char_count(); // Set cached values then publish
        final_word_count = word_count();
        validate_usage.publish(this);
      }

    /// @brief Mutable operation. Add a chunk of text to an object.
    /// Creates a chunk_info object from text and pushes to the end of the
    /// sequence of chunks.
    /// @param text Text string chunk to add to object.
    /// @throws dibase::blog::sies::call_context_violation if called by
    ///         thread other than the creator thread or if the object
    ///         has completed its setup and has become immutable.
      void add_text_chunk(std::string const & text)
      {
        validate_usage(this);
        data.add_text_chunk(text);
      }

    /// @brief Immutable operation. Returns number of text chunks in object.
    /// @returns Number of entries in chunk sequence.
    /// @throws dibase::blog::sies::call_context_violation if called by
    ///         thread other than the creator thread while the object
    ///         is still being setup and is still mutable.
      chunk_count_type number_of_chunks() const 
      {
        validate_usage(this);
        return data.number_of_chunks(); 
      }

    /// @brief Immutable operation. Returns text of a chunk.
    /// @param chunk_index  Index of chunk in sequence of chunks
    /// @returns Copy of the string text of the chunk
    /// @throws dibase::blog::sies::call_context_violation if called by
    ///         thread other than the creator thread while the object
    ///         is still being setup and is still mutable.
    /// @throws std::out_of_range if chunk_index is greater or equal to the
    ///         value returned by number_of_chunks.
      std::string  chunk_text(chunk_index_type chunk_index) const
      {
        validate_usage(this);
        return data.chunk_text(chunk_index);
      }

    /// @brief Immutable operation. Returns number of characters in a chunk.
    /// @param chunk_index  Index of chunk in sequence of chunks
    /// @returns Number of characters in specfied chunk
    /// @throws dibase::blog::sies::call_context_violation if called by
    ///         thread other than the creator thread while the object
    ///         is still being setup and is still mutable.
    /// @throws std::out_of_range if chunk_index is greater or equal to the
    ///         value returned by number_of_chunks.
      chunk_size_type  chunk_char_count(chunk_index_type chunk_index) const
      {
        validate_usage(this);
        return data.chunk_char_count(chunk_index);
      }

    /// @brief Immutable operation. Returns number of word in a chunk.
    /// Word defined as contiguous sequence of [a-z][A-Z][0-9] characters.
    /// @param chunk_index  Index of chunk in sequence of chunks
    /// @returns Number of words in specfied chunk
      chunk_size_type  chunk_word_count(chunk_index_type chunk_index) const
      {
        validate_usage(this);
        return data.chunk_word_count(chunk_index);
      }

    /// @brief Immutable operation. Returns occurrence of a character in a chunk.
    /// @param chunk_index  Index of chunk in sequence of chunks.
    /// @param chr          Character to return occurrence for.
    /// @returns occurrence of chr in specfied chunk.
    /// @throws dibase::blog::sies::call_context_violation if called by
    ///         thread other than the creator thread while the object
    ///         is still being setup and is still mutable.
    /// @throws std::out_of_range if chunk_index is greater or equal to the
    ///         value returned by number_of_chunks.
      chunk_size_type  chunk_char_occurrence
      ( chunk_index_type chunk_index
      , char chr
      ) const
      {
        validate_usage(this);
        return data.chunk_char_occurrence(chunk_index,chr);
      }

    /// @brief Immutable operation. Returns occurrence of a word in a chunk.
    /// Word defined as contiguous sequence of [a-z][A-Z][0-9] characters.
    /// @param chunk_index  Index of chunk in sequence of chunks
    /// @param word         Word to return occurrence for.
    /// @returns occurrence of word in specfied chunk.
    /// @throws dibase::blog::sies::call_context_violation if called by
    ///         thread other than the creator thread while the object
    ///         is still being setup and is still mutable.
    /// @throws std::out_of_range if chunk_index is greater or equal to the
    ///         value returned by number_of_chunks.
      chunk_size_type  chunk_word_occurrence
      ( chunk_index_type chunk_index
      , std::string const & word
      ) const
      {
        validate_usage(this);
        return data.chunk_word_occurrence(chunk_index,word);
      }

    /// @brief Immutable operation. Returns concatenation of all chunks' text.
    /// @returns Concatenated text of all chunks
    /// @throws dibase::blog::sies::call_context_violation if called by
    ///         thread other than the creator thread while the object
    ///         is still being setup and is still mutable.
      std::string  text() const
      {
        validate_usage(this);
        return data.text(); 
      }

    /// @brief Immutable operation. Returns number of characters in all chunks.
    /// @returns Cumulative number of characters in all chunks
    /// @throws dibase::blog::sies::call_context_violation if called by
    ///         thread other than the creator thread while the object
    ///         is still being setup and is still mutable.
      chunk_size_type  char_count() const
      {
        validate_usage(this);
        return validate_usage.published() ? final_char_count
                                          : data.char_count()
                                          ; 
      }

    /// @brief Immutable operation. Returns number of words in all chunks.
    /// @returns Cumulative number of words in all chunks
    /// @throws dibase::blog::sies::call_context_violation if called by
    ///         thread other than the creator thread while the object
    ///         is still being setup and is still mutable.
      chunk_size_type  word_count() const
      {
        validate_usage(this);
        return validate_usage.published() ? final_word_count
                                          : data.word_count()
                                          ;
      }

    /// @brief Immutable operation. Returns occurrence of character in all chunks
    /// @param chr          Character to return occurrence for.
    /// @returns Cumulative occurrence of chr in all chunks.
    /// @throws dibase::blog::sies::call_context_violation if called by
    ///         thread other than the creator thread while the object
    ///         is still being setup and is still mutable.
      chunk_size_type  char_occurrence(char chr) const
      {
        validate_usage(this);
        return data.char_occurrence(chr); 
      }

    /// @brief Immutable operation. Returns occurrence of a word in all chunks
    /// @param word          Word to return occurrence for.
    /// @returns Cumulative occurrence of word in all chunks.
    /// @throws dibase::blog::sies::call_context_violation if called by
    ///         thread other than the creator thread while the object
    ///         is still being setup and is still mutable.
      chunk_size_type  word_occurrence(std::string const & word) const
      {
        validate_usage(this);
        return data.word_occurrence(word); 
      }
    };
  } // namespace sies
}} // namespaces dibase::blog
#endif // DIBASE_BLOG_SIES_TEXT_REGISRTY_H

