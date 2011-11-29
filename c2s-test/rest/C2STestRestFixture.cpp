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

#include "C2STestRestFixture.h"
#include "C2STestServerRunner.h"

#include "C2STestRestMethodAdd.h"
#include "C2STestRestMethodThreading.h"
#include "C2STestRestMethodQueryFields.h"
#include "C2STestRestMethodInvalidEntity.h"
#include "C2STestRestMethodMediaTypeConverter.h"

#include "C2SRestResourcePrototype.h"

namespace c2s
{

  namespace test
  {

    c2s::thread::Mutex *C2STestRestFixture::pGlobalMutex = NULL;

    const unsigned int C2STestRestFixture::iPortOfTestServer = 8183;

    const std::string C2STestRestFixture::sContextRootOfTestResource = "g/forrest/test/rest-check/resource-1";
    const std::string C2STestRestFixture::sContextRootOfEmptyResource = "g/forrest/test/rest-check/resource-2";

    C2STestRestFixture::C2STestRestFixture() : m_sr( NULL )
    {
      if ( !pGlobalMutex )
        pGlobalMutex = new c2s::thread::Mutex();

      m_sr = new c2s::test::C2STestServerRunner( C2STestRestFixture::createResources( pGlobalMutex ) , iPortOfTestServer );
    };

    C2STestRestFixture::~C2STestRestFixture()
    {
      delete m_sr;
      delete pGlobalMutex;
    }

    std::list<c2s::C2SHttpResourcePrototype*> C2STestRestFixture::createResources( c2s::thread::Mutex *pGlobalMutex )
    {
      std::list<c2s::C2SHttpResourcePrototype*> resources;

      c2s::C2SRestResourcePrototype *pRestResource1 = c2s::C2SRestResourcePrototype::createRestResourceWithContextRoot( sContextRootOfTestResource );
      pRestResource1->registerMethodPrototype( new c2s::test::C2STestRestMethodAdd() );
      pRestResource1->registerMethodPrototype( new c2s::test::C2STestRestMethodQueryFields() );
      pRestResource1->registerMethodPrototype( new c2s::test::C2STestRestMethodMediaTypeConverter() );
      pRestResource1->registerMethodPrototype( new c2s::test::C2STestRestMethodInvalidEntity() );
      pRestResource1->registerMethodPrototype( new c2s::test::C2STestRestMethodThreading( pGlobalMutex ) );
      resources.push_back( pRestResource1 );

      resources.push_back( c2s::C2SRestResourcePrototype::createRestResourceWithContextRoot( "/" + sContextRootOfEmptyResource ) );

      return resources;
    }

  }

}
