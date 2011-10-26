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

#include "C2SHttpParser.h"
#include "C2SHttpRequest.h"
#include "C2SHttpResponse.h"

#include "FileUtils.h"
#include "StringUtils.h"

#include "GlobalDefines.h"

#define USE_PORT 8181

//TODO: redirect server logging to BOOST_MESSAGE

//BOOST_AUTO_TEST_CASE( gMapTest )
//{
//  std::map<std::string,std::string> resources;
//
//  resources[ "/" ] = "index.html";
//  resources[ "/default" ] = "default.html";
//  resources[ "test/basic/" ] = "myVarResource";
//  resources[ "test/basic/echo" ] = "myEchoResource";
//
//  BOOST_MESSAGE( resources.lower_bound( "/test/basic/echo" )->second );
//}

//TODO: move test to core/util
BOOST_AUTO_TEST_CASE( URLCoding )
{
  std::string s1 = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ!\"�$%&/()=?�*'-_.:;,<>";
  //same as s1 but without '%'
  std::string s1_2 = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ!\"�$&/()=?�*'-_.:;,<>";
  std::string sURLEncoded = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ%21%22%a4%24%25%26%2f%28%29%3d%3f%a7%2a%27-_.%3a%3b%2c%3c%3e";

  std::string s2 = g::util::urlEncode( s1 );
  BOOST_MESSAGE( "url encoded: " << s2 );
  BOOST_CHECK( s2 == sURLEncoded );

  std::string s3 = g::util::urlDecode( s2 );
  BOOST_MESSAGE( "url decoded: " << s3 );
  BOOST_CHECK( s1 == s3 );

  std::string s4 = g::util::urlDecode( s1_2 );
  BOOST_MESSAGE( "url decoded without url encoding: " << s4 );
  BOOST_CHECK( s1_2 == s4 );
}

BOOST_AUTO_TEST_CASE( HttpResponseOK )
{
  g::c2s::C2SHttpParser parser;
  g::c2s::C2SHttpResponseHeader header;

  std::string sChunk1 = "\n\nHTTP/1.1 200\r";
  std::string sChunk2 = "\nContent-Length: ";
  std::string sChunk3 = "44\r\nContent-Type: text/html\r\n\r\nThisIsMyContent\n\n\n";
  std::string sChunk4 = "This is my content part II";

  parser.parse( sChunk1.c_str() , sChunk1.size() , &header );
  parser.parse( sChunk2.c_str() , sChunk2.size() , &header );
  parser.parse( sChunk3.c_str() , sChunk3.size() , &header );
  parser.parse( sChunk4.c_str() , sChunk4.size() , &header );

  BOOST_CHECK( header.fVersion == 1.1f );
  BOOST_CHECK( header.Status == g::c2s::OK );
  BOOST_CHECK( !header.ReasonPhrase.size() );

  BOOST_CHECK( header.Fields.getContentType().Type == g::c2s::C2SHttpMediaType::text__html );
  BOOST_CHECK( header.Fields.getContentType().fQ == 1.f );

  BOOST_REQUIRE( header.Fields.getContentLength() == 44 );

  char *entity = parser.entity( header.Fields.getContentLength() );
  std::string sEntity( entity , header.Fields.getContentLength() );
  BOOST_CHECK( sEntity == "ThisIsMyContent\n\n\nThis is my content part II" );
}

BOOST_AUTO_TEST_CASE( HttpResponseNotFound )
{
  g::c2s::C2SHttpParser parser;
  g::c2s::C2SHttpResponseHeader header;

  std::string sChunk1 = "\n\nHTTP/1.1 404 Not Found\r";
  std::string sChunk2 = "\n\r\n";

  parser.parse( sChunk1.c_str() , sChunk1.size() , &header );
  parser.parse( sChunk2.c_str() , sChunk2.size() , &header );

  BOOST_CHECK( header.fVersion == 1.1f );
  BOOST_CHECK( header.Status == g::c2s::NotFound );
  BOOST_CHECK( header.ReasonPhrase == "Not Found" );
}

