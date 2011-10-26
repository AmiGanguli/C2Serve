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

#ifndef C2SRUNTIME_H_
#define C2SRUNTIME_H_

#include "C2SGlobalSettings.h"

#include <list>

namespace g
{

  namespace c2s
  {

    class C2SSocketListener;
    class C2SServerTypeInterface;

    /**
     *
     * @brief   Startup and shutdown your C2S service.
     *
     */
    class C2SRuntime
    {
    public:

      /**
       *
       * @brief   Startup C2S service.
       *
       *          Create socket listener, bind socket and start listener event loop.
       *
       * @param   type    Service type to use.
       *                  The service type specifies how data received from the socket has to be interpreted.
       *                  @see C2SHttpServerType
       *
       * @throws  C2SException
       *
       */
      static void run( const C2SServerTypeInterface &type );

      /**
       *
       * @brief   Interrupt listener and shutdown C2S service.
       *
       *          This method blocks until shutdown is complete.
       *
       */
      static void shutdown();

      /**
       *
       * @brief   Blocks until C2S service startup is complete.
       *
       *          Use that function if you need to wait for startup (very useful for unit testing).
       *
       */
      static void waitForStartup();

    private:

      C2SRuntime( const C2SServerTypeInterface &type );

      virtual ~C2SRuntime();

      C2SRuntime( const C2SRuntime & );

      C2SRuntime &operator=( const C2SRuntime & );

      void runInternal();

      void shutdownInternal();

      C2SSocketListener *m_pSocketListener;

      static C2SRuntime *pInstance;

      static volatile bool bIsRunning;

      static volatile bool bIsOnStartup;

      static volatile bool bIsOnShutdown;

    };

  }

}

#endif /* C2SRUNTIME_H_ */
