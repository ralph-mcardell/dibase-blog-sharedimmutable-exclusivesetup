// Project: Shared immutable, exclusive setup blog support code C++ library
/// @file logger.h
/// @brief Simple logging class. 
///
/// Code accompanying the 
/// "Comments on comments to Herb Sutter's updated GotW #6b solution" series of
/// Dibase blog postings.
///
/// Very simple logging class that collects logged text in a string and write
/// it to std::log on destruction.
///
/// @copyright Copyright (c) Dibase Limited 2013
/// @author Ralph E. McArdell

#ifndef DIBASE_BLOG_SIES_LOGGER_H
# define DIBASE_BLOG_SIES_LOGGER_H
# include <ostream>
# include <sstream>
# include <iostream>

namespace dibase { namespace blog {
  namespace sies // Shared Immutable, Exclusive Setup
  {
  /// @brief Log-to-string then write-to-output-stream on destruction
  /// Allows per-thread log messages to be collected then written by one single
  /// owning thread when done.
    class logger
    {
      std::ostringstream messages;
      std::ostream &     log_stream;

    public:
      /// @brief Construct from output stream to write logged messages to.
      /// @param logstrm  Output stream to log messages to on destruction.
      ///                 Defaults to std::clog.
      explicit logger(std::ostream & logstrm=std::clog)
      : log_stream(logstrm)
      {}

      logger(logger const &) = delete;
      logger & operator=(logger const &) = delete;
      logger(logger &&) = delete;
      logger & operator=(logger &&) = delete;

    /// @brief Destructor : write out any collected messages.
    ///
    /// Messages will have been written to the log object using the operator<<
    /// and stored as a std::string. If this string is not empty then its
    /// contents will be written to the stream passed by reference in the
    /// constructor, or std::clog by default.
    /// If the string terminates in a newline it is removed; std::endl is
    /// output last if anything is output.
      ~logger()
      {
        auto msg_size(messages.str().size());
        if (msg_size>0)
          {
            if (messages.str()[msg_size-1]=='\n')
              {
                std::string str{messages.str().substr(0,msg_size-1)};
                log_stream << str << std::endl;
              }
            else
              {
                log_stream << messages.str() << std::endl;
              }
          }
      }

    /// @brief Formatted output operator teplate.
    /// Forwards call to internal std::ostringstream used to collect and store
    /// log messages and returns reference to this stream (yes, yes, its a
    /// quick and dirty hack).
    /// @param T  Type of value to format as text and stream to log messages
    ///           string
    /// @param v  Value to format as text and stream to log messages
    ///           string
      template <class T>
      std::ostringstream& operator<<(T&&v)
      {
        messages << v;
        return messages;
      }
    };
  } // namespace sies
}} // namespaces dibase::blog
#endif // DIBASE_BLOG_SIES_LOGGER_H

