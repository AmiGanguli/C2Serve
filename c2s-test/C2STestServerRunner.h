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

#ifndef C2STESTSERVERRUNNER_H_
#define C2STESTSERVERRUNNER_H_

#include "C2STestServerThread.h"
#include "C2SHttpResourceManager.h"
#include "C2SGlobalSettings.h"

#include "Thread.h"

namespace c2s
{
  namespace test
  {

    class C2STestServerRunner
    {
    public:

      C2STestServerRunner( unsigned short iPort = C2SGlobalSettings::Settings().iPort )
        : m_serverThread( &m_serverRunTime )
      {
//          BOOST_MESSAGE( "startup server" );

        c2s::thread::Lock lock( &m_mutex );

        C2SGlobalSettings::Settings().iPort = iPort;

        m_serverThread.start();

        C2SHttpServer::waitForStartup();

//          BOOST_MESSAGE( "startup complete" );
      }

      C2STestServerRunner( const std::list<C2SHttpResource*> &resources , unsigned short iPort = C2SGlobalSettings::Settings().iPort )
        : m_serverThread( &m_serverRunTime )
      {
        c2s::thread::Lock lock( &m_mutex );

        std::list<C2SHttpResource*>::const_iterator it = resources.begin();
        for ( ; it != resources.end(); ++it )
        {
//            BOOST_MESSAGE( "add resource \"" + ( *it )->getContextRoot() + "\"" );
          C2SHttpResourceManager::registerResource( *it );
        }

//          BOOST_MESSAGE( "startup server" );

        C2SGlobalSettings::Settings().iPort = iPort;

        m_serverThread.start();

        C2SHttpServer::waitForStartup();

//          BOOST_MESSAGE( "startup complete" );
      }

      virtual ~C2STestServerRunner()
      {
        c2s::thread::Lock lock( &m_mutex );

        BOOST_MESSAGE( "shutdown server" );
        C2SHttpServer::shutdown();

        BOOST_MESSAGE( "server is down" );
      }

    private:

      C2STestServerRunner( const C2STestServerRunner & );

      C2STestServerRunner &operator=( const C2STestServerRunner & );

      C2STestServerThread m_serverRunTime;

      c2s::thread::Thread<c2s::test::C2STestServerThread> m_serverThread;

      c2s::thread::Mutex m_mutex;

    };

  }
}

#endif /* C2SSERVERRUNNER_H_ */
