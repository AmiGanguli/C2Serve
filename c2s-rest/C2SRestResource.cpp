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

#include "C2SRestResource.h"

#include "C2SMatching.h"
#include "C2SHttpRequest.h"
#include "C2SStringUtils.h"
#include "C2SGlobalSettings.h"

#include "C2SRestException.h"

#include "StringUtils.h"

#include <iostream>

namespace c2s
{

  C2SRestResource::C2SRestResource( const std::string &sHostName , const std::string &sContextRoot )
    : C2SHttpResource( sContextRoot ),
      m_description( sHostName , sContextRoot )
  {
  }

  C2SRestResource::C2SRestResource( const C2SRestResource &r )
    : C2SHttpResource( r ),
      m_description( r.m_description )
  {
    Methods::const_iterator mit = r.m_methods.begin();
    for ( ; mit != r.m_methods.end(); ++mit )
    {
      C2SRestMethodPrototype *pPrototype = *mit;
      C2SRestMethodPrototype *pClone = pPrototype->clone();
      if ( pClone == pPrototype )
        throw C2SRestException( "C2SRestResource::C2SRestResource: " , "C2SRestMethodPrototype::clone() is not allowed to return reference to itself!" , InternalServerError );

      m_methods.push_back( pClone );
    }
  };

  C2SRestResource::~C2SRestResource()
  {
    Methods::iterator mit = m_methods.begin();
    for ( ; mit != m_methods.end(); ++mit )
      delete ( *mit );
  }

  C2SRestResource *C2SRestResource::createRestResource( const std::string &sContextRoot )
  {
    C2SRestResource *pResource = new C2SRestResource( C2SGlobalSettings::Settings().C2SHostName , sContextRoot );
    return pResource;
  }

  void C2SRestResource::addMethod( C2SRestMethodPrototype *pMethod )
  {
    if ( this->existsMethod( pMethod ) )
      throw C2SRestException( "C2SRestResource::addMethod: " , "Duplicate REST method" , InternalServerError );

    m_methods.push_back( pMethod );
    m_methodsByPathGET.insert( std::make_pair( pMethod->getPath() , pMethod ) );
  }

  bool C2SRestResource::existsMethod( const C2SRestMethodPrototype * ) const
  {
    //TODO: implement!!
    std::cout << "C2SRestResource::existsMethod: Not yet implemented" << std::endl;
    return false;
  }

  void C2SRestResource::process( const C2SHttpRequest &request )
  {
    const std::string &sResource = request.header().URI;

    //TODO: is it correct to ignore leading slash when comparing URI strings??
    if ( uriEquals( sResource.c_str() , sResource.size() , m_sContextRoot.c_str() , m_sContextRoot.size() , false ) )
    {
      if ( request.header().Method != GET )
        throw C2SRestException( "C2SRestResource::process: " , "Access to context root of RESTful resource only allowed as GET request" , MethodNotAllowed );

      C2SHttpResponse response = m_description.process( request );
      m_pResponseHandler->sendResponse( response );
      return;
      //throw C2SRestException( "C2SRestResource::process: " , "Access to context root of RESTful resource not allowed (yet)" , Forbidden );
    }

    C2SRestMethodPrototype *pMethod = this->getBestMatch( request );

    if ( !pMethod )
      throw C2SRestException( "C2SRestResource::process: " , "Resource not found" , NotFound );

    if ( pMethod )
    {
      C2SHttpResponse *pResponse = pMethod->process( request );
      m_pResponseHandler->sendResponse( *pResponse );
      delete pResponse;
    }
  }

  C2SRestMethodPrototype *C2SRestResource::getBestMatch( const C2SHttpRequest &request )
  {
    //TODO: implement

    if ( !m_methods.size() )
      return NULL;

    const std::string &sURIComplete = request.header().URI;
    int iSkipIdx = c2s::uriSkip( sURIComplete.c_str() , sURIComplete.size() , m_sContextRoot.c_str() , m_sContextRoot.size() , false );

    if ( iSkipIdx < 0 )
      throw C2SRestException( "C2SRestResource::getBestMatch: " , "Cannot parse resource URI: " + sURIComplete , InternalServerError );

    C2SRestPathIDList pathList = c2s::util::splitString( sURIComplete.substr( iSkipIdx , sURIComplete.size() ) , '/' , true );

    std::map<int,C2SRestMethodPrototype*> candidates;
    Methods::iterator mit = m_methods.begin();
    for ( ; mit != m_methods.end(); ++mit )
    {
      C2SRestMethodPrototype *pMethod = *mit;
      int iDistance = pMethod->getDistance( pathList );

      if ( iDistance < 0 ) //mismatch
        continue;

      //this must be prevented when adding methods to a resource
      if ( candidates.find( iDistance ) != candidates.end() )
        throw C2SRestException( "C2SRestResource::getBestMatch: " , "Detected duplicate path distance, two or more methods match this call: " + request.header().URI , InternalServerError );

      candidates[ iDistance ] = pMethod;
    }

    if ( !candidates.size() )
      return NULL; //we didn't find anything

    //return candidate with smallest distance

    //check method types
    std::map<int,C2SRestMethodPrototype*>::iterator cit = candidates.begin();
    std::map<int,C2SRestMethodPrototype*>::iterator cend = candidates.end();
    for ( ; cit != cend; ++cit )
    {
      C2SRestMethodPrototype *pMatch = cit->second;
      if ( pMatch->getMethod() != request.header().Method )
        continue;

      pMatch->handle( pathList );
      return pMatch;
    }

    throw C2SRestException( "C2SRestResource::getBestMatch: " , "Method not allowed" , MethodNotAllowed );
  }

  C2SHttpResource *C2SRestResource::clone() const
  {
    return new C2SRestResource( *this );
  }

}
