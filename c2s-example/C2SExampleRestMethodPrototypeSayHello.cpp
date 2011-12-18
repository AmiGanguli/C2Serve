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

#include "C2SExampleRestMethodPrototypeSayHello.h"

#include "C2SHttpEntityStreamerPlainText.h"
#include "DateTimeLogger.h"

namespace c2s
{
  namespace example
  {

    const std::string C2SExampleRestMethodPrototypeSayHello::sPath = "say-hello/to";

    const std::string C2SExampleRestMethodPrototypeSayHello::sInvalidName = "c2s-example-invalid-name";

    C2SExampleRestMethodPrototypeSayHello::C2SExampleRestMethodPrototypeSayHello()
      : C2SRestMethodPrototypeGET<std::string>( sPath ),
        m_pLogger( new io::DateTimeLogger( "c2s-example" ) ),
        m_sNameToGreet( sInvalidName ),
        m_sNameToGreetFrom( sInvalidName )
    {
      C2SRestMethodPrototypeGET<std::string>::installEntityStreamer( new C2SHttpEntityStreamerPlainText() );
      C2SRestMethodPrototypeGET<std::string>::addPathParameter( "name" , &m_sNameToGreet );
      C2SRestMethodPrototypeGET<std::string>::addQueryParameter( "from" , &m_sNameToGreetFrom , m_sNameToGreetFrom );
    }

    C2SExampleRestMethodPrototypeSayHello::~C2SExampleRestMethodPrototypeSayHello()
    {
      delete m_pLogger;
    }

    C2SHttpResponse *C2SExampleRestMethodPrototypeSayHello::process()
    {
      std::string sGreetToName = std::string( " to " ) + m_sNameToGreet;
      if ( m_sNameToGreetFrom != sInvalidName )
        sGreetToName = sGreetToName + " from " + m_sNameToGreetFrom;
      m_pLogger->info( "Say hello" + sGreetToName );
      return C2SRestMethodPrototypeGET<std::string>::buildResponse( OK , std::string( "=== c2s-example says hello" ) + sGreetToName + " ===" );
    }

    C2SExampleRestMethodPrototypeSayHello *C2SExampleRestMethodPrototypeSayHello::clone() const
    {
      return new C2SExampleRestMethodPrototypeSayHello();
    }

  }
}
