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

#include "C2SHttpMediaTypeList.h"

namespace g
{
  namespace c2s
  {

    bool C2SHttpMediaTypeList::exists( const std::string &type ) const
    {
      return m_typesByType.find( type ) != m_typesByType.end();
    }

    void C2SHttpMediaTypeList::add( const C2SHttpMediaType &type )
    {
      m_typesByType[ type.Type ] = type;
    }

    const C2SHttpMediaType &C2SHttpMediaTypeList::get( const std::string &type ) const
    {
      std::map<std::string,C2SHttpMediaType>::const_iterator it = m_typesByType.find( type );
      if ( it == m_typesByType.end() )
        throw C2SHttpException( "C2SHttpMediaTypeList::get:" , "Cannot find media type '" + type + "'" , InternalServerError );

      return it->second;
    }

    C2SHttpMediaTypeList C2SHttpMediaTypeList::detect( const char *data , unsigned int size )
    {
      C2SHttpMediaTypeList list;
      splitNhandle( data , size , ',' , &list );
      return list;
    }

    void C2SHttpMediaTypeList::operator()( const char *data , unsigned int size )
    {
      C2SHttpMediaType type;
      splitNhandle( data , size , ';' , &type );
      m_typesByQuality.insert( type );
      m_typesByType.insert( std::make_pair( type.Type , type ) );
    }


  }
}
