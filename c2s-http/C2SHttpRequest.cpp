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

#include "C2SHttpRequest.h"

namespace c2s
{

  C2SHttpRequest::C2SHttpRequest()
    : m_pEntity( NULL )
  {
  }

  C2SHttpRequest::C2SHttpRequest( const C2SHttpRequestHeader &header )
  : m_header( header ),
    m_pEntity( NULL )
  {
  }

  C2SHttpRequest::~C2SHttpRequest()
  {
    delete m_pEntity;
  }

  void C2SHttpRequest::push( char *data , unsigned int size )
  {
    m_parser.parse( data , size , &m_header );
  }

  void C2SHttpRequest::finished()
  {
    if ( m_header.Fields.getContentLength() )
      m_pEntity = new C2SHttpEntity( m_parser.entity( m_header.Fields.getContentLength() ) , m_header.Fields.getContentLength() );
  }

  const C2SHttpEntity &C2SHttpRequest::entity() const
  {
    if ( !m_pEntity )
      throw C2SHttpException( "C2SHttpRequest::entity: " , "No entity available!" , InternalServerError );

    return *m_pEntity;
  }

  void C2SHttpRequest::setEntity( C2SHttpEntity *pHttpEntity )
  {
    if ( m_pEntity )
      delete m_pEntity;
    m_pEntity = pHttpEntity;
  }

}