BOOST_AUTO_TEST_CASE( HttpRequestPOST1 )
{
  g::c2s::C2SHttpParser parser;
  g::c2s::C2SHttpRequestHeader header;

  std::string sChunk1 = "\n\nPOST /c2s/test/http-parser HTTP/1.1\r";
  std::string sChunk2 = "\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.834\r\nContent-Length: 44\nContent-Type: application/x-www";
  std::string sChunk3 = "-form-urlencoded\r\n\r\nThisIsMyContent\n\n\n";
  std::string sChunk4 = "This is my content part II";
  parser.parse( sChunk1.c_str() , sChunk1.size() , &header );
  parser.parse( sChunk2.c_str() , sChunk2.size() , &header );
  parser.parse( sChunk3.c_str() , sChunk3.size() , &header );
  parser.parse( sChunk4.c_str() , sChunk4.size() , &header );

  BOOST_CHECK( header.Method == g::c2s::POST );
  BOOST_CHECK( header.URI == "/c2s/test/http-parser" );
  BOOST_CHECK( header.Version == 1.1f );

  BOOST_CHECK( header.Fields.isAccept( g::c2s::C2SHttpMediaType::text__html ) );
  BOOST_CHECK( header.Fields.isAccept( g::c2s::C2SHttpMediaType::application__xhtml_xml ) );
  BOOST_REQUIRE( header.Fields.isAccept( g::c2s::C2SHttpMediaType::application__xml ) );
  BOOST_REQUIRE( header.Fields.isAccept( g::c2s::C2SHttpMediaType::wildcard ) );
  BOOST_REQUIRE( !header.Fields.isAccept( g::c2s::C2SHttpMediaType::application__x_www_form_urlencoded ) );

  BOOST_CHECK( header.Fields.getAccept( g::c2s::C2SHttpMediaType::text__html ).fQ == 1.f );
  BOOST_CHECK( header.Fields.getAccept( g::c2s::C2SHttpMediaType::application__xml ).fQ == 0.9f );
  BOOST_CHECK( header.Fields.getAccept( g::c2s::C2SHttpMediaType::wildcard ).fQ == 0.834f );

  BOOST_CHECK( header.Fields.getContentType().Type == g::c2s::C2SHttpMediaType::application__x_www_form_urlencoded );
  BOOST_CHECK( header.Fields.getContentType().fQ == 1.f );

  BOOST_REQUIRE( header.Fields.getContentLength() == 44 );

  char *entity = parser.entity( header.Fields.getContentLength() );
  std::string sEntity( entity , header.Fields.getContentLength() );
  BOOST_CHECK( sEntity == "ThisIsMyContent\n\n\nThis is my content part II" );
}

BOOST_AUTO_TEST_CASE( HttpRequestPOST2 )
{
  g::c2s::C2SHttpParser parser;
  g::c2s::C2SHttpRequestHeader header;

  std::string sChunk1 = "\n\nPOST /c2s/test/http-parser/ HTTP/1.1\r\nAccept: text/html,";
  std::string sChunk2 = "application/xhtml+xml,application/xml;q=0.9,*/*;q=0.834\r\nContent-Length: 44\nContent-Type: application/x-www";
  std::string sChunk3 = "-form-urlencoded\r";
  std::string sChunk4 = "\n\r\nThisIsMyContent\n\n\nThis is my content part II";
  parser.parse( sChunk1.c_str() , sChunk1.size() , &header );
  parser.parse( sChunk2.c_str() , sChunk2.size() , &header );
  parser.parse( sChunk3.c_str() , sChunk3.size() , &header );
  parser.parse( sChunk4.c_str() , sChunk4.size() , &header );

  BOOST_CHECK( header.Method == g::c2s::POST );
  BOOST_CHECK( header.URI == "/c2s/test/http-parser/" );
  BOOST_CHECK( header.Version == 1.1f );

  BOOST_CHECK( header.Fields.isAccept( g::c2s::C2SHttpMediaType::text__html ) );
  BOOST_CHECK( header.Fields.isAccept( g::c2s::C2SHttpMediaType::application__xhtml_xml ) );
  BOOST_REQUIRE( header.Fields.isAccept( g::c2s::C2SHttpMediaType::application__xml ) );
  BOOST_REQUIRE( header.Fields.isAccept( g::c2s::C2SHttpMediaType::wildcard ) );
  BOOST_REQUIRE( !header.Fields.isAccept( g::c2s::C2SHttpMediaType::application__x_www_form_urlencoded ) );

  BOOST_CHECK( header.Fields.getAccept( g::c2s::C2SHttpMediaType::text__html ).fQ == 1.f );
  BOOST_CHECK( header.Fields.getAccept( g::c2s::C2SHttpMediaType::application__xml ).fQ == 0.9f );
  BOOST_CHECK( header.Fields.getAccept( g::c2s::C2SHttpMediaType::wildcard ).fQ == 0.834f );

  BOOST_CHECK( header.Fields.getContentType().Type == g::c2s::C2SHttpMediaType::application__x_www_form_urlencoded );
  BOOST_CHECK( header.Fields.getContentType().fQ == 1.f );

  BOOST_REQUIRE( header.Fields.getContentLength() == 44 );

  char *entity = parser.entity( header.Fields.getContentLength() );
  std::string sEntity( entity , header.Fields.getContentLength() );
  BOOST_CHECK( sEntity == "ThisIsMyContent\n\n\nThis is my content part II" );
}

