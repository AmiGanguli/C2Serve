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

#include "C2SHttpHeaderFields.h"

#include "C2SHttpException.h"

#include "StringUtils.h"

#define FIELD_ACCEPT "Accept"
#define FIELD_CONTENT_TYPE "Content-Type"
#define FIELD_CONTENT_LENGTH "Content-Length"

namespace c2s
{

  C2SHttpHeaderFields::C2SHttpHeaderFields()
    : m_iContentLength( 0 )
  {
  }

  C2SHttpHeaderFields::~C2SHttpHeaderFields()
  {
  }


  void C2SHttpHeaderFields::set( const std::string &sFieldName , const std::string &sFieldValue )
  {
    //TODO: What if this error occurs when creating the response header?? Which status code?
//      if ( m_fields.find( sFieldName ) != m_fields.end() )
//        throw C2SHttpException( "C2SHttpHeaderFields::addField: " , "Duplicate header field: " + sFieldName , BadRequest );

    m_fields[ sFieldName ] = sFieldValue;

    if ( sFieldName == FIELD_ACCEPT )
      m_accept = C2SHttpMediaTypeList::detect( sFieldValue.c_str() , sFieldValue.size() );
    else if ( sFieldName == FIELD_CONTENT_TYPE )
    {
      splitNhandle( sFieldValue.c_str() , sFieldValue.size() , ';' , &m_contentType );
    }
    else if ( sFieldName == FIELD_CONTENT_LENGTH )
      m_iContentLength = c2s::util::toNumber<unsigned int>( sFieldValue );
  }

  bool C2SHttpHeaderFields::isAccept( const std::string &sMediaType ) const
  {
    return m_accept.exists( sMediaType );
  }

  void C2SHttpHeaderFields::addAccept( const C2SHttpMediaType &mediaType )
  {
    m_accept.add( mediaType );
    std::string &sAcceptValue = m_fields[ FIELD_ACCEPT ];
    if ( sAcceptValue.size() )
      sAcceptValue += ",";

    sAcceptValue += mediaType.toString();
  }

  const C2SHttpMediaType &C2SHttpHeaderFields::getAccept( const std::string &sMediaType ) const
  {
    return m_accept.get( sMediaType );
  }

  void C2SHttpHeaderFields::setContentType( const C2SHttpMediaType &mediatype )
  {
    m_fields[ FIELD_CONTENT_TYPE ] = mediatype.Type;
    m_contentType = mediatype;
  }

  void C2SHttpHeaderFields::setContentLength( unsigned int iContentLength )
  {
    m_iContentLength = iContentLength;
    m_fields[ FIELD_CONTENT_LENGTH ] = c2s::util::toString( iContentLength );
  }

}
