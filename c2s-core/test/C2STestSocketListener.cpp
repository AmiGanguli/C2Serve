/**

 Copyright (c) 2012, C2Serve (http://www.c2serve.eu)
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

#include "C2STestSocketListener.h"
#include "C2STestSocketListenerDataHandling.h"

#include "C2SLogInterface.h"
#include "C2SSocketListener.h"
#include "C2SLogSimpleMessageFactory.h"

#include "Thread.h"
#include "C2SStatusSetter.h"

namespace c2s
{
  namespace test
  {

    const unsigned int C2STestSocketListener::iPortIntervalStart = 9100;

    const unsigned int C2STestSocketListener::iPortIntervalSize = 30;

    C2STestSocketListener::C2STestSocketListener()
      : m_bIsOnStartup( false ),
        m_bIsRunning( false )
    {
      m_pLogInstance = C2SLogSimpleMessageFactory::createLogInstanceWithDefaultPublication();
      m_pSocketDataHandling = new C2STestSocketListenerDataHandling();
      C2SSocketListenerSettings socketListenerSettins;
      srand( time( NULL ) );
      socketListenerSettins.iPort = iPortIntervalStart + rand() % iPortIntervalSize;
      m_pSocketListener = new C2SSocketListener( socketListenerSettins , *m_pSocketDataHandling , *m_pLogInstance );
      m_pSocketListenerThread = new c2s::thread::Thread<C2STestSocketListener>( this );
    }

    C2STestSocketListener::~C2STestSocketListener()
    {
      delete m_pSocketListener;
      delete m_pSocketDataHandling;
      delete m_pLogInstance;
      delete m_pSocketListenerThread;
    }

    void C2STestSocketListener::runTest()
    {
      C2STestSocketListener testSocketListener;
      testSocketListener.startSocketListener();
      testSocketListener.shutdownSocketListener();
    }

    void C2STestSocketListener::startSocketListener()
    {
      m_bIsOnStartup = true;
      m_pSocketListener->connect();
      C2SStatusSetter running( &m_bIsRunning , true );
      m_pSocketListenerThread->start();
      while( m_bIsOnStartup )
        ;
      usleep( 1000 );
    }

    void C2STestSocketListener::run()
    {
      m_bIsOnStartup = false;
      m_pSocketListener->run();
    }

    void C2STestSocketListener::shutdownSocketListener()
    {
      m_pSocketListener->interrupt();
      while( m_bIsRunning )
        ;
    }

  }
}
