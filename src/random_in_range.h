// Project: Shared immutable, exclusive setup blog support code C++ library
/// @file random_in_range.h
/// @brief Simple random unsigned integers within range functor class. 
///
/// Code accompanying the 
/// "Comments on comments to Herb Sutter's updated GotW #6b solution" series of
/// Dibase blog postings.
///
/// @copyright Copyright (c) Dibase Limited 2013
/// @author Ralph E. McArdell

#ifndef DIBASE_BLOG_SIES_RANDOM_IN_RANGE_H
# define DIBASE_BLOG_SIES_RANDOM_IN_RANGE_H
# include <random>
# include <memory>
# include <chrono>

namespace dibase { namespace blog {
  namespace sies // Shared Immutable, Exclusive Setup
  {
  /// @brief Functor producing pseudo random numbers within a range
    class random_in_range
    {
      typedef std::minstd_rand                            prng_type;
      typedef std::uniform_int_distribution<unsigned int> distribution_type;
      distribution_type           distribution;
      std::shared_ptr<prng_type>  prng;

    public:
    /// @brief Construct from range [min,max] values.
    /// Creates a random_in_range object with a new PRNG seeded with the
    /// current time value and a distribution over the range [min,max].
    /// @param min  Minimum random value to produce.
    /// @param max  Maximum random value to produce.
      random_in_range(unsigned int min, unsigned int max)
      : distribution{min, max}
      , prng{std::make_shared<prng_type>
                  ((static_cast<unsigned int>(std::time(nullptr)) )) }
      {}

    /// @brief Construct from existing random_in_range & range [min,max] values.
    /// Creates a random_in_range object that shares a PRNG with an existing
    /// random_in_range object a distribution over the range [min,max].
    /// @param min  Minimum random value to produce.
    /// @param max  Maximum random value to produce.
      random_in_range
      ( random_in_range const & other
      , unsigned int min
      , unsigned int max
      )
      : distribution{min, max}
      , prng{other.prng}
      {}

      random_in_range(random_in_range const &) = delete;
      random_in_range & operator=(random_in_range const &) = delete;
      random_in_range(random_in_range &&) = delete;
      random_in_range & operator=(random_in_range &&) = delete;

    /// @brief Function call: Returns a random number in the object's range.
      unsigned int operator()() { return distribution(*prng); }
    };

  } // namespace sies
}} // namespaces dibase::blog
#endif // DIBASE_BLOG_SIES_RANDOM_IN_RANGE_H

