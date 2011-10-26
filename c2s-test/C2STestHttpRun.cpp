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

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "C2STestServerRunner.h"

#include "C2SHttpClient.h"
#include "C2SHttpRequest.h"

#include "StringUtils.h"

#include <fstream>

#define USE_PORT 8182


class C2STestHttpRun
{
public:

  C2STestHttpRun() : m_sr( USE_PORT ) {};

private:

  g::c2s::test::C2STestServerRunner m_sr;

};

BOOST_GLOBAL_FIXTURE( C2STestHttpRun );

BOOST_AUTO_TEST_CASE( HtmlDefault )
{
  g::c2s::C2SHttpRequestHeader requestHeader( g::c2s::GET , "/" );
  requestHeader.Fields.addAccept( g::c2s::C2SHttpMediaType::wildcard );
  requestHeader.Fields.addAccept( g::c2s::C2SHttpMediaType::text__html );
  g::c2s::C2SHttpRequest request( requestHeader );

  g::c2s::C2SHttpClient client( "localhost" , USE_PORT );
  g::c2s::C2SHttpResponse response = client.send( request );

  BOOST_MESSAGE( "response status: " + g::util::toString( response.header().Status ) );

  BOOST_CHECK( response.header().fVersion == 1.1f );
  BOOST_CHECK( response.header().Status == g::c2s::OK );

  std::string sIndexFile = g::c2s::C2SGlobalSettings::Settings().C2SDocumentRoot + "index.html";
  BOOST_MESSAGE( "reading file " + sIndexFile );
  std::ifstream is( sIndexFile.c_str() );
  BOOST_REQUIRE( is.is_open() );

  is.seekg( 0 , std::ios::end );
  unsigned int size = static_cast<unsigned int>( is.tellg() );
  is.seekg( 0 , std::ios::beg );

  BOOST_MESSAGE( "expected content length: " + g::util::toString( size ) );
  BOOST_MESSAGE( "received content length: " + g::util::toString( response.header().Fields.getContentLength() ) );

  BOOST_REQUIRE( size == response.header().Fields.getContentLength() );
  char *refContent = new char[ size ];
  is.read( refContent , size );

  const g::c2s::C2SHttpEntity *pEntity = response.getEntity();
  BOOST_REQUIRE( pEntity );
  BOOST_MESSAGE( "received body length: " + g::util::toString( pEntity->size ) );
  BOOST_REQUIRE( pEntity->size == static_cast<unsigned int>( size ) );

  std::string sRefData( refContent , size );
  std::string sData( pEntity->data , pEntity->size );

  BOOST_CHECK( sRefData == sData );

  delete[] refContent;
}
