// Project: Shared immutable, exclusive setup blog support code C++ library
/// @file task.h
/// @brief Simple task type for experiment apps: joinable thread with logger.
///
/// Code accompanying the 
/// "Comments on comments to Herb Sutter's updated GotW #6b solution" series of
/// Dibase blog postings.
///
/// @copyright Copyright (c) Dibase Limited 2013
/// @author Ralph E. McArdell

#ifndef DIBASE_BLOG_SIES_TASK_H
# define DIBASE_BLOG_SIES_TASK_H
# include "logger.h"
# include <thread>

namespace dibase { namespace blog {
  namespace sies // Shared Immutable, Exclusive Setup
  {
  /// @brief Execute task function passed logger member on joinable thread.
  ///
  /// Contains a std::thread object and a logger object.
  /// Only constructable from suitable task function accepting the logger
  /// instance member as its first argument - by non-const reference.
  ///
  /// A task object joins its thread on destruction, after which, on
  /// destruction of its logger, it will write the logger's logged text to
  /// the logger's output stream.
    class task
    {
      logger      log;
      std::thread thread;

    public:
    /// @brief Construct from log output stream and task function.
    ///
    /// Member function template constructor
    ///
    /// Task functions should be of the form:
    ///
    ///  void tf(logger & log, Args...);
    ///
    /// Where Args are zero or more additional argument parameters.
    ///
    /// @param (template) F     Task function type
    /// @param (template) Args  Optional 2nd+ task function argument types
    ///                         parameter pack
    /// @param logstrm          Output stream to log messages to on destruction.
    /// @param f                Task function.
    /// @param args             Task function optional 2nd+ arguments
      template <class F, class ...Args> 
      task(std::ostream & logstrm, F&& f, Args&&... args)
      : log{logstrm}
      , thread{f, std::ref(log), args...}
      {}

      task(task const &) = delete;
      task & operator=(task const &) = delete;
      task(task &&) = delete;
      task & operator=(task &&) = delete;

    /// @brief Join task thread to wait for it to complete
    ///
    /// After task thread join returns logger destructor will cause any logged
    /// text to be output to the default logger stream (std::clog).
      ~task()
      {
        thread.join();
      }
    };
  } // namespace sies
}} // namespaces dibase::blog
#endif // DIBASE_BLOG_SIES_TASK_H

