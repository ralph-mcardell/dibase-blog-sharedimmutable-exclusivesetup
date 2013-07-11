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
  /// @param T    Type of data supposedly to have synchronised access.
  ///             Expected to be a pointer, bool or integer type.
    template <typename T>
    class non_atomic
    {
      T data;
    public:
    /// @brief construct from initial value by value
    /// @param d  Initial value of contained data object
      explicit non_atomic(T d) : data{d} {}
      non_atomic() = delete;
      non_atomic(non_atomic const &) = delete;
      non_atomic & operator=(non_atomic const &) = delete;
      non_atomic & operator=(non_atomic const &) volatile = delete;

    /// @brief Pseudo atomic store operation
    /// @param  d New value for contained object.
      void store(T d)   {data = d;}

    /// @brief Pseudo atomic load operation
    /// @returns  Current value of contained object
      T    load() const {return data;}
    };

  /// @brief Synchonisation policy wrapping std::atomic behaviours.
  ///
  /// This policy can be used to see the effects of using a std::atomic with
  /// the various memory order sychronisation values.
  ///
  /// @param T          Type of data supposedly to have synchronisaed access.
  ///                   Expected to be a pointer, bool or integer type.
  /// @param StoreOrder One of the std::memory_order enumeration values
  ///                   relevant to atomic store operations.
  /// @param LoadOrder  One of the std::memory_order enumeration values
  ///                   relevant to atomic load operations.
    template
    < typename T
    , std::memory_order StoreOrder
    , std::memory_order LoadOrder=StoreOrder
    >
    class atomic
    {
      std::atomic<T> data;
    public:
    /// @brief construct from initial value by value
    /// @param d  Initial value of atomic data object
      explicit atomic(T d) : data{d} {}

      atomic() = delete;
      atomic(atomic const &) = delete;
      atomic & operator=(atomic const &) = delete;
      atomic & operator=(atomic const &) volatile = delete;

    /// @brief Atomic store operation using StoreOrder memory order.
    /// @param  d New value to atomically object.
      void store(T d)   {data.store(d,StoreOrder);}

    /// @brief Atomic load operation using LoadOrder memory order.
    /// @returns  Atomically loaded value
      T    load() const {return data.load(LoadOrder);}
    };
  } // namespace sies
}} // namespaces dibase::blog
#endif // DIBASE_BLOG_SIES_ATOMIC_POLICIES_H

