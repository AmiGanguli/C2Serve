/**

 		ThreadTest.cpp
		
		Created on: 	30.01.2011
	  Author: 			Kay Wolter
		E-Mail:				wolter.kay@freenet.de

		@brief	Add description here!!!

 */

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "ThreadTestRunnable.h"
#include "ThreadTestTaskQueueRunnable.h"

#include "TaskQueue.h"
#include "ThreadQueue.h"
#include "Thread.h"

using namespace g::test::thread;

BOOST_AUTO_TEST_CASE( gThreadRun )
{
  //mutex to synchronize access to boost unittest functions
  g::thread::Mutex globalMutex;
  
  ThreadTestRunnable p( "RunCheck" , &globalMutex );
  g::thread::Thread<ThreadTestRunnable> t( &p );

  //first run
  t.start();
  while ( p.runs() < 1 );

  //second run
  t.start();
  while ( p.runs() < 2 );
}

BOOST_AUTO_TEST_CASE( gThreadSynchronization )
{
  //mutex to synchronize access to boost unittest functions
  g::thread::Mutex globalMutex;
  
  ThreadTestRunnable p( "SyncCheck" , &globalMutex );
  g::thread::Thread<ThreadTestRunnable> t( &p );

  //first run
  t.start();

  //second run
  t.start();

  //third run
  t.start();

  while ( p.runs() < 3 );
}

BOOST_AUTO_TEST_CASE( gThreadQueue )
{
  //mutex to synchronize access to boost unittest functions
  g::thread::Mutex globalMutex;
  
  g::thread::ThreadQueue<ThreadTestRunnable> tq;

  ThreadTestRunnable r1( "ThreadQueue#1" , &globalMutex );
  tq.queue( &r1 );

  ThreadTestRunnable r2( "ThreadQueue#2" , &globalMutex );
  tq.queue( &r2 );

  tq.start();
  tq.start();
  tq.start();
  tq.start();
  tq.start();
  tq.start();
  tq.start();

  tq.join();
}

BOOST_AUTO_TEST_CASE( gTaskQueue )
{
  //mutex to synchronize access to boost unittest functions
  g::thread::Mutex globalMutex;

  g::thread::TaskQueue tq( NUM_TASK_THREADS );
  unsigned int iNumThreadsRunning = 0;
  unsigned int iNumTasks = 100;

  std::list<ThreadTestTaskQueueRunnable*> runs;

  for ( unsigned int i = 0; i < iNumTasks; ++i )
    runs.push_back( new ThreadTestTaskQueueRunnable ( &iNumThreadsRunning , &globalMutex ) );


  std::list<ThreadTestTaskQueueRunnable*>::iterator it = runs.begin();
  for ( ; it != runs.end(); ++it )
    tq.queue( *it );

  BOOST_MESSAGE( "joining task queue.." );
  tq.join();

  it = runs.begin();
  for ( ; it != runs.end(); ++it )
    delete *it;


  BOOST_MESSAGE( "test done" );
}
