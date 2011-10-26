/**

 		ThreadTestTaskQueueRunnable.h
		
		Created on: 	14.03.2011
	  Author: 			Kay Wolter
		E-Mail:				wolter.kay@freenet.de

		@brief	Add description here!!!

 */

#ifndef THREADTESTTASKQUEUERUNNABLE_H_
#define THREADTESTTASKQUEUERUNNABLE_H_

#include "TaskQueue.h"

#define NUM_TASK_THREADS 30
#define SLEEP_TIME_MS_TASK 10

namespace g
{
  namespace test
  {
    namespace thread
    {

      class ThreadTestTaskQueueRunnable : public g::thread::TaskBase
      {
      public:

        ThreadTestTaskQueueRunnable( unsigned int *pRunningThreads , g::thread::Mutex *pGlobalMutex )
          : m_pRunningThreads( pRunningThreads ),
            m_globalMutex( *pGlobalMutex )
        {};

        void run()
        {
          ++( *m_pRunningThreads );

          m_globalMutex.lock();
          BOOST_CHECK( ( *m_pRunningThreads ) <= NUM_TASK_THREADS );
          BOOST_MESSAGE( "#tasks running: " << *m_pRunningThreads );
          m_globalMutex.unlock();

#ifdef WINXX
          Sleep( SLEEP_TIME_MS_TASK );
#else	  
          usleep( SLEEP_TIME_MS_TASK * 1000 );
#endif

          --( *m_pRunningThreads );
        }

      private:

        unsigned int *m_pRunningThreads;

        g::thread::Mutex &m_globalMutex;

      };

    }
  }
}

#endif /* THREADTESTTASKQUEUERUNNABLE_H_ */
