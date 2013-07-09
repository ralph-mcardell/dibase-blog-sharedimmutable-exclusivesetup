// Project: Shared immutable, exclusive setup blog support code C++ library
/// @file text_info.h
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

#ifndef DIBASE_BLOG_SIES_TEXT_INFO_H
# define DIBASE_BLOG_SIES_TEXT_INFO_H
# include <string>
# include <map>
# include <vector>
# include <numeric>

namespace dibase { namespace blog {
  namespace sies // Shared Immutable, Exclusive Setup
  {
  /// @brief Return next word from given position in string
  ///
  /// 'Word' is a consecutive sequence of [A-Z][a-z][0-9] characters.
  /// @param text   String to return 'next' word from.
  /// @param pos    0-based position to start search in Text for word. 
  ///               Updated and output to position following that of end of
  ///               returned word.
  /// @return Next word from pos in text. Empty if no word found.
    std::string 
        split_next_word(std::string const & text, std::string::size_type & pos);

  /// @brief Replaces [A-Z] with [a-z] in place within the passed string.
  /// @param [in,out] str  String that will be modified to make lowercase.
    void inplace_tolower(std::string & str);

  /// @brief Replaces [A-Z] with [a-z] in a copy of the passed string.
  /// @param str  String to make lowercase.
  /// @returns Lowercase copy of str.
    inline
    std::string tolower(std::string const & str)
    {
      auto lc_str(str);
      inplace_tolower(lc_str);
      return lc_str;
    }

  /// @brief Object type having various data-fields that should be setup
  /// 
  /// Type contains an vector of structs containing information on chunks of
  /// of text. The type deliberately has to have chunks added to it _after_
  /// construction and has a set of const, information providing member
  /// functions.
  ///
  /// Intended as a common provider of vaguely interesting example services
  /// for use by various pattern-implementation examples.
    class text_info
    {
    public:
    /// @brief Internal type used to hold information on one chunk of text
      struct chunk_info
      {
        typedef std::string::size_type                chunk_size_type;
        typedef std::map<char,chunk_size_type>        char_occ_map_type;
        typedef std::map<std::string,chunk_size_type> word_occ_map_type;
        std::string chunk;
        std::string::size_type  char_count;
        std::string::size_type  word_count;
        char_occ_map_type char_occ_map;
        word_occ_map_type word_occ_map;

        chunk_info()
        : char_count{0U}
        , word_count{0U}
        {}
        chunk_info(std::string chunk_text);
        bool operator==(text_info::chunk_info const & other);
        bool operator!=(text_info::chunk_info const & other)
        {
          return !(*this==other);
        }
      };

      typedef chunk_info::chunk_size_type chunk_size_type;

    private:
      typedef std::vector<chunk_info>     chunk_vector;

      chunk_vector  text_data; ///< The data member - sequence of text chunks

    /// @brief Helper: look up item in map and returns value or zero.
    ///
    /// Function template to cover both types of occurrence map used by
    /// chunk_info.
    ///
    /// @param occ_map    : Occurrence map to perform lookup on.
    /// @param key        : Key used (char or std::string) to lookup value.
    /// @returns occurrence value for key or 0 if no entry for key in occ_map.
      template <typename KeyT>
      static chunk_size_type lookup_occurrence
      ( std::map<KeyT, chunk_size_type> const & occ_map
      , KeyT key
      )
      {
        auto pos(occ_map.find(key));
        return (pos==occ_map.end())?0U:pos->second;
      }

    public:
      typedef chunk_vector::size_type     chunk_count_type;
      typedef chunk_count_type            chunk_index_type;

      text_info() = default;
      text_info(text_info const &) = delete;
      text_info(text_info &&) = delete;
      text_info & operator=(text_info const &) = delete;
      text_info & operator=(text_info &&) = delete;

    /// @brief Mutable operation. Add a chunk of text to an object.
    /// Creates a chunk_info object from text and pushes to the end of the
    /// sequence of chunks.
    /// @param text Text string chunk to add to object.
      void add_text_chunk(std::string const & text)
      {
        text_data.push_back(text_info::chunk_info{text});
      }

    /// @brief Immutable operation. Returns number of text chunks in object.
    /// @returns Number of entries in chunk sequence.
      chunk_count_type number_of_chunks() const { return text_data.size(); }

    /// @brief Immutable operation. Returns copy the chunk text.
    /// @param chunk_index  Index of chunk in sequence of chunks
    /// @returns Copy of the string text of the chunk
    /// @throws std::out_of_range if chunk_index is greater or equal to the
    ///         value returned by number_of_chunks.
      std::string  chunk_text(chunk_index_type chunk_index) const
      {
        return text_data.at(chunk_index).chunk;
      }

