/**

  Copyright (c) 2011, C2Serve (http://www.c2serve.eu)
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
  1. Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
  3. All advertising materials mentioning features or use of this software
     must display the following acknowledgement: "This product includes software of the C2Serve project".
  4. Neither the name of the C2Serve project nor the
     names of its contributors may be used to endorse or promote products
     derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY C2SERVE ''AS IS'' AND ANY
  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL C2SERVE BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "C2SThreadTestRunnable.h"
#include "C2SThreadTestTaskQueueRunnable.h"

#include "C2STaskQueue.h"
#include "C2SThreadQueue.h"

using namespace c2s::thread;
using namespace c2s::thread::test;

BOOST_AUTO_TEST_CASE( run )
{
  //mutex to synchronize access to boost unittest functions
  C2SMutex globalMutex;
  
  C2SThreadTestRunnable p( "RunCheck" , &globalMutex );

  //first run
  p.start();
  while ( p.runs() < 1 );

  //second run
  p.start();
  while ( p.runs() < 2 );
}

BOOST_AUTO_TEST_CASE( synchronization )
{
  //mutex to synchronize access to boost unittest functions
  C2SMutex globalMutex;
  
  C2SThreadTestRunnable p( "SyncCheck" , &globalMutex );

  //first run
  p.startThread();
  p.blockUntilThreadIsStarted();

  //second run
  p.startThread();
  p.blockUntilThreadIsStarted();

  //third run
  p.startThread();
  p.blockUntilThreadIsStarted();

  while ( p.runs() < 3 );
}

BOOST_AUTO_TEST_CASE( queue )
{
  //mutex to synchronize access to boost unittest functions
  C2SMutex globalMutex;
  
  C2SThreadQueue<C2SThreadTestRunnable> tq;

  C2SThreadTestRunnable r1( "ThreadQueueRunner#1" , &globalMutex );
  tq.queue( &r1 );

  C2SThreadTestRunnable r2( "ThreadQueueRunner#2" , &globalMutex );
  tq.queue( &r2 );

  unsigned int iNumberOfThreadsToStart = 100;
  for ( unsigned int iNumberOfThreadsStarted = 0; iNumberOfThreadsStarted < iNumberOfThreadsToStart; ++iNumberOfThreadsStarted )
    tq.start();

  tq.join();
}

BOOST_AUTO_TEST_CASE( task_queue )
{
  //mutex to synchronize access to boost unittest functions
  C2SMutex globalMutex;

  C2STaskQueue tq( NUM_TASK_THREADS );
  unsigned int iNumThreadsRunning = 0;
  unsigned int iNumTasks = 100;

  std::list<C2SThreadTestTaskQueueRunnable*> runs;

  for ( unsigned int i = 0; i < iNumTasks; ++i )
    runs.push_back( new C2SThreadTestTaskQueueRunnable ( &iNumThreadsRunning , &globalMutex ) );


  std::list<C2SThreadTestTaskQueueRunnable*>::iterator it = runs.begin();
  for ( ; it != runs.end(); ++it )
    tq.queue( *it );

  BOOST_MESSAGE( "joining task queue.." );
  tq.join();

  it = runs.begin();
  for ( ; it != runs.end(); ++it )
    delete *it;


  BOOST_MESSAGE( "test done" );
}
