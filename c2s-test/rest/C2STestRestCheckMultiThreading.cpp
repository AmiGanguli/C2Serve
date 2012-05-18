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

#include "C2STestRestCheckMultiThreading.h"
#include "C2STestRestMethodThreading.h"
#include "C2STestRestFixture.h"

#include "C2STaskQueue.h"
#include "StringUtils.h"

namespace c2s
{

  namespace test
  {

    C2STestRestCheckMultiThreading::C2STestRestCheckMultiThreading( const c2s::test::C2STestRestRequest &request , const c2s::test::C2STestRestResponse &response_check )
      : m_request( request ),
        response_check( response_check )
    {}

    void C2STestRestCheckMultiThreading::runTest()
    {
#ifdef WINXX
      //TODO
      unsigned int iNumRequests = 20;
#else
      unsigned int iNumRequests = 1000;
#endif
      unsigned int iNumThreads = 5;
      unsigned int iSleepMS = 5;

      c2s::thread::C2STaskQueue tq( iNumThreads );
      std::list<C2STestRestCheckMultiThreading*> checks;

      for ( unsigned int i = 0; i < iNumRequests; ++i )
        checks.push_back( C2STestRestCheckMultiThreading::create( iSleepMS ) );

      std::list<C2STestRestCheckMultiThreading*>::iterator it = checks.begin();
      for ( ; it != checks.end(); ++it )
        tq.queue( *it );

      tq.join();

      it = checks.begin();
      for ( ; it != checks.end(); ++it )
        delete *it;
    }

    void C2STestRestCheckMultiThreading::run()
    {
      c2s::C2SHttpResponse *pServerResponse = m_request.process();
      c2s::thread::C2SLock<c2s::thread::C2SMutex> lock( c2s::test::C2STestRestFixture::pGlobalMutex );
      response_check.check( *pServerResponse );
      delete pServerResponse;
    }

    C2STestRestCheckMultiThreading *C2STestRestCheckMultiThreading::create( unsigned int iSleepMS )
    {
      return new C2STestRestCheckMultiThreading(

          c2s::test::C2STestRestRequest::
          build( c2s::C2S_GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodThreading::sPath + "/" + c2s::util::toString( iSleepMS ) )
          ,
          c2s::test::C2STestRestResponse::
          build( c2s::OK )

      );
    }

  }

}
