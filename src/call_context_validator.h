// Project: Shared immutable, exclusive setup blog support code C++ library
/// @file call_context_validator.h
/// @brief CLass template implementing call context validation with publish
///
/// Code accompanying the 
/// "Comments on comments to Herb Sutter's updated GotW #6b solution" series of
/// Dibase blog postings.
///
/// Defines a class template that provides the call context validation and
/// publish services as discussed in parts 1 & 2 of the blog posts.
///
/// @copyright Copyright (c) Dibase Limited 2013
/// @author Ralph E. McArdell

#ifndef DIBASE_BLOG_SIES_CALL_CONTEXT_VALIDATOR_H
# define DIBASE_BLOG_SIES_CALL_CONTEXT_VALIDATOR_H
# include <stdexcept>
# include <string>
# include <thread>
# include <atomic>
# include <type_traits>

namespace dibase { namespace blog {
  namespace sies // Shared Immutable, Exclusive Setup
  {
  /// @brief Specific exception type for call context violations
    class call_context_violation : public std::runtime_error
    {
    public:
    /// @brief Construct from std::string message
      explicit call_context_violation(std::string const & what_arg)
      : std::runtime_error(what_arg)
      {}

    /// @brief Construct from C-string message
      explicit call_context_violation(char const * what_arg)
      : std::runtime_error(what_arg)
      {}
    };

  /// @brief Class template implementing the call context validation method
  ///
  /// Implements the call context validation idea as dicussed in parts 1 & 2 of
  /// the "Comments on comments to Herb Sutter's updated GotW #6b solution"
  /// series of Dibase blog postings.
  ///
  /// The published atomic 'flag' is specified by a template template parameter
  /// to allow different policies to be tried. The required type _only_ has
  /// to implement parameterless store and load member functions and an explict
  /// single argument constructor that can be used to set the initial value
  /// (that is, a simplified subset of the std::atomic<> interface).
  ///
  /// This particular implementation is designed to be used as a class data
  /// member - that is the validation and publishing services are provided by
  /// composition rather than by inheritance.
  ///
  /// @param AtomicPolicy The atomic synchronisation policy used for the
  ///                     published 'flag'. Needs to accept T* as its first,
  ///                     type, template parameter and optionally one or
  ///                     more (typically 2) std::memory_order values.
  /// @param T            The type to be validated, intended to be the
  ///                     containing type, pointer tothis type used with
  ///                     AtomicPolicy.
  /// @param M            Zero or more memory order parameters.
  ///                     Typical number of values are: 
  ///                     0 : for non atomic, synchronising, pseudo policies.
  ///                     1 : Load and store have same memory order values
  ///                     2 : Store and load have different memory order values.
    template 
    < template <class, std::memory_order...> class AtomicPolicy
    , class T
    , std::memory_order... M
    >
    class call_context_validator
    {
      AtomicPolicy<T*, M...>  published_;
      std::thread::id         update_id;

    public:
    /// @brief Construct. 
    /// Initialise atomic published pointer 'flag' to null pointer and update
    /// thread id to the constructor calling thread's id.
      call_context_validator()
      : published_{nullptr}
      , update_id{std::this_thread::get_id()}
      {}

      call_context_validator(call_context_validator const &) = delete;
      call_context_validator& operator=(call_context_validator const &) = delete;
      call_context_validator(call_context_validator &&) = delete;
      call_context_validator& operator=(call_context_validator &&) = delete;

    /// @brief Publish object making it immutable with shared thread access.
    /// @param that   this pointer of containing validated object .
    /// @throws dibase::blog::sies::call_context_violation if called by
    ///         thread other than the creator thread or if the object
    ///         has already been published.
      void publish(T* that)
      {
        (*this)(that);
        published_.store(that);
      }

    /// @bried Unvalidated query to see if object in published state.
    /// @returns true if object has been published, false otherwise
      bool published() const
      {
        return published_.load();
      }

    /// @bried Unvalidated query to see if object is not in published state.
    /// @returns true if object has NOT been published, false otherwise
      bool unpublished() const
      {
        return !published_.load();
      }

    /// @brief Perform validation for mutating operations of validated object.
    /// @param  this pointer of containing validated object (not used).
    /// @throws dibase::blog::sies::call_context_violation if called by
    ///         thread other than the creator thread or if the object
    ///         has been published.
      void operator()(T*) const
      {
        if ( published()
          || std::this_thread::get_id()!=update_id
           )
          {
            throw call_context_violation
                  { "Illegal usage : Concurrent access or attempt at "
                    "mutating operation on a published immutable "
                    "object."
                  };
          }
      }

    /// @brief Perform validation for non-mutating operations of validated object.
    /// @param  this pointer of containing validated object (not used).
    /// @throws dibase::blog::sies::call_context_violation if called by
    ///         thread other than the creator thread unless the object
    ///         has been published.
      void operator()(T const *) const
      {
        if (unpublished()
          && std::this_thread::get_id()!=update_id
           )
          {
            throw call_context_violation
                  { "Illegal usage : Concurrent access to "
                    "an unpublished object."
                  };
          }
      }
    };

  } // namespace sies
}} // namespaces dibase::blog
#endif // DIBASE_BLOG_SIES_CALL_CONTEXT_VALIDATOR_H

