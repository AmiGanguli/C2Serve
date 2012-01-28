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

#include "C2SHttpParseResource.h"

#include "C2SHttpDefines.h"
#include "C2SHttpException.h"
#include "C2SHttpRequestHeader.h"
#include "C2SStringUtils.h"
#include "StringUtils.h"

namespace c2s
{

  C2SHttpParseResource::C2SHttpParseResource( C2SHttpRequestHeader *pHeader )
    : m_pHeader( pHeader ),
      m_uriParser( pHeader ),
      m_iArgIdx( 0 )
  {};

  void C2SHttpParseResource::operator()( const char *data , unsigned int size )
  {
    if ( !m_iArgIdx )
      splitNhandle( data , size , '?' , &m_uriParser );
    else if ( m_iArgIdx == 1 )
    {
      unsigned int n = sHttpVersionPrefix.size();
      if ( n > size )
        throw C2SHttpException( "C2SHttpResourceParser::operator():" , "Cannot parse version string: '" + std::string( data , size ) + "'!" , BadRequest );

      data += n;
      m_pHeader->Version = c2s::util::toNumber<float>( std::string( data , size - n ) );
    }

    ++m_iArgIdx;
  }
}
