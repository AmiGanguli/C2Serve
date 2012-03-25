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

#include "C2SRuntime.h"

#include "C2SSettings.h"
#include "C2SException.h"
#include "C2SSocketInfo.h"
#include "C2SStatusSetter.h"
#include "C2SSocketListener.h"
#include "C2SLogInterface.h"
#include "C2SLogSimpleMessageFactory.h"
#include "C2SSocketListenerThread.h"

#include "ThreadQueue.h"
#include "FileUtils.h"

#define TIMEOUT_MS 5000

namespace c2s
{

  C2SRuntime::C2SRuntime( const C2SSettings &settings , C2SDataHandlingInterface *pDataHandling , C2SLogAbstractFactory *pLogFactory )
    : m_pDataHandling( pDataHandling ),
      m_pLogFactory( pLogFactory )
  {
    if ( !m_pLogFactory )
      m_pLogFactory = new C2SLogSimpleMessageFactory();
    m_pLogInstance = m_pLogFactory->createLogInstance();
    this->createSocketListener( settings );
  }

  C2SRuntime::~C2SRuntime()
  {
    delete m_pSocketListener;
    delete m_pLogFactory;
    delete m_pLogInstance;
    delete m_pSocketListenerThread;
  }

  void C2SRuntime::start()
  {
    m_pSocketListener->connect();
    m_pSocketListenerThread->startListener();
  }

  void C2SRuntime::run()
  {
    m_pSocketListener->connect();
    m_pSocketListenerThread->run();
  }

  void C2SRuntime::shutdown()
  {
    m_pSocketListenerThread->stopListener();
  }

  void C2SRuntime::createSocketListener( const C2SSettings &settings )
  {
    C2SSocketListenerSettings socketListenerSettings;
    socketListenerSettings.iPort = settings.iPort;
    socketListenerSettings.iNumThreads = settings.iNumThreads;
    m_pSocketListener = new C2SSocketListener( socketListenerSettings , *m_pDataHandling , *m_pLogInstance );
    m_pSocketListenerThread = new C2SSocketListenerThread( m_pSocketListener );
  }

}
