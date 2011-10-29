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

#include "C2SHttpResponse.h"

#include <cassert>

namespace c2s
{

  C2SHttpResponse::C2SHttpResponse( const C2SHttpResponseHeader &header )
    : m_header( header ),
      m_pBody( 0 )
  {
  }

  C2SHttpResponse::C2SHttpResponse()
    : m_pBody( 0 )
  {};

  C2SHttpResponse::C2SHttpResponse( const C2SHttpResponse &response )
    : m_parser( response.m_parser ),
      m_header( response.m_header ),
      m_pBody( 0 )
  {
    if ( m_header.Fields.getContentLength() )
    {
      if ( response.m_pBody )
      {
        assert( m_header.Fields.getContentLength() == response.m_pBody->size );

        //copy content
        char *data = new char[ response.m_pBody->size ];
        memcpy( data , response.m_pBody->data , response.m_pBody->size );
        m_pBody = new C2SHttpEntity( data , response.m_pBody->size , true );
      }
      else
        m_pBody = new C2SHttpEntity( m_parser.entity( m_header.Fields.getContentLength() ) , m_header.Fields.getContentLength() );
    }
  }

  C2SHttpResponse::~C2SHttpResponse()
  {
    delete m_pBody;
  }

  C2SHttpResponse &C2SHttpResponse::operator=( const C2SHttpResponse &response )
  {
    if ( this != &response )
    {
      if ( m_pBody )
        delete m_pBody;
      m_pBody = 0;

      m_header = response.m_header;
      m_parser = response.m_parser;

      if ( m_header.Fields.getContentLength() )
        m_pBody = new C2SHttpEntity( m_parser.entity( m_header.Fields.getContentLength() ) , m_header.Fields.getContentLength() );
    }
    return *this;
  }

  void C2SHttpResponse::push( char *data , unsigned int size )
  {
    m_parser.parse( data , size , &m_header );
  }

//    void C2SHttpResponse::setEntity( const char *data , unsigned int size )
//    {
//      delete m_pBody;
//      m_pBody = new C2SHttpEntity( data , size );
//    }

  void C2SHttpResponse::setEntity( C2SHttpEntity *pEntity )
  {
    delete m_pBody;
    m_pBody = pEntity;
    m_header.Fields.setContentLength( m_pBody->size );
  }

  C2SHttpResponse *C2SHttpResponse::build( C2SHttpStatus status )
  {
    return new C2SHttpResponse ( C2SHttpResponseHeader( status ) );
  }
  void C2SHttpResponse::finished()
  {
    delete m_pBody;
    m_pBody = NULL;
    if ( m_header.Fields.getContentLength() )
      m_pBody = new C2SHttpEntity( m_parser.entity( m_header.Fields.getContentLength() ) , m_header.Fields.getContentLength() );
  }

}
