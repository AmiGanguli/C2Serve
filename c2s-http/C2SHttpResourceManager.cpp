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
#include "C2SHttpResponse.h"

#include "C2SMatching.h"
#include "Logger.h"
#include "StringUtils.h"

#include <algorithm>
#include <iostream>
#include <cassert>

#define DEFAULT_DOCUMENTS "default/"

#define SEND_DATA_AT_ONCE

namespace g
{

  namespace c2s
  {

    void release( const std::pair<std::string,C2SHttpResource*> &p )
    {
      delete p.second;
    }

    void C2SHttpResourceManagerResponseHandler::sendResponse( const C2SHttpResponse &response )
    {
#ifdef SEND_DATA_AT_ONCE

      std::string sHeader = response.header().toString();
      const C2SHttpEntity *pBody = response.getEntity();
      if ( !pBody )
        m_pDataPush->push( sHeader.c_str() , sHeader.size() );
      else
      {
        //append body
        unsigned int iTotalSize = sHeader.size() + pBody->size;
        char *data = new char[ iTotalSize ];
        char *curpos = data;
        memcpy( curpos , sHeader.c_str() , sHeader.size() );
        curpos += sHeader.size();
        memcpy( curpos , pBody->data , pBody->size );
        m_pDataPush->push( data , iTotalSize );
        delete[] data;
      }

#else

      //TODO: why doesn't this work???
      std::string sHeader = response.header().toString();
      m_pDataPush->handle( sHeader.c_str() , sHeader.size() );

      const C2SHttpBody *pBody = response.getEntity();
      if ( pBody )
        m_pDataPush->handle( pBody->data , pBody->size );

#endif
    }

    C2SHttpResourceManager::C2SHttpResourceManager( C2SDataPushInterface *pDataPush )
      : m_responseHandler( pDataPush )
    {
      //clone registered resources
      ResourceContainer &Resources = C2SHttpResourceManager::getResourcePrototypes();
      const_iterator it  = Resources.begin();
      for ( ; it != Resources.end(); ++it )
      {
        C2SHttpResource *pResource = it->second->clone();
        pResource->setResponseHandler( &m_responseHandler );
        m_resources.insert( std::make_pair( it->first , pResource ) );
      }
    }

    C2SHttpResourceManager::~C2SHttpResourceManager()
    {
      std::for_each(  m_resources.begin() , m_resources.end() , release );
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
          iterator it = m_resources.find( "/" );
          if ( it == m_resources.end() )
            throw C2SHttpException( "C2SHttpResourceManager::handleRequest: " , "No root resource available" , NotFound );

          //redirect URI to index.html
          C2SHttpRequestHeader header( request.header() );
          header.URI = "index.html";
          C2SHttpRequest redirected( header );
          it->second->process( redirected );
        }
        else
        {
          C2SHttpResource *pBestMatch = this->bestMatch( request );
          if ( !pBestMatch )
            throw C2SHttpException( "C2SHttpResourceManager::handleRequest:" , "Not found: '" + request.header().URI + "'" , NotFound );

          pBestMatch->process( request );
        }
      }
      catch ( const C2SHttpException &e )
      {
#ifdef DEBUG
        std::cout << "Resource: " << request.header().URI << "; Status: " << e.status << " " << e.phrase << std::endl;
#endif
        C2SHttpResponseHeader header( e.status , e.phrase );
        C2SHttpResponse response( header );
        m_responseHandler.sendResponse( response );
      }
    }

    C2SHttpResource *C2SHttpResourceManager::bestMatch( const C2SHttpRequest &request )
    {
      if ( !m_resources.size() )
        return NULL;

      //find resource with longest match string
      C2SHttpResource *pBestMatch = findLongestMatchString( request.header().URI , m_resources );

      if ( pBestMatch ) //return resource with longest match string
        return pBestMatch;

      //we found no match and will try to handle the request with the root resource
      iterator it = m_resources.find( "/" );
      if ( it == m_resources.end() )
        throw C2SHttpException( "C2SHttpResourceManager::bestMatch: " , "No root resource available" , NotFound );

      return it->second;
    }

    void C2SHttpResourceManager::registerResource( C2SHttpResource *pResource )
    {
      ResourceContainer &Resources = C2SHttpResourceManager::getResourcePrototypes();
      if ( Resources.find( pResource->getContextRoot() ) != Resources.end() )
        throw C2SHttpException( "C2SHttpResourceManager::registerResource:" , "Duplicate context root: '" + pResource->getContextRoot() + "'" , InternalServerError );

      Resources.insert( std::make_pair( pResource->getContextRoot() , pResource ) );
    }

    void C2SHttpResourceManager::releaseResources()
    {
      ResourceContainer &Resources = C2SHttpResourceManager::getResourcePrototypes();
      std::for_each(  Resources.begin() , Resources.end() , release );
      Resources.clear();
    }

    C2SHttpResourceManager::ResourceContainer &C2SHttpResourceManager::getResourcePrototypes()
    {
      //TODO: use prototype pattern
      static ResourceContainer Resources;
      return Resources;
//      static ResrouceContainerStatus status;
//      static ResourceContainer *pResources;
//      if ( status.bIsCreated )
//        return *pResources;
//
//      pResources = new ResourceContainer();
//      status.bIsCreated = true;
//      return *pResources;
    }

  }

}