    /// @brief Immutable operation. Returns number of characters in a chunk.
    /// @param chunk_index  Index of chunk in sequence of chunks
    /// @returns Number of characters in specfied chunk
    /// @throws std::out_of_range if chunk_index is greater or equal to the
    ///         value returned by number_of_chunks.
      chunk_size_type  chunk_char_count(chunk_index_type chunk_index) const
      {
        return text_data.at(chunk_index).char_count;
      }

    /// @brief Immutable operation. Returns number of word in a chunk.
    /// Word defined as contiguous sequence of [a-z][A-Z][0-9] characters.
    /// @param chunk_index  Index of chunk in sequence of chunks
    /// @returns Number of words in specfied chunk
    /// @throws std::out_of_range if chunk_index is greater or equal to the
    ///         value returned by number_of_chunks.
      chunk_size_type  chunk_word_count(chunk_index_type chunk_index) const
      {
        return text_data.at(chunk_index).word_count;
      }

    /// @brief Immutable operation. Returns occurrence of a character in a chunk.
    /// @param chunk_index  Index of chunk in sequence of chunks.
    /// @param chr          Character to return occurrence for.
    /// @returns occurrence of chr in specfied chunk.
    /// @throws std::out_of_range if chunk_index is greater or equal to the
    ///         value returned by number_of_chunks.
     chunk_size_type  chunk_char_occurrence
      ( chunk_index_type chunk_index
      , char chr
      ) const
      {
        auto & occ_map(text_data.at(chunk_index).char_occ_map);
        return lookup_occurrence(occ_map,chr);
      }

    /// @brief Immutable operation. Returns occurrence of a word in a chunk.
    /// Word defined as contiguous sequence of [a-z][A-Z][0-9] characters.
    /// @param chunk_index  Index of chunk in sequence of chunks
    /// @param word         Word to return occurrence for.
    /// @returns occurrence of word in specfied chunk.
    /// @throws std::out_of_range if chunk_index is greater or equal to the
    ///         value returned by number_of_chunks.
      chunk_size_type  chunk_word_occurrence
      ( chunk_index_type chunk_index
      , std::string const & word
      ) const
      {
        auto & occ_map(text_data.at(chunk_index).word_occ_map);
        return lookup_occurrence(occ_map,tolower(word));
      }

    /// @brief Immutable operation. Returns concatenation of all chunks' text.
    /// @returns Concatenated text of all chunks
      std::string  text() const
      {
        return std::accumulate(text_data.begin(), text_data.end(), chunk_info()
                              , [](chunk_info & acc, chunk_info const & v) 
                                -> chunk_info
                                {
                                  acc.chunk += v.chunk;
                                  return acc;
                                }
                              ).chunk; 
      }

    /// @brief Immutable operation. Returns number of characters in all chunks.
    /// @returns Cumulative number of characters in all chunks
      chunk_size_type  char_count() const
      {
        return std::accumulate(text_data.begin(), text_data.end(), chunk_info()
                              , [](chunk_info & acc, chunk_info const & v) 
                                -> chunk_info
                                {
                                  acc.char_count += v.char_count;
                                  return acc;
                                }
                              ).char_count; 
      }

    /// @brief Immutable operation. Returns number of words in all chunks.
    /// @returns Cumulative number of words in all chunks
      chunk_size_type  word_count() const
      {
        return std::accumulate(text_data.begin(), text_data.end(), chunk_info()
                              , [](chunk_info & acc, chunk_info const & v) 
                                -> chunk_info
                                {
                                  acc.word_count += v.word_count;
                                  return acc;
                                }
                              ).word_count; 
      }

    /// @brief Immutable operation. Returns occurrence of character in all chunks
    /// @param chr          Character to return occurrence for.
    /// @returns Cumulative occurrence of chr in all chunks.
      chunk_size_type  char_occurrence(char chr) const
      {
        return std::accumulate(text_data.begin(), text_data.end(), chunk_info()
                              , [this,chr](chunk_info & acc, chunk_info const & v) 
                                -> chunk_info
                                {
                                  acc.char_count += 
                                          lookup_occurrence(v.char_occ_map,chr);
                                  return acc;
                                }
                              ).char_count; 
      }

    /// @brief Immutable operation. Returns occurrence of a word in all chunks
    /// @param word          Word to return occurrence for.
    /// @returns Cumulative occurrence of word in all chunks.
      chunk_size_type  word_occurrence(std::string const & word) const
      {
        auto lcword(tolower(word));
        return std::accumulate(text_data.begin(), text_data.end(), chunk_info()
                              , [this,lcword](chunk_info & acc, chunk_info const & v) 
                                -> chunk_info
                                {
                                  acc.word_count += 
                                      lookup_occurrence(v.word_occ_map,lcword);
                                  return acc;
                                }
                              ).word_count; 
      }
    };
  } // namespace sies
}} // namespaces dibase::blog
#endif // DIBASE_BLOG_SIES_TEXT_INFO_H

