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

#ifndef THREADTESTRUNNABLE_H_
#define THREADTESTRUNNABLE_H_

#include "Lock.h"

#include <boost/test/unit_test.hpp>

#include <string>

#ifdef WINXX
#include <windows.h>
#endif

#define SLEEP_TIME_MS 30

namespace c2s
{

  namespace test
  {

    namespace thread
    {

      class ThreadTestRunnable
      {
      public:

        ThreadTestRunnable( const std::string &sID , c2s::thread::Mutex *pGlobalMutex )
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
          c2s::thread::Lock lock( &m_mutex );

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
          c2s::thread::Lock lock( &m_mutex );
          return m_iRunCounter;
        }

      private:

        int m_iRunCounter;

        bool m_bRunning;

        c2s::thread::Mutex m_mutex;
        
        c2s::thread::Mutex &m_globalMutex;

        int m_iSleepMS;

        std::string m_sID;

      };

    }

  }

}

#endif /* THREADTESTRUNNABLE_H_ */
