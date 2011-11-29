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

#include "C2STestRestRequest.h"
#include "C2STestRestFixture.h"

#include "C2SHttpClient.h"

namespace c2s
{
  namespace test
  {

    C2STestRestRequest::C2STestRestRequest( C2SHttpMethod method , const std::string &sURI )
      : m_request( C2SHttpRequestHeader( method , sURI ) )
    {
    }

    C2STestRestRequest::~C2STestRestRequest()
    {
    }

    C2STestRestRequest C2STestRestRequest::build( C2SHttpMethod method , const std::string &sURI )
    {
      return C2STestRestRequest( method , sURI );
    }

    C2STestRestRequest &C2STestRestRequest::query_field( const std::string &name , const std::string &value )
    {
      m_request.header().QueryFields.addField( name , value );
      return *this;
    }

    C2STestRestRequest &C2STestRestRequest::accept( const C2SHttpMediaType &mediatype )
    {
      m_request.header().Fields.addAccept( mediatype );
      return *this;
    }

    C2STestRestRequest &C2STestRestRequest::entity( const C2SHttpMediaType &mediaType , const std::string &sEntityData )
    {
      m_sRequestEntityData = sEntityData;
      m_request.setEntity( new C2SHttpEntity( m_sRequestEntityData.c_str() , m_sRequestEntityData.size() ) );
      m_request.header().Fields.setContentType( mediaType );
      m_request.header().Fields.setContentLength( m_sRequestEntityData.size() );
      return *this;
    }

    C2SHttpResponse C2STestRestRequest::process() const
    {
      C2SHttpClient client( "localhost" , C2STestRestFixture::iPortOfTestServer );
      return client.send( m_request );
    }

    const std::string &C2STestRestRequest::getURI() const
    {
      return m_request.header().URI;
    }

  }
}

