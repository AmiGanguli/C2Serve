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

#include <boost/test/unit_test.hpp>
#include <boost/bind.hpp>

#include "C2SHttpClient.h"

#include "C2STestServerRunner.h"
#include "C2SRestResource.h"

#include "C2STestRestMethodAdd.h"
#include "C2STestRestMethodThreading.h"
#include "C2STestRestMethodQueryFields.h"
#include "C2STestRestResponse.h"

#include "TaskQueue.h"
#include "StringUtils.h"

using namespace boost::unit_test;

//#define USE_PORT 8184
#define RESOURCE_CONTEXT_ROOT_1 std::string( "g/forrest/test/rest-check/resource-1" )
#define RESOURCE_CONTEXT_ROOT_2 std::string( "g/forrest/test/rest-check/resource-2" )

std::list<g::c2s::C2SHttpResource*> createResources( g::thread::Mutex *pGlobalMutex );

class ForRestTestRestful
{
public:

  ForRestTestRestful() : m_sr( NULL )
  {
    if ( !pGlobalMutex )
      pGlobalMutex = new g::thread::Mutex();

    m_sr = new g::c2s::test::C2STestServerRunner( createResources( pGlobalMutex ) , USE_PORT );
  };

  virtual ~ForRestTestRestful()
  {
    delete m_sr;
    delete pGlobalMutex;
  }

  static g::thread::Mutex *pGlobalMutex;

private:

  g::c2s::test::C2STestServerRunner *m_sr;

};

g::thread::Mutex *ForRestTestRestful::pGlobalMutex = NULL;

BOOST_GLOBAL_FIXTURE( ForRestTestRestful );

void checkThreading();

void checkResponse( const g::c2s::test::C2STestRestRequest &request , const g::c2s::test::C2STestRestResponse &response_check )
{
  g::c2s::C2SHttpResponse response = request.process();
  response_check.check( response );
}