BOOST_AUTO_TEST_CASE( HttpRequestPOST3 )
{
  g::c2s::C2SHttpParser parser;
  g::c2s::C2SHttpRequestHeader header;

  std::string sChunk1 = "\n\nPOST /c2s/test/http-parser?fieldname1=value1&fieldname2=value2&fieldname3=value3 HTTP/1.1\r\nAccept: text/html,";
  std::string sChunk2 = "application/xhtml+xml,application/xml;q=0.9,*/*;q=0.834\r\nContent-Length: 44\nContent-Type: application/x-www";
  std::string sChunk3 = "-form-urlencoded\r";
  std::string sChunk4 = "\n\r\nThisIsMyContent\n\n\nThis is my content part II";
  parser.parse( sChunk1.c_str() , sChunk1.size() , &header );
  parser.parse( sChunk2.c_str() , sChunk2.size() , &header );
  parser.parse( sChunk3.c_str() , sChunk3.size() , &header );
  parser.parse( sChunk4.c_str() , sChunk4.size() , &header );

  BOOST_CHECK( header.QueryFields.get( "fieldname1" ) == "value1" );
  BOOST_CHECK( header.QueryFields.get( "fieldname2" ) == "value2" );
  BOOST_CHECK( header.QueryFields.get( "fieldname3" ) == "value3" );

  BOOST_CHECK( header.Method == g::c2s::POST );
  BOOST_CHECK( header.URI == "/c2s/test/http-parser" );
  BOOST_CHECK( header.Version == 1.1f );

  BOOST_CHECK( header.Fields.isAccept( g::c2s::C2SHttpMediaType::text__html ) );
  BOOST_CHECK( header.Fields.isAccept( g::c2s::C2SHttpMediaType::application__xhtml_xml ) );
  BOOST_REQUIRE( header.Fields.isAccept( g::c2s::C2SHttpMediaType::application__xml ) );
  BOOST_REQUIRE( header.Fields.isAccept( g::c2s::C2SHttpMediaType::wildcard ) );
  BOOST_REQUIRE( !header.Fields.isAccept( g::c2s::C2SHttpMediaType::application__x_www_form_urlencoded ) );

  BOOST_CHECK( header.Fields.getAccept( g::c2s::C2SHttpMediaType::text__html ).fQ == 1.f );
  BOOST_CHECK( header.Fields.getAccept( g::c2s::C2SHttpMediaType::application__xml ).fQ == 0.9f );
  BOOST_CHECK( header.Fields.getAccept( g::c2s::C2SHttpMediaType::wildcard ).fQ == 0.834f );

  BOOST_CHECK( header.Fields.getContentType().Type == g::c2s::C2SHttpMediaType::application__x_www_form_urlencoded );
  BOOST_CHECK( header.Fields.getContentType().fQ == 1.f );

  BOOST_REQUIRE( header.Fields.getContentLength() == 44 );

  char *entity = parser.entity( header.Fields.getContentLength() );
  std::string sEntity( entity , header.Fields.getContentLength() );
  BOOST_CHECK( sEntity == "ThisIsMyContent\n\n\nThis is my content part II" );
}

BOOST_AUTO_TEST_CASE( HttpRequestGET )
{
  g::c2s::C2SHttpParser parser;
  g::c2s::C2SHttpRequestHeader header;

  header.Method = g::c2s::POST;

  std::string sChunk1 = "\n\nGET /c2s/test/http-parser/ HTTP/1.1\r\nAccept: text/html,";
  std::string sChunk2 = "application/xhtml+xml,application/xml;q=0.9,*/*;q=0.834\r\n";

  parser.parse( sChunk1.c_str() , sChunk1.size() , &header );
  parser.parse( sChunk2.c_str() , sChunk2.size() , &header );

  BOOST_CHECK( header.Method == g::c2s::GET );
  BOOST_CHECK( header.URI == "/c2s/test/http-parser/" );
  BOOST_CHECK( header.Version == 1.1f );

  BOOST_CHECK( header.Fields.isAccept( g::c2s::C2SHttpMediaType::text__html ) );
  BOOST_CHECK( header.Fields.isAccept( g::c2s::C2SHttpMediaType::application__xhtml_xml ) );
  BOOST_REQUIRE( header.Fields.isAccept( g::c2s::C2SHttpMediaType::application__xml ) );
  BOOST_REQUIRE( header.Fields.isAccept( g::c2s::C2SHttpMediaType::wildcard ) );
  BOOST_REQUIRE( !header.Fields.isAccept( g::c2s::C2SHttpMediaType::application__x_www_form_urlencoded ) );

  BOOST_CHECK( header.Fields.getAccept( g::c2s::C2SHttpMediaType::text__html ).fQ == 1.f );
  BOOST_CHECK( header.Fields.getAccept( g::c2s::C2SHttpMediaType::application__xml ).fQ == 0.9f );
  BOOST_CHECK( header.Fields.getAccept( g::c2s::C2SHttpMediaType::wildcard ).fQ == 0.834f );

  BOOST_CHECK( header.Fields.getContentLength() == 0 );
}

