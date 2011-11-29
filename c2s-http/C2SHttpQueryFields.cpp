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

#include "C2SHttpQueryFields.h"

#include "C2SHttpException.h"

#include "StringUtils.h"

#include <iostream>

namespace c2s
{

  C2SHttpQueryFields::C2SHttpQueryFields()
  {
  }

  C2SHttpQueryFields::~C2SHttpQueryFields()
  {
  }

  void C2SHttpQueryFields::addField( const std::string &sFieldName , const std::string &sFieldValue )
  {
    if ( m_fields.find( sFieldName ) != m_fields.end() )
      throw C2SHttpException( "C2SHttpQueryFields::add: " , "Duplicate field name: " + sFieldName , BadRequest );

    m_fields[ sFieldName ] = sFieldValue;
  }

  const std::string &C2SHttpQueryFields::getValueForFieldName( const std::string &sFieldName ) const
  {
    const_iterator it = m_fields.find( sFieldName );
    if ( it == m_fields.end() )
      throw C2SHttpException( "C2SHttpQueryFields::get: " , "Cannot find query field: " + sFieldName , InternalServerError );

    return it->second;
  }

  std::ostream &operator<<( std::ostream &os , const C2SHttpQueryFields &fields )
  {
    if ( !fields.size() )
      return os;

    os << "?";

    C2SHttpQueryFields::const_iterator it = fields.begin();
    for ( unsigned int iCurrentFieldToStream = 1; it != fields.end(); ++it, ++iCurrentFieldToStream )
    {
      os << it->first << "=" << c2s::util::urlEncode( it->second );
      if ( iCurrentFieldToStream < fields.size() )
        os << "&";
    }

    return os;
  }

}