test_suite *init_unit_test_suite( int , char ** const )
{
  test_suite* requests = BOOST_TEST_SUITE( "requests" );
  framework::master_test_suite().add( requests );

  //check response of resource context root
  requests->add( BOOST_TEST_CASE( boost::bind( &checkResponse ,

      g::c2s::test::C2STestRestRequest::
      build( g::c2s::GET , RESOURCE_CONTEXT_ROOT_1 )
      ,
      g::c2s::test::C2STestRestResponse::
      build( g::c2s::OK )

      ) )
    );
  //check response of resource context root (leading '/')
  requests->add( BOOST_TEST_CASE( boost::bind( &checkResponse ,

      g::c2s::test::C2STestRestRequest::
      build( g::c2s::GET , "/" + RESOURCE_CONTEXT_ROOT_1 )
      ,
      g::c2s::test::C2STestRestResponse::
      build( g::c2s::OK )

      ) )
    );
  //check response of resource context root (POST method (not allowed))
  requests->add( BOOST_TEST_CASE( boost::bind( &checkResponse ,

      g::c2s::test::C2STestRestRequest::
      build( g::c2s::POST , "/" + RESOURCE_CONTEXT_ROOT_1 )
      ,
      g::c2s::test::C2STestRestResponse::
      build( g::c2s::MethodNotAllowed )

      ) )
    );
  //check response of resource context root (PUT method (not allowed))
  requests->add( BOOST_TEST_CASE( boost::bind( &checkResponse ,

      g::c2s::test::C2STestRestRequest::
      build( g::c2s::PUT , "/" + RESOURCE_CONTEXT_ROOT_1 )
      ,
      g::c2s::test::C2STestRestResponse::
      build( g::c2s::MethodNotAllowed )

      ) )
    );
  //check response of resource context root (DELETE method (not allowed))
  requests->add( BOOST_TEST_CASE( boost::bind( &checkResponse ,

      g::c2s::test::C2STestRestRequest::
      build( g::c2s::DELETE , "/" + RESOURCE_CONTEXT_ROOT_1 )
      ,
      g::c2s::test::C2STestRestResponse::
      build( g::c2s::MethodNotAllowed )

      ) )
    );

  //check response of resource that cannot be found
  requests->add( BOOST_TEST_CASE( boost::bind( &checkResponse ,

      g::c2s::test::C2STestRestRequest::
      build( g::c2s::GET , "/" + RESOURCE_CONTEXT_ROOT_1 + "/not-a-method" )
      ,
      g::c2s::test::C2STestRestResponse::
      build( g::c2s::NotFound )

      ) )
    );
  //check response of resource that cannot be found due to forgotten '/'
  requests->add( BOOST_TEST_CASE( boost::bind( &checkResponse ,

      g::c2s::test::C2STestRestRequest::
      build( g::c2s::GET , "/" + RESOURCE_CONTEXT_ROOT_1 + g::c2s::test::C2STestRestMethodQueryFields::sPath )
      ,
      g::c2s::test::C2STestRestResponse::
      build( g::c2s::NotFound )

      ) )
    );

  //check encoding and decoding of http query fields (missing query parameter)
  requests->add( BOOST_TEST_CASE( boost::bind( &checkResponse ,

      g::c2s::test::C2STestRestRequest::
      build( g::c2s::GET , "/" + RESOURCE_CONTEXT_ROOT_1 + "/" + g::c2s::test::C2STestRestMethodQueryFields::sPath )
      ,
      g::c2s::test::C2STestRestResponse::
      build( g::c2s::BadRequest )

      ) )
    );
  //check encoding and decoding of http query fields
  requests->add( BOOST_TEST_CASE( boost::bind( &checkResponse ,

      g::c2s::test::C2STestRestRequest::
      build( g::c2s::GET , "/" + RESOURCE_CONTEXT_ROOT_1 + "/" + g::c2s::test::C2STestRestMethodQueryFields::sPath ).
      query_field( "test_uri" , g::c2s::test::C2STestRestMethodQueryFields::sQueryFieldValueExpected )
      ,
      g::c2s::test::C2STestRestResponse::
      build( g::c2s::OK )

      ) )
    );

  //check functionality of path parameters (no path parameters specified)
  requests->add( BOOST_TEST_CASE( boost::bind( &checkResponse ,

      g::c2s::test::C2STestRestRequest::
      build( g::c2s::GET , "/" + RESOURCE_CONTEXT_ROOT_1 + "/" + g::c2s::test::C2STestRestMethodAdd::sPath )
      ,
      g::c2s::test::C2STestRestResponse::
      build( g::c2s::NotFound )

      ) )
    );
  //check functionality of path parameters (one parameter too much specified)
  requests->add( BOOST_TEST_CASE( boost::bind( &checkResponse ,

      g::c2s::test::C2STestRestRequest::
      build( g::c2s::GET , "/" + RESOURCE_CONTEXT_ROOT_1 + "/" + g::c2s::test::C2STestRestMethodAdd::sPath + "/2/4/invalid" )
      ,
      g::c2s::test::C2STestRestResponse::
      build( g::c2s::NotFound )

      ) )
    );

  //check functionality of path parameters (check XML response entity)
  requests->add( BOOST_TEST_CASE( boost::bind( &checkResponse ,

      g::c2s::test::C2STestRestRequest::
      build( g::c2s::GET , "/" + RESOURCE_CONTEXT_ROOT_1 + "/" + g::c2s::test::C2STestRestMethodAdd::sPath + "/2/4" ).
      accept( g::c2s::C2SHttpMediaType::application__xml )
      ,
      g::c2s::test::C2STestRestResponse::build( g::c2s::OK ).
      entity( g::c2s::C2SHttpMediaType::application__xml , "<result>0</result>" )

      ) )
    );
  //check functionality of path parameters (check JSON response entity)
  requests->add( BOOST_TEST_CASE( boost::bind( &checkResponse ,

      g::c2s::test::C2STestRestRequest::
      build( g::c2s::GET , "/" + RESOURCE_CONTEXT_ROOT_1 + "/" + g::c2s::test::C2STestRestMethodAdd::sPath + "/2/4" ).
      accept( g::c2s::C2SHttpMediaType::application__json )
      ,
      g::c2s::test::C2STestRestResponse::build( g::c2s::OK ).
      entity( g::c2s::C2SHttpMediaType::application__json , "{ \"result\" : 0 }" )

      ) )
    );
  //check functionality of path parameters (check XML response entity as default)
  requests->add( BOOST_TEST_CASE( boost::bind( &checkResponse ,

      g::c2s::test::C2STestRestRequest::
      build( g::c2s::GET , "/" + RESOURCE_CONTEXT_ROOT_1 + "/" + g::c2s::test::C2STestRestMethodAdd::sPath + "/2/4" ).
      accept( g::c2s::C2SHttpMediaType::wildcard )
      ,
      g::c2s::test::C2STestRestResponse::build( g::c2s::OK ).
      entity( g::c2s::C2SHttpMediaType::application__xml , "<result>0</result>" )

      ) )
    );
  //check functionality of path parameters (no content type defined)
  requests->add( BOOST_TEST_CASE( boost::bind( &checkResponse ,

      g::c2s::test::C2STestRestRequest::
      build( g::c2s::GET , "/" + RESOURCE_CONTEXT_ROOT_1 + "/" + g::c2s::test::C2STestRestMethodAdd::sPath + "/2/4" )
      ,
      g::c2s::test::C2STestRestResponse::build( g::c2s::NotAcceptable )

      ) )
    );
  //check functionality of path parameters (wrong content type defined)
  requests->add( BOOST_TEST_CASE( boost::bind( &checkResponse ,

      g::c2s::test::C2STestRestRequest::
      build( g::c2s::GET , "/" + RESOURCE_CONTEXT_ROOT_1 + "/" + g::c2s::test::C2STestRestMethodAdd::sPath + "/2/4" ).
      accept( g::c2s::C2SHttpMediaType::application__xhtml_xml )
      ,
      g::c2s::test::C2STestRestResponse::build( g::c2s::NotAcceptable )

      ) )
    );
  //check functionality of path parameters (wrong method type)
  requests->add( BOOST_TEST_CASE( boost::bind( &checkResponse ,

      g::c2s::test::C2STestRestRequest::
      build( g::c2s::POST , "/" + RESOURCE_CONTEXT_ROOT_1 + "/" + g::c2s::test::C2STestRestMethodAdd::sPath + "/2/4" ).
      accept( g::c2s::C2SHttpMediaType::application__xml )
      ,
      g::c2s::test::C2STestRestResponse::build( g::c2s::MethodNotAllowed )

      ) )
    );
  //check functionality of path parameters (wrong method type)
  requests->add( BOOST_TEST_CASE( boost::bind( &checkResponse ,

      g::c2s::test::C2STestRestRequest::
      build( g::c2s::PUT , "/" + RESOURCE_CONTEXT_ROOT_1 + "/" + g::c2s::test::C2STestRestMethodAdd::sPath + "/2/4" ).
      accept( g::c2s::C2SHttpMediaType::application__xml )
      ,
      g::c2s::test::C2STestRestResponse::build( g::c2s::MethodNotAllowed )

      ) )
    );
  //check functionality of path parameters (wrong method type)
  requests->add( BOOST_TEST_CASE( boost::bind( &checkResponse ,

      g::c2s::test::C2STestRestRequest::
      build( g::c2s::DELETE , "/" + RESOURCE_CONTEXT_ROOT_1 + "/" + g::c2s::test::C2STestRestMethodAdd::sPath + "/2/4" ).
      accept( g::c2s::C2SHttpMediaType::application__xml )
      ,
      g::c2s::test::C2STestRestResponse::build( g::c2s::MethodNotAllowed )

      ) )
    );
  //check functionality of path parameters and query field parameters
  requests->add( BOOST_TEST_CASE( boost::bind( &checkResponse ,

      g::c2s::test::C2STestRestRequest::
      build( g::c2s::GET , "/" + RESOURCE_CONTEXT_ROOT_1 + "/" + g::c2s::test::C2STestRestMethodAdd::sPath + "/2/4" ).
      accept( g::c2s::C2SHttpMediaType::application__xml ).
      query_field( "multiplier" , "2" )
      ,
      g::c2s::test::C2STestRestResponse::build( g::c2s::OK ).
      entity( g::c2s::C2SHttpMediaType::application__xml , "<result>12</result>" )

      ) )
    );
  //check functionality of path parameters and query field parameters
  requests->add( BOOST_TEST_CASE( boost::bind( &checkResponse ,

      g::c2s::test::C2STestRestRequest::
      build( g::c2s::GET , "/" + RESOURCE_CONTEXT_ROOT_1 + "/" + g::c2s::test::C2STestRestMethodAdd::sPath + "/2/4" ).
      accept( g::c2s::C2SHttpMediaType::application__json ).
      query_field( "multiplier" , "2" )
      ,
      g::c2s::test::C2STestRestResponse::build( g::c2s::OK ).
      entity( g::c2s::C2SHttpMediaType::application__json , "{ \"result\" : 12 }" )

      ) )
    );
  //check functionality of path parameters and query field parameters (string specified)
  //TODO: check '2k' (???)
  requests->add( BOOST_TEST_CASE( boost::bind( &checkResponse ,

      g::c2s::test::C2STestRestRequest::
      build( g::c2s::GET , "/" + RESOURCE_CONTEXT_ROOT_1 + "/" + g::c2s::test::C2STestRestMethodAdd::sPath + "/2/4" ).
      accept( g::c2s::C2SHttpMediaType::application__json ).
      query_field( "multiplier" , "k2" )
      ,
      g::c2s::test::C2STestRestResponse::build( g::c2s::BadRequest )

      ) )
    );
  //check functionality of path parameters and query field parameters (string specified)
  requests->add( BOOST_TEST_CASE( boost::bind( &checkResponse ,

      g::c2s::test::C2STestRestRequest::
      build( g::c2s::GET , "/" + RESOURCE_CONTEXT_ROOT_1 + "/" + g::c2s::test::C2STestRestMethodAdd::sPath + "/string/4" ).
      accept( g::c2s::C2SHttpMediaType::application__json ).
      query_field( "multiplier" , "2" )
      ,
      g::c2s::test::C2STestRestResponse::build( g::c2s::NotFound )

      ) )
    );

  //check second resource
  requests->add( BOOST_TEST_CASE( boost::bind( &checkResponse ,

      g::c2s::test::C2STestRestRequest::
      build( g::c2s::GET , RESOURCE_CONTEXT_ROOT_2 )
      ,
      g::c2s::test::C2STestRestResponse::
      build( g::c2s::OK )

      ) )
    );
  requests->add( BOOST_TEST_CASE( boost::bind( &checkResponse ,

      g::c2s::test::C2STestRestRequest::
      build( g::c2s::GET , "/" + RESOURCE_CONTEXT_ROOT_2 )
      ,
      g::c2s::test::C2STestRestResponse::
      build( g::c2s::OK )

      ) )
    );
  requests->add( BOOST_TEST_CASE( boost::bind( &checkResponse ,

      g::c2s::test::C2STestRestRequest::
      build( g::c2s::GET , "/" + RESOURCE_CONTEXT_ROOT_2 + "/not-a-method" )
      ,
      g::c2s::test::C2STestRestResponse::
      build( g::c2s::NotFound )

      ) )
    );
  requests->add( BOOST_TEST_CASE( boost::bind( &checkResponse ,

      g::c2s::test::C2STestRestRequest::
      build( g::c2s::GET , "/" + RESOURCE_CONTEXT_ROOT_2 + "not-a-method" )
      ,
      g::c2s::test::C2STestRestResponse::
      build( g::c2s::NotFound )

      ) )
    );

  test_suite* threading_test = BOOST_TEST_SUITE( "threading" );
  threading_test->add( BOOST_TEST_CASE( &checkThreading ) );
  framework::master_test_suite().add( threading_test );

  return 0;
}

