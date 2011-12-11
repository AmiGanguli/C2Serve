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

#include "C2SException.h"
#include "C2SSocketInfo.h"
#include "C2SStatusSetter.h"
#include "C2SSocketListener.h"

#include "ThreadQueue.h"
#include "FileUtils.h"

#define TIMEOUT_MS 5000

namespace c2s
{

  C2SRuntime::C2SRuntime()
    : m_bIsRunning( false ),
      m_bIsOnStartup( false ),
      m_bIsOnShutdown( false )
  {
    //TODO: set listeners from outside (allow multiple listeners)
    const C2SGlobalSettings &gs = C2SGlobalSettings::Settings();
    C2SSocketListenerSettings ls;
    ls.iPort = gs.iPort;
    ls.iNumThreads = gs.iNumThreads;
    m_pSocketListener = new C2SSocketListener( ls , *this );
  }

  C2SRuntime::~C2SRuntime()
  {
    delete m_pSocketListener;
  }

  void C2SRuntime::run()
  {
    if ( m_bIsRunning || m_bIsOnStartup || m_bIsOnShutdown )
      throw C2SException( "C2SRuntime::run: Server is already running!" );

    m_bIsOnStartup = true;

    C2SStatusSetter running( &m_bIsRunning , true );
    this->runInternal();
  }

  void C2SRuntime::shutdown()
  {
    if ( !m_bIsRunning )
      throw C2SException( "C2SRuntime::shutdown: Server is currently not running!" );

    C2SStatusSetter running( &m_bIsOnStartup , true );
    this->shutdownInternal();
  }

  void C2SRuntime::waitForStartup()
  {
    while( m_bIsOnStartup || !m_bIsRunning )
      ;
  }

  void C2SRuntime::runInternal()
  {
    m_bIsOnStartup = false;
    m_pSocketListener->run();
  }

  void C2SRuntime::shutdownInternal()
  {
    m_pSocketListener->interrupt();

    //wait for run shutdown
    while( m_bIsRunning )
      ;
  }
}
