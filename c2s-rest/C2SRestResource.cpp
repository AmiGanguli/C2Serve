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

#include "C2SRestException.h"
#include "C2SRestMatchMethodToRequest.h"

#include "C2SMatching.h"
#include "C2SHttpRequest.h"
#include "C2SStringUtils.h"
#include "C2SGlobalSettings.h"

#include "StringUtils.h"

#include <iostream>

namespace c2s
{

  C2SRestResource::C2SRestResource( const std::string &sHostName , const std::string &sContextRoot )
    : C2SHttpResource( sContextRoot ),
      m_resourceDescription( sHostName , sContextRoot )
  {
  }

  C2SRestResource::C2SRestResource( const C2SRestResource &r )
    : C2SHttpResource( r ),
      m_resourceDescription( r.m_resourceDescription )
  {
    C2SRestMethodPrototypeList::const_iterator mit = r.m_registeredMethodPrototypes.begin();
    for ( ; mit != r.m_registeredMethodPrototypes.end(); ++mit )
    {
      C2SRestMethodPrototype *pPrototype = *mit;
      C2SRestMethodPrototype *pClone = pPrototype->clone();
      if ( pClone == pPrototype )
        throw C2SRestException( "C2SRestResource::C2SRestResource: " , "C2SRestMethodPrototype::clone() is not allowed to return reference to itself!" , InternalServerError );

      m_registeredMethodPrototypes.push_back( pClone );
    }
  };

  C2SRestResource::~C2SRestResource()
  {
    C2SRestMethodPrototypeList::iterator mit = m_registeredMethodPrototypes.begin();
    for ( ; mit != m_registeredMethodPrototypes.end(); ++mit )
      delete ( *mit );
  }

  C2SRestResource *C2SRestResource::createRestResourceWithContextRoot( const std::string &sContextRootOfRestResource )
  {
    C2SRestResource *pResource = new C2SRestResource( C2SGlobalSettings::Settings().C2SHostName , sContextRootOfRestResource );
    return pResource;
  }

  void C2SRestResource::registerMethodPrototype( C2SRestMethodPrototype *pMethod )
  {
    if ( this->existsMethodPrototype( pMethod ) )
      throw C2SRestException( "C2SRestResource::registerMethodPrototype: " , "Duplicate REST method" , InternalServerError );

    m_registeredMethodPrototypes.push_back( pMethod );
  }

  bool C2SRestResource::existsMethodPrototype( const C2SRestMethodPrototype * ) const
  {
    //TODO: implement!!
    std::cout << "C2SRestResource::existsMethodPrototype: Not yet implemented" << std::endl;
    return false;
  }

  void C2SRestResource::processRequest( const C2SHttpRequest &request )
  {
    const std::string &sResource = request.header().URI;

    //TODO: is it correct to ignore leading slash when comparing URI strings??
    if ( uriEquals( sResource.c_str() , sResource.size() , m_sContextRoot.c_str() , m_sContextRoot.size() , false ) )
    {
      if ( request.header().Method != GET )
        throw C2SRestException( "C2SRestResource::processRequest: " , "Access to context root of RESTful resource only allowed as GET request" , MethodNotAllowed );

      C2SHttpResponse response = m_resourceDescription.process( request );
      m_pResponseHandler->sendResponse( response );
      return;
      //throw C2SRestException( "C2SRestResource::process: " , "Access to context root of RESTful resource not allowed (yet)" , Forbidden );
    }

    C2SRestMethodPrototype *pMethod = this->getPrototypeWithBestMatchForRequest( request );

    if ( !pMethod )
      throw C2SRestException( "C2SRestResource::processRequest: " , "Resource not found" , NotFound );

    if ( pMethod )
    {
      C2SHttpResponse *pResponse = pMethod->process( request );
      m_pResponseHandler->sendResponse( *pResponse );
      delete pResponse;
    }
  }

  C2SRestMethodPrototype *C2SRestResource::getPrototypeWithBestMatchForRequest( const C2SHttpRequest &requestToMatch )
  {
    C2SRestMatchMethodToRequest methodToRequestMatching( m_sContextRoot , m_registeredMethodPrototypes , requestToMatch );
    return methodToRequestMatching.getPrototypeWithBestMatchAndPrepareFromURI();
  }

  C2SHttpResource *C2SRestResource::clone() const
  {
    return new C2SRestResource( *this );
  }

}
