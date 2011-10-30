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

#include "C2STestRestCheckServerResponses.h"

#include "C2STestRestFixture.h"

#include "C2STestRestMethodAdd.h"
#include "C2STestRestMethodThreading.h"
#include "C2STestRestMethodQueryFields.h"

#include "C2STestRestRequest.h"
#include "C2STestRestResponse.h"

namespace c2s
{

  namespace test
  {

    C2STestRestCheckServerResponses::C2STestRestCheckServerResponses()
    {

    }

    C2STestRestCheckServerResponses::~C2STestRestCheckServerResponses()
    {
    }

    void C2STestRestCheckServerResponses::runTest()
    {

      //check response of resource context root
      checkResponse(

          c2s::test::C2STestRestRequest::
          build( c2s::GET , c2s::test::C2STestRestFixture::sContextRootOfTestResource )
          ,
          c2s::test::C2STestRestResponse::
          build( c2s::OK )

        );
      //check response of resource context root (leading '/')
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource )
          ,
          c2s::test::C2STestRestResponse::
          build( c2s::OK )

        );
      //check response of resource context root (POST method (not allowed))
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::POST , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource )
          ,
          c2s::test::C2STestRestResponse::
          build( c2s::MethodNotAllowed )

        );
      //check response of resource context root (PUT method (not allowed))
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::PUT , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource )
          ,
          c2s::test::C2STestRestResponse::
          build( c2s::MethodNotAllowed )

        );
      //check response of resource context root (DELETE method (not allowed))
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::DELETE , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource )
          ,
          c2s::test::C2STestRestResponse::
          build( c2s::MethodNotAllowed )

        );

      //check response of resource that cannot be found
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/not-a-method" )
          ,
          c2s::test::C2STestRestResponse::
          build( c2s::NotFound )

        );
      //check response of resource that cannot be found due to forgotten '/'
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + c2s::test::C2STestRestMethodQueryFields::sPath )
          ,
          c2s::test::C2STestRestResponse::
          build( c2s::NotFound )

        );

      //check encoding and decoding of http query fields (missing query parameter)
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodQueryFields::sPath )
          ,
          c2s::test::C2STestRestResponse::
          build( c2s::BadRequest )

        );
      //check encoding and decoding of http query fields
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodQueryFields::sPath ).
          query_field( "test_uri" , c2s::test::C2STestRestMethodQueryFields::sQueryFieldValueExpected )
          ,
          c2s::test::C2STestRestResponse::
          build( c2s::OK )

        );

      //check functionality of path parameters (no path parameters specified)
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodAdd::sPath )
          ,
          c2s::test::C2STestRestResponse::
          build( c2s::NotFound )

        );
      //check functionality of path parameters (one parameter too much specified)
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodAdd::sPath + "/2/4/invalid" )
          ,
          c2s::test::C2STestRestResponse::
          build( c2s::NotFound )

        );

      //check functionality of path parameters (check XML response entity)
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodAdd::sPath + "/2/4" ).
          accept( c2s::C2SHttpMediaType::application__xml )
          ,
          c2s::test::C2STestRestResponse::build( c2s::OK ).
          entity( c2s::C2SHttpMediaType::application__xml , "<result>0</result>" )

        );
      //check functionality of path parameters (check JSON response entity)
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodAdd::sPath + "/2/4" ).
          accept( c2s::C2SHttpMediaType::application__json )
          ,
          c2s::test::C2STestRestResponse::build( c2s::OK ).
          entity( c2s::C2SHttpMediaType::application__json , "{ \"result\" : 0 }" )

        );
      //check functionality of path parameters (check XML response entity as default)
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodAdd::sPath + "/2/4" ).
          accept( c2s::C2SHttpMediaType::wildcard )
          ,
          c2s::test::C2STestRestResponse::build( c2s::OK ).
          entity( c2s::C2SHttpMediaType::application__xml , "<result>0</result>" )

        );
      //check functionality of path parameters (no content type defined)
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodAdd::sPath + "/2/4" )
          ,
          c2s::test::C2STestRestResponse::build( c2s::NotAcceptable )

        );
      //check functionality of path parameters (wrong content type defined)
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodAdd::sPath + "/2/4" ).
          accept( c2s::C2SHttpMediaType::application__xhtml_xml )
          ,
          c2s::test::C2STestRestResponse::build( c2s::NotAcceptable )

        );
      //check functionality of path parameters (wrong method type)
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::POST , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodAdd::sPath + "/2/4" ).
          accept( c2s::C2SHttpMediaType::application__xml )
          ,
          c2s::test::C2STestRestResponse::build( c2s::MethodNotAllowed )

        );
      //check functionality of path parameters (wrong method type)
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::PUT , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodAdd::sPath + "/2/4" ).
          accept( c2s::C2SHttpMediaType::application__xml )
          ,
          c2s::test::C2STestRestResponse::build( c2s::MethodNotAllowed )

        );
      //check functionality of path parameters (wrong method type)
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::DELETE , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodAdd::sPath + "/2/4" ).
          accept( c2s::C2SHttpMediaType::application__xml )
          ,
          c2s::test::C2STestRestResponse::build( c2s::MethodNotAllowed )

        );
      //check functionality of path parameters and query field parameters
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodAdd::sPath + "/2/4" ).
          accept( c2s::C2SHttpMediaType::application__xml ).
          query_field( "multiplier" , "2" )
          ,
          c2s::test::C2STestRestResponse::build( c2s::OK ).
          entity( c2s::C2SHttpMediaType::application__xml , "<result>12</result>" )

        );
      //check functionality of path parameters and query field parameters
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodAdd::sPath + "/2/4" ).
          accept( c2s::C2SHttpMediaType::application__json ).
          query_field( "multiplier" , "2" )
          ,
          c2s::test::C2STestRestResponse::build( c2s::OK ).
          entity( c2s::C2SHttpMediaType::application__json , "{ \"result\" : 12 }" )

        );
      //check functionality of path parameters and query field parameters (string specified)
      //TODO: check '2k' (???)
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodAdd::sPath + "/2/4" ).
          accept( c2s::C2SHttpMediaType::application__json ).
          query_field( "multiplier" , "k2" )
          ,
          c2s::test::C2STestRestResponse::build( c2s::BadRequest )

        );
      //check functionality of path parameters and query field parameters (string specified)
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodAdd::sPath + "/string/4" ).
          accept( c2s::C2SHttpMediaType::application__json ).
          query_field( "multiplier" , "2" )
          ,
          c2s::test::C2STestRestResponse::build( c2s::NotFound )

        );

      //check second resource
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , c2s::test::C2STestRestFixture::sContextRootOfEmptyResource )
          ,
          c2s::test::C2STestRestResponse::
          build( c2s::OK )

        );
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfEmptyResource )
          ,
          c2s::test::C2STestRestResponse::
          build( c2s::OK )

        );
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfEmptyResource + "/not-a-method" )
          ,
          c2s::test::C2STestRestResponse::
          build( c2s::NotFound )

        );
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfEmptyResource + "not-a-method" )
          ,
          c2s::test::C2STestRestResponse::
          build( c2s::NotFound )

        );

    }

    void C2STestRestCheckServerResponses::checkResponse( const c2s::test::C2STestRestRequest &request , const c2s::test::C2STestRestResponse &response_check )
    {
      c2s::C2SHttpResponse response = request.process();
      response_check.check( response );
    }

  }

}
