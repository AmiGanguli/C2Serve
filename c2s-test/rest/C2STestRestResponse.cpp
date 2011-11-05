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

#include "C2STestRestResponse.h"
#include "C2STestRestFixture.h"

#include "C2SHttpClient.h"

#include <boost/test/unit_test.hpp>

namespace c2s
{

  namespace test
  {

    C2STestRestResponse::C2STestRestResponse( C2SHttpStatus status )
      : m_status( status )
    {
    }

    C2STestRestResponse::~C2STestRestResponse()
    {
    }

    C2STestRestResponse C2STestRestResponse::build( C2SHttpStatus status )
    {
      return C2STestRestResponse( status );
    }

    C2STestRestResponse &C2STestRestResponse::entity( const C2SHttpMediaType &mediatype , const std::string &entity )
    {
      m_entity = entity;
      m_mediatype = mediatype;
      return *this;
    }

    void C2STestRestResponse::check( const C2SHttpResponse &response ) const
    {
      BOOST_MESSAGE( "response status: " << response.header().Status << "; Phrase: \"" + response.header().ReasonPhrase + "\"" );
      BOOST_CHECK( response.header().Status == m_status );

      if ( m_entity.size() )
      {
        BOOST_MESSAGE( "response content length: " << response.header().Fields.getContentLength() );
        BOOST_CHECK( response.header().Fields.getContentLength() == m_entity.size() );
        std::string sResponseEntity;
        response.getEntity( &sResponseEntity );
        BOOST_MESSAGE( "response data: \"" << sResponseEntity << "\"" );
        BOOST_CHECK( m_entity == sResponseEntity );
        BOOST_CHECK( m_mediatype.Type == response.header().Fields.getContentType().Type );
      }
    }

  }

}
