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

#include "C2SHttpParseQueryFieldAndAddToList.h"
#include "C2SHttpQueryFields.h"
#include "C2SHttpException.h"

#include "StringUtils.h"

namespace c2s
{

  C2SHttpParseQueryFieldAndAddToList::C2SHttpParseQueryFieldAndAddToList( C2SHttpQueryFields *pQueryFields )
    : m_pQueryFields( pQueryFields ),
      m_currData( NULL ),
      m_currSize( 0 ),
      m_bIsNewField( false )
  {};

  void C2SHttpParseQueryFieldAndAddToList::setNewField()
  {
    m_currData = NULL;
    m_currSize = 0;
    m_bIsNewField = true;
  }

  void C2SHttpParseQueryFieldAndAddToList::operator()( const char *data , unsigned int size )
  {
    if ( !m_pQueryFields )
      throw C2SHttpException( "C2SHttpQueryFieldSetter::operator():" , "Query fields are NULL", InternalServerError );

    if ( m_bIsNewField )
    {
      m_currData = data;
      m_currSize = size;
    }
    else
    {
      if ( !m_currSize )
        throw C2SHttpException( "C2SHttpQueryFieldSetter::operator():" , "Cannot parse URI string, detected empty query field name", BadRequest );

      m_pQueryFields->addField( std::string( m_currData , m_currSize ) , c2s::util::urlDecode( std::string( data , size ) ) );
    }

    m_bIsNewField = false;
  }

}