//TODO: move this check to string utils
void checkUriEquals( bool bExpectation , const std::string &s1 , const std::string &s2 , bool bCheckLeadingSlash = true )
{
  BOOST_MESSAGE( "compare uri: \"" + s1 + "\" == \"" + s2 + "\"" );
  BOOST_CHECK( bExpectation == g::c2s::uriEquals( s1.c_str() , s1.size() , s2.c_str() , s2.size() , bCheckLeadingSlash ) );
}

BOOST_AUTO_TEST_CASE( URICompare )
{
  checkUriEquals( true , "" , "" );
  checkUriEquals( false , "" , "/" );
  checkUriEquals( false , "/" , "" );
  checkUriEquals( true , "" , "/" , false );
  checkUriEquals( true , "/" , "" , false );
  checkUriEquals( true , "//" , "/" );
  checkUriEquals( true , "//" , "///" );
  checkUriEquals( true , "/uri/" , "/uri/" );
  checkUriEquals( false , "uri/" , "/uri/" );
  checkUriEquals( true , "uri/" , "/uri/" , false );
  checkUriEquals( true , "//uri/" , "/uri//" );
  checkUriEquals( false , "//uri/" , "/uri//sub" );
  checkUriEquals( true , "//uri/sub/" , "/uri//sub" );
}

void checkUriSkip( int iExpectation , const std::string &s1 , const std::string &s2 , bool bCheckLeadingSlash = true )
{
  int iSkipPoint = g::c2s::uriSkip( s1.c_str() , s1.size() , s2.c_str() , s2.size() , bCheckLeadingSlash );
  BOOST_MESSAGE( "skip uri: \"" + s1 + "\" == \"" + s2 + "\"" + "; result: " + g::util::toString( iSkipPoint ) + "; expected: " + g::util::toString( iExpectation ) );
  BOOST_CHECK( iExpectation == iSkipPoint );
}

BOOST_AUTO_TEST_CASE( URISkip )
{
  checkUriSkip( -1 , "/uri" , "uri" );
  checkUriSkip( 4 , "/uri" , "uri" , false );
  checkUriSkip( -1 , "uri" , "/uri" );
  checkUriSkip( 3 , "uri" , "/uri" , false );
  checkUriSkip( 3 , "uri" , "uri/" );
  checkUriSkip( 6 , "uri///sub/" , "uri/" );
  checkUriSkip( 0 , "" , "" );
  checkUriSkip( -1 , "" , "/" );
  checkUriSkip( -1 , "/" , "" );
  checkUriSkip( 0 , "" , "/" , false );
  checkUriSkip( 1 , "/" , "" , false );
  checkUriSkip( 2 , "//" , "/" );
  checkUriSkip( 2 , "//" , "///" );
  checkUriSkip( 5 , "/uri/" , "/uri/" );
  checkUriSkip( -1 , "uri/" , "/uri/" );
  checkUriSkip( 4 , "uri/" , "/uri/" , false );
  checkUriSkip( 6 , "//uri/" , "/uri//" );
  checkUriSkip( -1 , "//uri/" , "/uri//sub" );
  checkUriSkip( 10 , "//uri/sub/" , "/uri//sub" );
  checkUriSkip( 11 , "//uri/sub//subsub///" , "/uri//sub" );
  checkUriSkip( -1 , "//uri/sub/" , "/uri//sub///subsub///" );
}

/*BOOST_AUTO_TEST_CASE( ServerStartupShutdown )
{
  g::c2s::test::C2STestServerRunner sr( USE_PORT );

  std::string sCWD = g::util::getCWD();
  BOOST_CHECK( g::c2s::C2SStatus::Status().C2SHome == sCWD + "/c2s/" );
  BOOST_CHECK( g::c2s::C2SGlobalSettings::Settings().sC2SDocumentRoot == sCWD + "/c2s/deploy/ROOT/" );

}*/

