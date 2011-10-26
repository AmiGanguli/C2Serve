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

#ifndef C2STESTRESTMETHODS_H_
#define C2STESTRESTMETHODS_H_

#include <boost/test/unit_test.hpp>

#include "C2SRestEntityMethodPrototype.h"

#include "Lock.h"
#include "StringUtils.h"

#ifdef WINXX
#include <windows.h>
#endif

#define PATH_ADDER std::string( "math/add" )
#define PATH_THREADING std::string( "threading" )
#define PATH_QUERY_FIELDS std::string( "check-query-fields/" )

#define TEST_QUERY_FIELD std::string( "12arg:,478859'*##*" )

namespace g
{
  namespace c2s
  {
    namespace test
    {

      template <class Type>
      class C2STestRestEntityStreamerXML : public C2SRestEntityStreamer<Type>
      {
      public:

        C2STestRestEntityStreamerXML()
          : C2SRestEntityStreamer<Type>( C2SHttpMediaType::application__xml )
        {};


        C2SHttpEntity *entity( const Type &data ) const
        {
          std::string sXML = "<result>" + g::util::toString( data ) + "</result>";
          char *cdata = new char[ sXML.size() ];
          std::memcpy( cdata , sXML.c_str() , sXML.size() );
          return new C2SHttpEntity( cdata , sXML.size() , true );
        }

      private:

      };

      template <class Type>
      class C2STestRestEntityStreamerJSON : public C2SRestEntityStreamer<Type>
      {
      public:

        C2STestRestEntityStreamerJSON()
          : C2SRestEntityStreamer<Type>( C2SHttpMediaType::application__json )
        {};


        C2SHttpEntity *entity( const Type &data ) const
        {
          std::string sJSON = "{ \"result\" : " + g::util::toString( data ) + " }";
          char *cdata = new char[ sJSON.size() ];
          std::memcpy( cdata , sJSON.c_str() , sJSON.size() );
          return new C2SHttpEntity( cdata , sJSON.size() , true );
        }

      private:

      };

      class C2STestRestMethodAdder : public C2SRestEntityMethodPrototype<unsigned int>
      {
      public:

        C2STestRestMethodAdder()
          : C2SRestEntityMethodPrototype<unsigned int>( GET , PATH_ADDER ),
            m_iArg1( 0 ),
            m_iArg2( 0 ),
            m_iMultiplier( 0 )
        {
        	C2SRestEntityMethodPrototype<unsigned int>::addPathParameter( "arg1" , &m_iArg1 );
        	C2SRestEntityMethodPrototype<unsigned int>::addPathParameter( "arg2" , &m_iArg2 );
        	C2SRestEntityMethodPrototype<unsigned int>::addQueryParameter( "multiplier" , &m_iMultiplier , m_iMultiplier );
        	C2SRestEntityMethodPrototype<unsigned int>::addEntityStreamer( new C2STestRestEntityStreamerXML<unsigned int>() );
        	C2SRestEntityMethodPrototype<unsigned int>::addEntityStreamer( new C2STestRestEntityStreamerJSON<unsigned int>() );
        };

        C2SHttpResponse *process()
        {
          BOOST_MESSAGE( "C2STestRestMethodAdder::process: " << m_iArg1 << " + " << m_iArg2 << " * " << m_iMultiplier );
          unsigned int iResult = ( m_iArg1 + m_iArg2 ) * m_iMultiplier;
          return C2SRestEntityMethodPrototype<unsigned int>::buildResponse( OK , iResult );
        }

        C2STestRestMethodAdder *clone() const
        {
          return new C2STestRestMethodAdder();
        }

      private:

        unsigned int m_iArg1;

        unsigned int m_iArg2;

        unsigned int m_iMultiplier;

      };

      class C2STestRestMethodQueryFields : public C2SRestMethodPrototype
      {
      public:

        C2STestRestMethodQueryFields()
          : C2SRestMethodPrototype( GET , PATH_QUERY_FIELDS )
        {
          C2SRestMethodPrototype::addQueryParameter( "test_uri" , &m_sTestURI );
        };

        C2SHttpResponse *process()
        {
          BOOST_MESSAGE( "C2STestRestMethodQueryFields::process: \"" << m_sTestURI << "\"" );
          BOOST_CHECK( m_sTestURI == TEST_QUERY_FIELD );
          return C2SRestMethodPrototype::buildResponse( OK );
        }

        C2STestRestMethodQueryFields *clone() const
        {
          return new C2STestRestMethodQueryFields();
        }

      private:

        std::string m_sTestURI;

      };

      class C2STestRestMethodThreading : public C2SRestMethodPrototype
      {
      public:

        C2STestRestMethodThreading( g::thread::Mutex *pGlobalMutex )
          : C2SRestMethodPrototype( GET , PATH_THREADING ),
            m_bIsRunning( false ),
            m_iSleepMS( 0 ),
            m_globalMutex( *pGlobalMutex )
        {
          C2SRestMethodPrototype::addPathParameter( "sleep" , &( m_iSleepMS ) );
        };

        C2SHttpResponse *process()
        {
          m_globalMutex.lock();
          BOOST_MESSAGE( "ForRestTestRestfulMethodThreading::process: sleep: " << m_iSleepMS );
          BOOST_CHECK( !m_bIsRunning );
          m_bIsRunning = true;
          m_globalMutex.unlock();

#ifdef WINXX
          Sleep( m_iSleepMS );
#else
          usleep( m_iSleepMS * 1000 );
#endif

          m_bIsRunning = false;
          return C2SHttpResponse::build( OK );
        }

        C2STestRestMethodThreading *clone() const
        {
          return new C2STestRestMethodThreading( &m_globalMutex );
        }

      private:

        C2STestRestMethodThreading( const C2STestRestMethodThreading & );

        C2STestRestMethodThreading &operator=( const C2STestRestMethodThreading & );

        bool m_bIsRunning;

        int m_iSleepMS;

        g::thread::Mutex &m_globalMutex;

      };

    }
  }
}

#endif /* C2STESTRESTMETHODS_H_ */
