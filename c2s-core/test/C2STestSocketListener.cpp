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
#include "C2SSocketClient.h"
#include "C2SSocketListener.h"
#include "C2SSocketListenerThread.h"
#include "C2SLogSimpleMessageFactory.h"

namespace c2s
{
  namespace test
  {

    const unsigned int C2STestSocketListener::iPortIntervalStart = 9100;

    const unsigned int C2STestSocketListener::iPortIntervalSize = 30;

    C2STestSocketListener::C2STestSocketListener()
    {
      m_pLogInstance = C2SLogSimpleMessageFactory::createLogInstanceWithDefaultPublication();
      m_pSocketDataHandling = new C2STestSocketListenerDataHandling();
      C2SSocketListenerSettings socketListenerSettings;
      srand( time( NULL ) );
      socketListenerSettings.iPort = iPortIntervalStart + rand() % iPortIntervalSize;
      m_pSocketListener = new C2SSocketListener( socketListenerSettings , *m_pSocketDataHandling , *m_pLogInstance );
      m_pSocketListenerThread = new C2SSocketListenerThread( m_pSocketListener );
      m_pSocketClient = new C2SSocketClient( "localhost" , socketListenerSettings.iPort );
    }

    C2STestSocketListener::~C2STestSocketListener()
    {
      delete m_pSocketListener;
      delete m_pSocketDataHandling;
      delete m_pLogInstance;
      delete m_pSocketListenerThread;
      delete m_pSocketClient;
    }

    void C2STestSocketListener::runTest()
    {
      C2STestSocketListener testSocketListener;
      testSocketListener.startSocketListener();
      testSocketListener.sendTestMessageThroughSocket();
      testSocketListener.shutdownSocketListener();
    }

    void C2STestSocketListener::createTestMessageToSendThroughSocket()
    {
      m_sTestMessageToSendThroughSocket = "0123456789\nabcdefghijklmnopqrstuvwxyz\nABCDEFGHIJKLMOPQRSTUVWXYZ";
    }

    void C2STestSocketListener::startSocketListener()
    {
      m_pSocketListener->connect();
      m_pSocketListenerThread->startListener();
    }

    void C2STestSocketListener::sendTestMessageThroughSocket()
    {
      m_pSocketClient->sendDataThroughSocket( m_sTestMessageToSendThroughSocket.c_str() , m_sTestMessageToSendThroughSocket.size() );
    }

    void C2STestSocketListener::shutdownSocketListener()
    {
      m_pSocketListenerThread->stopListener();
    }

  }
}
