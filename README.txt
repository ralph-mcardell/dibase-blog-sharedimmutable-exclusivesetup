dibase-blog-sharedimmutable-exclusivesetup
============================================

Code accompanying the "Comments on comments to Herb Sutter's updated GotW #6b 
solution" series of Dibase blog postings.

Shared Immutable, Exclusive Setup (SIES) is the best name I could come
up with at short notice for the usage pattern under discussion.

Known to build with g++ 4.6.3 and run on Linux (Ubuntu 12.04 x86 64 bit).

Use:

    make

to build libraries and applciations.

Use:

    make test

to build unit tests (requires Catch from philsquared - use 

    git submodule update --init

to pull down Catch source).

R. E. McArdell

