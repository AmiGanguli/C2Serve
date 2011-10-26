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

#ifndef C2SRESTRESOURCE_H_
#define C2SRESTRESOURCE_H_

#include "C2SHttpResource.h"

#include "C2SRestMethodPrototype.h"
#include "C2SRestResourceDescription.h"

#include <list>
#include <map>

namespace g
{

  namespace c2s
  {

    class C2SRestResource: public C2SHttpResource
    {
    public:

      virtual ~C2SRestResource();

      typedef std::list<C2SRestMethodPrototype*> Methods;

      static C2SRestResource *createRestResource( const std::string &sContextRoot );

      void process( const C2SHttpRequest &request );

      C2SHttpResource *clone() const;

      //registering method prototype
      void addMethod( C2SRestMethodPrototype *pMethod );

      //TODO: implement
      bool existsMethod( const C2SRestMethodPrototype *pMethod ) const;

    protected:

      C2SRestMethodPrototype *getBestMatch( const C2SHttpRequest &request );

      std::list<C2SRestMethodPrototype*> m_methods;

      std::map<std::string,C2SRestMethodPrototype*> m_methodsByPathGET;

      C2SRestResourceDescription m_description;

    private:

      C2SRestResource( const std::string &sHostName , const std::string &sContextRoot );

      C2SRestResource( const C2SRestResource &r );

    };

  }

}

#endif /* C2SRESTRESOURCE_H_ */
