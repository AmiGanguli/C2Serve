/**

 		ThreadTestRunnable.h
		
		Created on: 	02.02.2011
	  Author: 			Kay Wolter
		E-Mail:				wolter.kay@freenet.de

		@brief	Add description here!!!

 */

#ifndef THREADTESTRUNNABLE_H_
#define THREADTESTRUNNABLE_H_

#include "Lock.h"

#include <boost/test/unit_test.hpp>

#include <string>

#ifdef WINXX
#include <windows.h>
#endif

#define SLEEP_TIME_MS 30

namespace g
{

  namespace test
  {

    namespace thread
    {

      class ThreadTestRunnable
      {
      public:

        ThreadTestRunnable( const std::string &sID , g::thread::Mutex *pGlobalMutex )
          : m_iRunCounter( 0 ),
            m_bRunning( false ),
            m_globalMutex( *pGlobalMutex ),
            m_iSleepMS( SLEEP_TIME_MS ),
            m_sID( sID )
        {};

        ~ThreadTestRunnable()
        {
          m_globalMutex.lock();
          BOOST_CHECK( !m_bRunning );
          m_globalMutex.unlock();
        }

        void run()
        {
          g::thread::Lock lock( &m_mutex );

          m_globalMutex.lock();
          BOOST_CHECK( !m_bRunning );
          BOOST_MESSAGE( "Running \"" + m_sID + "\"" );
          m_globalMutex.unlock();

          m_bRunning = true;
#ifdef WINXX
          Sleep( m_iSleepMS );
#else
          usleep( m_iSleepMS * 1000 );
#endif
          m_bRunning = false;

          m_globalMutex.lock();
          BOOST_MESSAGE( "Stopping \"" + m_sID + "\"" );
          m_globalMutex.unlock();

          ++m_iRunCounter;
        }

        int runs()
        {
          g::thread::Lock lock( &m_mutex );
          return m_iRunCounter;
        }

      private:

        int m_iRunCounter;

        bool m_bRunning;

        g::thread::Mutex m_mutex;
        
        g::thread::Mutex &m_globalMutex;

        int m_iSleepMS;

        std::string m_sID;

      };

    }

  }

}

#endif /* THREADTESTRUNNABLE_H_ */
