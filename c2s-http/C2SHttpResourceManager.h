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

#ifndef C2SHTTPRESOURCEMANAGER_H_
#define C2SHTTPRESOURCEMANAGER_H_

#include "C2SHttpRequest.h"
#include "C2SHttpResourcePrototype.h"

#include "C2SDataPullInterface.h"

#include <map>

namespace io
{
  class Logger;
}

namespace c2s
{

  class C2SHttpResourceManagerResponseHandler : public C2SHttpResponseHandlerInterface
  {
  public:

    C2SHttpResourceManagerResponseHandler( C2SDataPushInterface *pDataPush )
      : m_pDataPush( pDataPush )
    {};

    virtual void sendResponse( const C2SHttpResponse &response );

  private:

    C2SDataPushInterface *m_pDataPush;

  };

  class C2SHttpResourceManager
  {
  public:

    C2SHttpResourceManager( C2SDataPushInterface *pDataCallback );

    virtual ~C2SHttpResourceManager();

    void handleRequest( const C2SHttpRequest &request );

    C2SHttpResourcePrototype *bestMatch( const C2SHttpRequest &request );

    static void registerResourcePrototype( C2SHttpResourcePrototype *pResource );

    static void releaseResourcePrototypes();

  private:

    C2SHttpResourceManager( const C2SHttpResourceManager & );

    C2SHttpResourceManager &operator=( const C2SHttpResourceManager & );

    typedef std::map<std::string,C2SHttpResourcePrototype*> ResourceContainer;
    typedef std::map<std::string,C2SHttpResourcePrototype*>::const_iterator const_iterator;
    typedef std::map<std::string,C2SHttpResourcePrototype*>::iterator iterator;

    static ResourceContainer &getResourcePrototypes();

    ResourceContainer m_resources;

    C2SHttpResourceManagerResponseHandler m_responseHandler;

  };

}

#endif /* C2SHTTPRESOURCEMANAGER_H_ */
