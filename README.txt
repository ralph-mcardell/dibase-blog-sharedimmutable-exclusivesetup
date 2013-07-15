dibase-blog-sharedimmutable-exclusivesetup
============================================

Code accompanying the "Comments on comments to Herb Sutter's updated GotW #6b 
solution" series of Dibase blog postings.

Shared Immutable, Exclusive Setup (SIES) is the best name I could come
up with at short notice for the usage pattern under discussion.

Known to build with g++ 4.6 and 4.7 and to run on GNU/Linux 
(Ubuntu 12.04 x86 64 bit, Raspbian 3.6.11+, armv6l).

G++ 4.7 or later is recommended as it has much better support for the C++11 memory
model and atomics, as detailed at:

    http://gcc.gnu.org/wiki/Atomic/GCCMM

and in fact allows unit tests around atomic and memory order load/store operations
to pass on the Raspberry Pi which the g++ 4.6.3 default compiler and C++ standard
library did not.

Use:

    make

to build libraries and applciations.

Use:

    make test

to build unit tests (requires Catch from philsquared - use 

    git submodule update --init

to pull down Catch source).

R. E. McArdell

