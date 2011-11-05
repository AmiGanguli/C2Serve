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
#include "C2STestRestMethodInvalidEntity.h"
#include "C2STestRestMethodMediaTypeConverter.h"

#include "C2STestRestEntityUnstreamerXML.h"

#include "C2STestRestRequest.h"
#include "C2STestRestResponse.h"

#include <boost/test/unit_test.hpp>

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
      checkContextRootForGET();
      checkContextRootForGETWithLeadingSlash();
      checkContextRootForPOST();
      checkContextRootForPUT();
      checkContextRootForDELETE();
      checkResourceNotFound();
      checkResourceNotFoundDueToMissingSlash();
      checkResourceNotFoundDueToMissingPathParameters();
      checkResourceNotFoundDueToForbiddenPathParameters();
      checkEncodingAndDecodingOfQueryFields();
      checkEncodingAndDecodingOfQueryFieldsMissingQueryParameter();
      checkInvalidEntityType();
      checkPathParametersWithXMLResponseEntity();
      checkPathParametersWithJSONResponseEntity();
      checkPathParametersWithXMLResponseEntityAsDefault();
      checkForUndefinedContentType();
      checkForWrongContentType();
      checkForWrongMethodTypePOST();
      checkForWrongMethodTypePUT();
      checkForWrongMethodTypeDELETE();
      checkPathParametersAndQueryParametersWithXMLResponseEntity();
      checkPathParametersAndQueryParametersWithJSONResponseEntity();
      checkPathParametersAndQueryParametersWithWrongQueryParameterType();
      checkPathParametersAndQueryParametersWithWrongPathParameterType();
      checkEmptyResourceContextRoot();
      checkEmptyResourceContextRootWithLeadingSlash();
      checkEmptyResourceMethodNotFound();
      checkXMLRequestEntityToJSONResponseEntity();
      checkHTMLRequestEntityToJSONResponseEntityUnsupportedMediaTypeResponse();
    }

    void C2STestRestCheckServerResponses::checkResponse( const c2s::test::C2STestRestRequest &request , const c2s::test::C2STestRestResponse &response_check )
    {
      BOOST_MESSAGE( "processing URI \"" + request.getURI() + "\"" );
      c2s::C2SHttpResponse response = request.process();
      response_check.check( response );
    }

    void C2STestRestCheckServerResponses::checkContextRootForGET()
    {
      checkResponse(

          c2s::test::C2STestRestRequest::
          build( c2s::GET , c2s::test::C2STestRestFixture::sContextRootOfTestResource )
          ,
          c2s::test::C2STestRestResponse::
          build( c2s::OK )

        );
    }

    void C2STestRestCheckServerResponses::checkContextRootForGETWithLeadingSlash()
    {
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource )
          ,
          c2s::test::C2STestRestResponse::
          build( c2s::OK )

        );
    }

    void C2STestRestCheckServerResponses::checkContextRootForPOST()
    {
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::POST , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource )
          ,
          c2s::test::C2STestRestResponse::
          build( c2s::MethodNotAllowed )

        );
    }

    void C2STestRestCheckServerResponses::checkContextRootForPUT()
    {
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::PUT , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource )
          ,
          c2s::test::C2STestRestResponse::
          build( c2s::MethodNotAllowed )

        );
    }

    void C2STestRestCheckServerResponses::checkContextRootForDELETE()
    {
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::DELETE , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource )
          ,
          c2s::test::C2STestRestResponse::
          build( c2s::MethodNotAllowed )

        );
    }

    void C2STestRestCheckServerResponses::checkResourceNotFound()
    {
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/not-a-method" )
          ,
          c2s::test::C2STestRestResponse::
          build( c2s::NotFound )

        );
    }

    void C2STestRestCheckServerResponses::checkResourceNotFoundDueToMissingSlash()
    {
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + c2s::test::C2STestRestMethodQueryFields::sPath )
          ,
          c2s::test::C2STestRestResponse::
          build( c2s::NotFound )

        );
    }

    void C2STestRestCheckServerResponses::checkResourceNotFoundDueToMissingPathParameters()
    {
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodAdd::sPath )
          ,
          c2s::test::C2STestRestResponse::
          build( c2s::NotFound )

        );
    }

    void C2STestRestCheckServerResponses::checkResourceNotFoundDueToForbiddenPathParameters()
    {
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodAdd::sPath + "/2/4/invalid" )
          ,
          c2s::test::C2STestRestResponse::
          build( c2s::NotFound )

        );
    }

    void C2STestRestCheckServerResponses::checkEncodingAndDecodingOfQueryFields()
    {
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodQueryFields::sPath ).
          query_field( "test_uri" , c2s::test::C2STestRestMethodQueryFields::sQueryFieldValueExpected )
          ,
          c2s::test::C2STestRestResponse::
          build( c2s::OK )

        );
    }

    void C2STestRestCheckServerResponses::checkEncodingAndDecodingOfQueryFieldsMissingQueryParameter()
    {
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodQueryFields::sPath )
          ,
          c2s::test::C2STestRestResponse::
          build( c2s::BadRequest )

        );
    }

    void C2STestRestCheckServerResponses::checkInvalidEntityType()
    {
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodInvalidEntity::sPath )
          ,
          c2s::test::C2STestRestResponse::build( c2s::InternalServerError )

        );
    }

    void C2STestRestCheckServerResponses::checkPathParametersWithXMLResponseEntity()
    {
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodAdd::sPath + "/2/4" ).
          accept( c2s::C2SHttpMediaType::application__xml )
          ,
          c2s::test::C2STestRestResponse::build( c2s::OK ).
          entity( c2s::C2SHttpMediaType::application__xml , "<result>0</result>" )

        );
    }

    void C2STestRestCheckServerResponses::checkPathParametersWithJSONResponseEntity()
    {
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodAdd::sPath + "/2/4" ).
          accept( c2s::C2SHttpMediaType::application__json )
          ,
          c2s::test::C2STestRestResponse::build( c2s::OK ).
          entity( c2s::C2SHttpMediaType::application__json , "{ \"result\" : 0 }" )

        );
    }

    void C2STestRestCheckServerResponses::checkPathParametersWithXMLResponseEntityAsDefault()
    {
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodAdd::sPath + "/2/4" ).
          accept( c2s::C2SHttpMediaType::wildcard )
          ,
          c2s::test::C2STestRestResponse::build( c2s::OK ).
          entity( c2s::C2SHttpMediaType::application__xml , "<result>0</result>" )

        );
    }

    void C2STestRestCheckServerResponses::checkForUndefinedContentType()
    {
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodAdd::sPath + "/2/4" )
          ,
          c2s::test::C2STestRestResponse::build( c2s::NotAcceptable )

        );
    }

    void C2STestRestCheckServerResponses::checkForWrongContentType()
    {
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodAdd::sPath + "/2/4" ).
          accept( c2s::C2SHttpMediaType::application__xhtml_xml )
          ,
          c2s::test::C2STestRestResponse::build( c2s::NotAcceptable )

        );
    }

    void C2STestRestCheckServerResponses::checkForWrongMethodTypePOST()
    {
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::POST , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodAdd::sPath + "/2/4" ).
          accept( c2s::C2SHttpMediaType::application__xml )
          ,
          c2s::test::C2STestRestResponse::build( c2s::MethodNotAllowed )

        );
    }

    void C2STestRestCheckServerResponses::checkForWrongMethodTypePUT()
    {
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::PUT , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodAdd::sPath + "/2/4" ).
          accept( c2s::C2SHttpMediaType::application__xml )
          ,
          c2s::test::C2STestRestResponse::build( c2s::MethodNotAllowed )

        );
    }

    void C2STestRestCheckServerResponses::checkForWrongMethodTypeDELETE()
    {
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::DELETE , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodAdd::sPath + "/2/4" ).
          accept( c2s::C2SHttpMediaType::application__xml )
          ,
          c2s::test::C2STestRestResponse::build( c2s::MethodNotAllowed )

        );
    }

    void C2STestRestCheckServerResponses::checkPathParametersAndQueryParametersWithXMLResponseEntity()
    {
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodAdd::sPath + "/2/4" ).
          accept( c2s::C2SHttpMediaType::application__xml ).
          query_field( "multiplier" , "2" )
          ,
          c2s::test::C2STestRestResponse::build( c2s::OK ).
          entity( c2s::C2SHttpMediaType::application__xml , "<result>12</result>" )

        );
    }

    void C2STestRestCheckServerResponses::checkPathParametersAndQueryParametersWithJSONResponseEntity()
    {
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodAdd::sPath + "/2/4" ).
          accept( c2s::C2SHttpMediaType::application__json ).
          query_field( "multiplier" , "2" )
          ,
          c2s::test::C2STestRestResponse::build( c2s::OK ).
          entity( c2s::C2SHttpMediaType::application__json , "{ \"result\" : 12 }" )

        );
    }

    void C2STestRestCheckServerResponses::checkPathParametersAndQueryParametersWithWrongQueryParameterType()
    {
      //TODO: check '2k' (???)
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodAdd::sPath + "/2/4" ).
          accept( c2s::C2SHttpMediaType::application__json ).
          query_field( "multiplier" , "k2" )
          ,
          c2s::test::C2STestRestResponse::build( c2s::BadRequest )

        );
    }

    void C2STestRestCheckServerResponses::checkPathParametersAndQueryParametersWithWrongPathParameterType()
    {
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodAdd::sPath + "/string/4" ).
          accept( c2s::C2SHttpMediaType::application__json ).
          query_field( "multiplier" , "2" )
          ,
          c2s::test::C2STestRestResponse::build( c2s::NotFound )

        );
    }

    void C2STestRestCheckServerResponses::checkEmptyResourceContextRoot()
    {
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , c2s::test::C2STestRestFixture::sContextRootOfEmptyResource )
          ,
          c2s::test::C2STestRestResponse::
          build( c2s::OK )

        );
    }

    void C2STestRestCheckServerResponses::checkEmptyResourceContextRootWithLeadingSlash()
    {
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfEmptyResource )
          ,
          c2s::test::C2STestRestResponse::
          build( c2s::OK )

        );
    }

    void C2STestRestCheckServerResponses::checkEmptyResourceMethodNotFound()
    {
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfEmptyResource + "/not-a-method" )
          ,
          c2s::test::C2STestRestResponse::
          build( c2s::NotFound )

        );
    }

    void C2STestRestCheckServerResponses::checkXMLRequestEntityToJSONResponseEntity()
    {
      std::string sEntityData = "<" + C2STestRestEntityUnstreamerXML::sRootElementID + ">";
      sEntityData += "my content";
      sEntityData += "</" + C2STestRestEntityUnstreamerXML::sRootElementID + ">";

      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::POST , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodMediaTypeConverter::sPath ).
          accept( c2s::C2SHttpMediaType::application__xml ).
          entity( c2s::C2SHttpMediaType::application__xml , sEntityData )
          ,
          c2s::test::C2STestRestResponse::
          build( c2s::Created )

        );
    }

    void C2STestRestCheckServerResponses::checkHTMLRequestEntityToJSONResponseEntityUnsupportedMediaTypeResponse()
    {
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::POST , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodMediaTypeConverter::sPath ).
          accept( c2s::C2SHttpMediaType::application__xml ).
          entity( c2s::C2SHttpMediaType::text__html , "<html>my content</html>" )
          ,
          c2s::test::C2STestRestResponse::
          build( c2s::UnsupportedMediaType )

        );
    }

  }

}
