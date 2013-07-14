// Project: Shared immutable, exclusive setup blog support code C++ library
/// @file racecheck.cpp
/// @brief Check for race conditions / sychronisation / memory order problems
///
/// Code accompanying the 
/// "Comments on comments to Herb Sutter's updated GotW #6b solution" series of
/// Dibase blog postings.
///
/// The program repeatedly creates a random number of threads, one of which is
/// randomly choosen to be the creator and the other reader threads. Each thread
/// is passed a set of data - the creator adds it to a text_registry object it
/// creates while the readers wait until query operation access is not an error
/// and check the created object's reported results against those of the
/// exemplar data they were passed.
///
/// @copyright Copyright (c) Dibase Limited 2013
/// @author Ralph E. McArdell

#include "rnd_text_info_maker.h"
#include "random_in_range.h"
#include "task.h"
#include "logger.h"
#include "text_info.h"
#include "text_registry.h"
#include "atomic-policies.h"
#include <vector>
#include <string>
#include <thread>
#include <memory>
#include <chrono>

using namespace dibase::blog::sies;

struct clog_task : task
{
  template <class F, class ...Args> 
  explicit clog_task(F&& f, Args&&... args)
  : task{std::clog, f, args...}
  {}
};

typedef std::unique_ptr<clog_task>  task_ptr;

// Unsynchronised access type aliases - should produce some race condition
// problems but seem not to on x86-64 VMWare 9 VM having 2 processors with 4
// cores each running x86-64 Linux, g++ 4.6.3 on a 2*Opteron 4334 Windows 8 host
typedef text_registry<non_atomic>       text_registry_type;
typedef non_atomic<text_registry_type*> text_registry_ptr;

// Synchronised access type aliases. In theory should create no race conditions.
// Creator thread atomically writing to text_registry_ptr should be sufficient
// to allow the store to be observed eventually by reader threads, and all
// updates are to memory associated with the one object and its pointer, so
// a store-release followed by a consume-load should allow readers to see
// all writes to the object by the creator thread _after_ the store-release.
/*typedef text_registry
          < atomic
          , std::memory_order_release
          , std::memory_order_consume >                       text_registry_type;
typedef atomic<text_registry_type*,std::memory_order_relaxed> text_registry_ptr;
*/
void reader(logger & log, text_info const * p_reference, text_registry_ptr& p_data);

void creator(logger & log, text_info const * p_reference, text_registry_ptr& p_data)
{
  auto i=0U;
  try
    {
//      log << "Creator:\n";
      p_data.store(new text_registry_type);
      std::this_thread::sleep_for(std::chrono::milliseconds(1U));
      for (; i!=p_reference->number_of_chunks(); ++i)
        {
          p_data.load()->add_text_chunk(p_reference->chunk_text(i));
        }
      p_data.load()->setup_complete();
      reader(log, p_reference, p_data); // check creator can read what it wrote!
      std::this_thread::sleep_for(std::chrono::milliseconds(1U));
    }
  catch (std::exception & e)
    {
      log << "   After " << i << " text chunks added " 
          << typeid(e).name() << " exception caught.\n   Reason: "
          << e.what() << '\n';
    }
  catch (...)
    {
      log << "   Exception caught of unknown type.\n";
    }
}

void reader(logger & log, text_info const * p_reference, text_registry_ptr& p_data)
{
//  log << "Reader:\n";
  auto start_time(std::chrono::system_clock::now());
  text_registry_type * p_data_local{nullptr};
  while ((p_data_local=p_data.load())==nullptr)
    {
      if (std::chrono::system_clock::now()-start_time>std::chrono::seconds(3))
        {
          log << "   Pointer to data was not set within timeout\n";
          return;
        }
    }

  bool doing{true};
  do
    {
      try
        {
          auto txt(p_data_local->text());
          auto wc(p_data_local->word_count());
          auto cc(p_data_local->char_count());
          auto co(p_data_local->char_occurrence('z'));
          auto wo(p_data_local->word_occurrence("ee"));
          auto nc(p_data_local->number_of_chunks());
          auto ecc(p_reference->char_count());
          auto ewc(p_reference->word_count());
          auto etxt(p_reference->text());
          auto eco(p_reference->char_occurrence('z'));
          auto ewo(p_reference->word_occurrence("ee"));
          auto enc(p_reference->number_of_chunks());
          if (cc!=ecc)
            {
              log << "Read " << cc << " num chars, expected " << ecc << " num chars\n";
            }
          if (wc!=ewc)
            {
              log << "Read " << wc << " num words, expected " << ewc << " num words\n";
            }
          if (co!=eco)
            {
              log << "Read " << co << " of occurrence 'z', expected " << eco << "\n";
            }
          if (wo!=ewo)
            {
              log << "Read " << wo << " of occurrence 'ee', expected " << ewo << "\n";
            }
          if (nc!=enc)
            {
              log << "Read " << nc << " chunks, expected " << enc << " chunks\n";
            }
          if (txt!=etxt)
            {
              log << "Read text:\n" << txt << "\nExpected text:\n" << etxt << "\n";
            }
          doing = false;
        }
      catch (call_context_violation &)
        {
        }
      catch (std::exception & e)
        {
          log << "   " << typeid(e).name() << " exception caught. Reason: "
              << e.what() << '\n';
          doing = false;
        }
      catch (...)
        {
          log << "   Exception caught of unknown type.\n";
          doing = false;
        }
    } 
  while (doing);
}


auto const NumberOfRepeats(300U);
auto const MinTextChunks(10U);
auto const MaxTextChunks(50U);
auto const MinWordsPerChunk(80U);
auto const MaxWordsPerChunk(1500U);
auto const MinThreads(8U);
auto const MaxThreads(30U);
auto const MinWordSize(2U);
auto const MaxWordSize(7U);

int main()
{
  random_in_range     rnd_num_threads{MinThreads, MaxThreads};
  rnd_text_info_maker make_rnd_text_info( MinTextChunks,MaxTextChunks
                                        , MinWordsPerChunk,MaxWordsPerChunk
                                        , MinWordSize,MaxWordSize
                                        );
  for (auto i=NumberOfRepeats; i!=0; --i)
    {
      std::clog << "#### Run #"<<NumberOfRepeats-i+1<<'\r';
      auto pti(make_rnd_text_info());
      auto n_threads(rnd_num_threads());
      auto creator_thread_idx(random_in_range(rnd_num_threads,0U,n_threads-1)());
      text_registry_ptr data{nullptr};
      {
        std::vector<task_ptr> tasks;
        for (auto i=0U; i!=n_threads; ++i)
          {
            if (i==creator_thread_idx)
              {
                tasks.push_back(task_ptr{new clog_task{creator, pti.get(), std::ref(data)}});
              }
            else
              {
                tasks.push_back(task_ptr{new clog_task{reader, pti.get(), std::ref(data)}});
              }
          }
      }
      delete data.load();
    }
  std::clog << std::endl;
}

