// Project: Shared immutable, exclusive setup blog support code C++ library
/// @file atomic-policies.h
/// @brief Types implementing atomic policies for call_context_validator
///
/// Code accompanying the 
/// "Comments on comments to Herb Sutter's updated GotW #6b solution" series of
/// Dibase blog postings.
///
/// Defines a class templates that provides various atomic / memory 
/// synchronisation policies for use with the call_context_validator template
/// class.
///
/// @copyright Copyright (c) Dibase Limited 2013
/// @author Ralph E. McArdell

#ifndef DIBASE_BLOG_SIES_ATOMIC_POLICIES_H
# define DIBASE_BLOG_SIES_ATOMIC_POLICIES_H
# include <atomic>

namespace dibase { namespace blog {
  namespace sies // Shared Immutable, Exclusive Setup
  {
  /// @brief Synchonisation policy type that provides no synchronisation
  ///
  /// This policy can be used to see the effects of not having any atomic
  /// or memory sychronisation at all.
  ///
  /// @param T    Type of data supposedly to have synchronisaed access.
  ///             Expected to be a pointer, bool or integer type.
    template <typename T>
    class non_atomic
    {
      T data;
    public:
      explicit non_atomic(T d) : data{d} {}
      non_atomic() = delete;
      non_atomic(non_atomic const &) = delete;
      non_atomic & operator=(non_atomic const &) = delete;
      non_atomic & operator=(non_atomic const &) volatile = delete;
      void store(T d)   {data = d;}
      T    load() const {return data;}
    };

  /// @brief Synchonisation policy wrapping std::atomic behaviours.
  ///
  /// This policy can be used to see the effects of using a std::atomic with
  /// the various memory order sychronisation settings.
  ///
  /// Outer wrapper specifies memory order synchronisation
  /// @param StoreOrder One of the std::meory_order enumeration values
  ///                   relevant to atomic store operations.
  /// @param LoadOrder  One of the std::meory_order enumeration values
  ///                   relevant to atomic load operations.
    template
    < std::memory_order StoreOrder
    , std::memory_order LoadOrder=StoreOrder
    >
    struct memory_order
    {
    /// @brief Synchonisation policy wrapping std::atomic behaviours.
    ///
    /// Inner wrapper specifies type having atomic properties.
    /// This policy can be used to see the effects of using a std::atomic.
    ///
    /// @param T          Type of data supposedly to have synchronisaed access.
    ///                   Expected to be a pointer, bool or integer type.
      template
      < typename T
      >
      class atomic
      {
        std::atomic<T> data;
      public:
        explicit atomic(T d) : data{d} {}
        atomic() = delete;
        atomic(atomic const &) = delete;
        atomic & operator=(atomic const &) = delete;
        atomic & operator=(atomic const &) volatile = delete;
        void store(T d)   {data.store(d,StoreOrder);}
        T    load() const {return data.load(LoadOrder);}
      };
    };
  } // namespace sies
}} // namespaces dibase::blog
#endif // DIBASE_BLOG_SIES_ATOMIC_POLICIES_H