std::list<g::c2s::C2SHttpResource*> createResources( g::thread::Mutex *pGlobalMutex )
{
  std::list<g::c2s::C2SHttpResource*> resources;

  g::c2s::C2SRestResource *pRestResource1 = g::c2s::C2SRestResource::createRestResource( RESOURCE_CONTEXT_ROOT_1 );
  pRestResource1->addMethod( new g::c2s::test::C2STestRestMethodAdd() );
  pRestResource1->addMethod( new g::c2s::test::C2STestRestMethodQueryFields() );
  pRestResource1->addMethod( new g::c2s::test::C2STestRestMethodThreading( pGlobalMutex ) );
  resources.push_back( pRestResource1 );

  resources.push_back( g::c2s::C2SRestResource::createRestResource( "/" + RESOURCE_CONTEXT_ROOT_2 ) );

  return resources;
}

class CheckThreading : public g::thread::TaskBase
{
public:

  CheckThreading( const g::c2s::test::C2STestRestRequest &request , const g::c2s::test::C2STestRestResponse &response_check )
    : m_request( request ),
      response_check( response_check )
  {}

  void run()
  {
    g::c2s::C2SHttpResponse response = m_request.process();
    g::thread::Lock lock( ForRestTestRestful::pGlobalMutex );
    response_check.check( response );
  }

  static CheckThreading *create( unsigned int iSleepMS )
  {
    return new CheckThreading(

        g::c2s::test::C2STestRestRequest::
        build( g::c2s::GET , "/" + RESOURCE_CONTEXT_ROOT_1 + "/" + g::c2s::test::C2STestRestMethodThreading::sPath + "/" + g::util::toString( iSleepMS ) )
        ,
        g::c2s::test::C2STestRestResponse::
        build( g::c2s::OK )

    );
  }

private:

  g::c2s::test::C2STestRestRequest m_request;

  g::c2s::test::C2STestRestResponse response_check;

};

void checkThreading()
{
  unsigned int iNumRequests = 1000;
  unsigned int iNumThreads = 5;
  unsigned int iSleepMS = 5;

  g::thread::TaskQueue tq( iNumThreads );
  std::list<CheckThreading*> checks;

  for ( unsigned int i = 0; i < iNumRequests; ++i )
    checks.push_back( CheckThreading::create( iSleepMS ) );

  std::list<CheckThreading*>::iterator it = checks.begin();
  for ( ; it != checks.end(); ++it )
    tq.queue( *it );

  tq.join();

  it = checks.begin();
  for ( ; it != checks.end(); ++it )
    delete *it;
}
