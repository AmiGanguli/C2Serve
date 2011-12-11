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

#include "C2STestServerRunner.h"
#include "C2STestServerThread.h"

#include "C2SHttpServer.h"

#include <boost/test/unit_test.hpp>

namespace c2s
{
  namespace test
  {

    C2STestServerRunner::C2STestServerRunner( unsigned short iPort )
    {
//          BOOST_MESSAGE( "startup server" );

      c2s::thread::Lock lock( &m_mutex );

      m_serviceSettings.iPort = iPort;

      this->createServerRuntime();
      this->startServerRuntime();

//          BOOST_MESSAGE( "startup complete" );
    }

    C2STestServerRunner::C2STestServerRunner( const std::list<C2SHttpResourcePrototype*> &resources , unsigned short iPort )
    {
      c2s::thread::Lock lock( &m_mutex );

      m_serviceSettings.iPort = iPort;

      this->createServerRuntime();

      std::list<C2SHttpResourcePrototype*>::const_iterator it = resources.begin();
      for ( ; it != resources.end(); ++it )
      {
//            BOOST_MESSAGE( "add resource \"" + ( *it )->getContextRoot() + "\"" );
        m_pHttpServerRuntime->registerResourcePrototype( *it );
      }

      this->startServerRuntime();
//          BOOST_MESSAGE( "startup server" );

//          BOOST_MESSAGE( "startup complete" );
    }

    C2STestServerRunner::~C2STestServerRunner()
    {
      c2s::thread::Lock lock( &m_mutex );

      BOOST_MESSAGE( "shutdown server" );
      m_pHttpServerRuntime->shutdown();

      delete m_pHttpServerRuntime;
      delete m_pServerRunThread;
      delete m_pServerThread;


      BOOST_MESSAGE( "server is down" );
    }

    void C2STestServerRunner::createServerRuntime()
    {

      m_pHttpServerRuntime = new C2SHttpServer( m_serviceSettings );
      m_pServerRunThread = new C2STestServerThread( m_pHttpServerRuntime );
      m_pServerThread = new c2s::thread::Thread<c2s::test::C2STestServerThread>( m_pServerRunThread );

    }

    void C2STestServerRunner::startServerRuntime()
    {
      m_pServerThread->start();
      m_pHttpServerRuntime->waitForStartup();

    }

  }
}
