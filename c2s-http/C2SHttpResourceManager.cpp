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

#include "C2SHttpResourceManager.h"
#include "C2SHttpResourcePrototypeList.h"
#include "C2SHttpResponse.h"
#include "C2SHttpRequest.h"
#include "C2SHttpResourcePrototype.h"
#include "C2SHttpResourceManagerResponseHandler.h"

#include "C2SMatching.h"
#include "Logger.h"
#include "StringUtils.h"

#include <algorithm>
#include <iostream>
#include <cassert>

#define DEFAULT_DOCUMENTS "default/"

namespace c2s
{

  C2SHttpResourceManager::C2SHttpResourceManager( C2SDataPushInterface *pDataPush , const C2SHttpResourcePrototypeList &listOfResourcePrototypes )
    : m_pClonesOfResourcePrototypesList( new C2SHttpResourcePrototypeList( listOfResourcePrototypes ) ),
      m_pResponseHandler( new C2SHttpResourceManagerResponseHandler( pDataPush ) )
  {
    C2SHttpResourcePrototypeList::iterator it = m_pClonesOfResourcePrototypesList->begin();
    C2SHttpResourcePrototypeList::iterator end = m_pClonesOfResourcePrototypesList->end();
    for ( ; it != end; ++it )
      it->second->setResponseHandler( m_pResponseHandler );
  }

  C2SHttpResourceManager::~C2SHttpResourceManager()
  {
    delete m_pResponseHandler;
    m_pClonesOfResourcePrototypesList->deleteResourcePrototypesAndClearList();
    delete m_pClonesOfResourcePrototypesList;
  }

  void C2SHttpResourceManager::handleRequest( const C2SHttpRequest &request )
  {
    try
    {
      if ( !request.header().URI.size() )
        throw C2SHttpException( "C2SHttpResourceManager::bestMatch:" , "No URI specified" , BadRequest );

      if ( request.header().URI == "/" && request.header().Method == GET )
      {
        //get root resource
        C2SHttpResourcePrototypeList::iterator it = m_pClonesOfResourcePrototypesList->find( "/" );
        if ( it == m_pClonesOfResourcePrototypesList->end() )
          throw C2SHttpException( "C2SHttpResourceManager::handleRequest: " , "No root resource available" , NotFound );

        //redirect URI to index.html
        C2SHttpRequestHeader header( request.header() );
        header.URI = "index.html";
        C2SHttpRequest redirected( header );
        it->second->processRequest( redirected );
      }
      else
      {
        C2SHttpResourcePrototype *pBestMatch = this->bestMatch( request );
        if ( !pBestMatch )
          throw C2SHttpException( "C2SHttpResourceManager::handleRequest:" , "Not found: '" + request.header().URI + "'" , NotFound );

        pBestMatch->processRequest( request );
      }
    }
    catch ( const C2SHttpException &e )
    {
#ifdef DEBUG
      std::cout << "Resource: " << request.header().URI << "; Status: " << e.status << " " << e.phrase << std::endl;
#endif
      C2SHttpResponseHeader header( e.status , e.phrase );
      C2SHttpResponse response( header );
      m_pResponseHandler->sendResponse( response );
    }
  }

  C2SHttpResourcePrototype *C2SHttpResourceManager::bestMatch( const C2SHttpRequest &request )
  {
    if ( !m_pClonesOfResourcePrototypesList->size() )
      return NULL;

    //find resource with longest match string
    C2SHttpResourcePrototype *pBestMatch = findLongestMatchString( request.header().URI , *m_pClonesOfResourcePrototypesList );

    if ( pBestMatch ) //return resource with longest match string
      return pBestMatch;

    //we found no match and will try to handle the request with the root resource
    C2SHttpResourcePrototypeList::iterator it = m_pClonesOfResourcePrototypesList->find( "/" );
    if ( it == m_pClonesOfResourcePrototypesList->end() )
      throw C2SHttpException( "C2SHttpResourceManager::bestMatch: " , "No root resource available" , NotFound );

    return it->second;
  }

}
