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

#include "C2STestRestEntityStreamerXML.h"
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
      checkForMethodTypePOSTWithDuplicateGET();
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
      checkJSONRequestEntityToXMLResponseEntity();
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
      BOOST_MESSAGE( "checkContextRootForGET" );
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
      BOOST_MESSAGE( "checkContextRootForGETWithLeadingSlash" );
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
      BOOST_MESSAGE( "checkContextRootForPOST" );
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
      BOOST_MESSAGE( "checkContextRootForPUT" );
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
      BOOST_MESSAGE( "checkContextRootForDELETE" );
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
      BOOST_MESSAGE( "checkResourceNotFound" );
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
      BOOST_MESSAGE( "checkResourceNotFoundDueToMissingSlash" );
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
      BOOST_MESSAGE( "checkResourceNotFoundDueToMissingPathParameters" );
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
      BOOST_MESSAGE( "checkResourceNotFoundDueToForbiddenPathParameters" );
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
      BOOST_MESSAGE( "checkEncodingAndDecodingOfQueryFields" );
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodQueryFields::sPath ).
          query_field( C2STestRestMethodQueryFields::sQueryField1Name , c2s::test::C2STestRestMethodQueryFields::sQueryField1ValueExpected ).
          query_field( C2STestRestMethodQueryFields::sQueryField2Name , c2s::test::C2STestRestMethodQueryFields::sQueryField2ValueExpected )
          ,
          c2s::test::C2STestRestResponse::
          build( c2s::OK )

        );
    }

    void C2STestRestCheckServerResponses::checkEncodingAndDecodingOfQueryFieldsMissingQueryParameter()
    {
      BOOST_MESSAGE( "checkEncodingAndDecodingOfQueryFieldsMissingQueryParameter" );
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
      BOOST_MESSAGE( "checkInvalidEntityType" );
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodInvalidEntity::sPath )
          ,
          c2s::test::C2STestRestResponse::build( c2s::InternalServerError )

        );
    }

    void C2STestRestCheckServerResponses::checkPathParametersWithXMLResponseEntity()
    {
      BOOST_MESSAGE( "checkPathParametersWithXMLResponseEntity" );
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodAdd::sPath + "/2/4" ).
          accept( c2s::C2SHttpMediaType::application__xml )
          ,
          c2s::test::C2STestRestResponse::build( c2s::OK ).
          entity( c2s::C2SHttpMediaType::application__xml , "<" + C2STestRestEntityStreamerXML<unsigned int>::sRootElement + ">0</" + C2STestRestEntityStreamerXML<unsigned int>::sRootElement + ">" )

        );
    }

    void C2STestRestCheckServerResponses::checkPathParametersWithJSONResponseEntity()
    {
      BOOST_MESSAGE( "checkPathParametersWithJSONResponseEntity" );
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodAdd::sPath + "/2/4" ).
          accept( c2s::C2SHttpMediaType::application__json )
          ,
          c2s::test::C2STestRestResponse::build( c2s::OK ).
          entity( c2s::C2SHttpMediaType::application__json , "{0}" )

        );
    }

    void C2STestRestCheckServerResponses::checkPathParametersWithXMLResponseEntityAsDefault()
    {
      BOOST_MESSAGE( "checkPathParametersWithXMLResponseEntityAsDefault" );
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodAdd::sPath + "/2/4" ).
          accept( c2s::C2SHttpMediaType::wildcard )
          ,
          c2s::test::C2STestRestResponse::build( c2s::OK ).
          entity( c2s::C2SHttpMediaType::application__xml , "<" + C2STestRestEntityStreamerXML<unsigned int>::sRootElement + ">0</" + C2STestRestEntityStreamerXML<unsigned int>::sRootElement + ">" )

        );
    }

    void C2STestRestCheckServerResponses::checkForUndefinedContentType()
    {
      BOOST_MESSAGE( "checkForUndefinedContentType" );
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodAdd::sPath + "/2/4" )
          ,
          c2s::test::C2STestRestResponse::build( c2s::NotAcceptable )

        );
    }

    void C2STestRestCheckServerResponses::checkForWrongContentType()
    {
      BOOST_MESSAGE( "checkForWrongContentType" );
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodAdd::sPath + "/2/4" ).
          accept( c2s::C2SHttpMediaType::application__xhtml_xml )
          ,
          c2s::test::C2STestRestResponse::build( c2s::NotAcceptable )

        );
    }

    void C2STestRestCheckServerResponses::checkForMethodTypePOSTWithDuplicateGET()
    {
      BOOST_MESSAGE( "checkForMethodTypePOSTWithDuplicateGET" );
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::POST , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodAdd::sPath + "/2/4" ).
          accept( c2s::C2SHttpMediaType::application__json )
          ,
          c2s::test::C2STestRestResponse::build( c2s::Created ).
          entity( c2s::C2SHttpMediaType::application__json , "{6}" )

        );
    }

    void C2STestRestCheckServerResponses::checkForWrongMethodTypePUT()
    {
      BOOST_MESSAGE( "checkForWrongMethodTypePUT" );
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
      BOOST_MESSAGE( "checkForWrongMethodTypeDELETE" );
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
      BOOST_MESSAGE( "checkPathParametersAndQueryParametersWithXMLResponseEntity" );
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodAdd::sPath + "/2/4" ).
          accept( c2s::C2SHttpMediaType::application__xml ).
          query_field( "multiplier" , "2" )
          ,
          c2s::test::C2STestRestResponse::build( c2s::OK ).
          entity( c2s::C2SHttpMediaType::application__xml , "<" + C2STestRestEntityStreamerXML<unsigned int>::sRootElement + ">12</" + C2STestRestEntityStreamerXML<unsigned int>::sRootElement + ">" )

        );
    }

    void C2STestRestCheckServerResponses::checkPathParametersAndQueryParametersWithJSONResponseEntity()
    {
      BOOST_MESSAGE( "checkPathParametersAndQueryParametersWithJSONResponseEntity" );
      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::GET , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodAdd::sPath + "/2/4" ).
          accept( c2s::C2SHttpMediaType::application__json ).
          query_field( "multiplier" , "2" )
          ,
          c2s::test::C2STestRestResponse::build( c2s::OK ).
          entity( c2s::C2SHttpMediaType::application__json , "{12}" )

        );
    }

    void C2STestRestCheckServerResponses::checkPathParametersAndQueryParametersWithWrongQueryParameterType()
    {
      BOOST_MESSAGE( "checkPathParametersAndQueryParametersWithWrongQueryParameterType" );
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
      BOOST_MESSAGE( "checkPathParametersAndQueryParametersWithWrongPathParameterType" );
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
      BOOST_MESSAGE( "checkEmptyResourceContextRoot" );
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
      BOOST_MESSAGE( "checkEmptyResourceContextRootWithLeadingSlash" );
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
      BOOST_MESSAGE( "checkEmptyResourceMethodNotFound" );
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
      BOOST_MESSAGE( "checkXMLRequestEntityToJSONResponseEntity" );
      std::string sEntityContent = "my content";
      std::string sEntityDataToSend = "<" + C2STestRestEntityUnstreamerXML::sRootElementID + ">";
      sEntityDataToSend += sEntityContent;
      sEntityDataToSend += "</" + C2STestRestEntityUnstreamerXML::sRootElementID + ">";

      std::string sEntityDataReceivedExpected = "{" + sEntityContent + "}";

      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::POST , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodMediaTypeConverter::sPath ).
          accept( c2s::C2SHttpMediaType::application__json ).
          entity( c2s::C2SHttpMediaType::application__xml , sEntityDataToSend )
          ,
          c2s::test::C2STestRestResponse::
          build( c2s::Created ).
          entity( c2s::C2SHttpMediaType::application__json , sEntityDataReceivedExpected )

        );
    }

    void C2STestRestCheckServerResponses::checkJSONRequestEntityToXMLResponseEntity()
    {
      BOOST_MESSAGE( "checkJSONRequestEntityToXMLResponseEntity" );
      std::string sEntityContent = "my content";
      std::string sEntityDataReceivedExpected = "<" + C2STestRestEntityStreamerXML<std::string>::sRootElement + ">";
      sEntityDataReceivedExpected += sEntityContent;
      sEntityDataReceivedExpected += "</" + C2STestRestEntityStreamerXML<std::string>::sRootElement + ">";

      std::string sEntityDataToSend = "{" + sEntityContent + "}";

      checkResponse (

          c2s::test::C2STestRestRequest::
          build( c2s::POST , "/" + c2s::test::C2STestRestFixture::sContextRootOfTestResource + "/" + c2s::test::C2STestRestMethodMediaTypeConverter::sPath ).
          accept( c2s::C2SHttpMediaType::application__xml ).
          entity( c2s::C2SHttpMediaType::application__json , sEntityDataToSend )
          ,
          c2s::test::C2STestRestResponse::
          build( c2s::Created ).
          entity( c2s::C2SHttpMediaType::application__xml , sEntityDataReceivedExpected )

        );
    }

    void C2STestRestCheckServerResponses::checkHTMLRequestEntityToJSONResponseEntityUnsupportedMediaTypeResponse()
    {
      BOOST_MESSAGE( "checkHTMLRequestEntityToJSONResponseEntityUnsupportedMediaTypeResponse" );
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
