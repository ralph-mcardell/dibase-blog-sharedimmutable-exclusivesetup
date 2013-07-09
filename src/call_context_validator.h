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
  /// @param T            The type to be validated, intended to be the
  ///                     containing type.
  /// @param AtomicPolicy The atomic synchronisation policy used for the
  ///                     published 'flag'. Needs to accept T* as its U
  ///                     template parameter.
    template <class T, template <class U> class AtomicPolicy>
    class call_context_validator
    {
      AtomicPolicy<T*>  published_;
      std::thread::id   update_id;

    public:
      call_context_validator()
      : published_{nullptr}
      , update_id{std::this_thread::get_id()}
      {}

      call_context_validator(call_context_validator const &) = delete;
      call_context_validator& operator=(call_context_validator const &) = delete;
      call_context_validator(call_context_validator &&) = delete;
      call_context_validator& operator=(call_context_validator &&) = delete;

      void publish(T* that)
      {
        (*this)(that);
        published_.store(that);
      }

      bool published() const
      {
        return published_.load();
      }

      bool unpublished() const
      {
        return !published_.load();
      }

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

