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

#include "C2SRestMethodPrototype.h"

#include "StringUtils.h"

namespace c2s
{

  C2SRestMethodPrototype::C2SRestMethodPrototype( C2SHttpMethod methodType , const std::string &sPath )
    : m_methodType( methodType )
  {
    std::vector<std::string> vPathSegments = c2s::util::splitString( sPath , '/' );
    for ( unsigned int i = 0; i < vPathSegments.size(); ++i )
    {
      if ( vPathSegments[ i ].size() )
        m_pathSegments.appendPathSegment( new C2SRestPathSegment( vPathSegments[ i ] ) );
    }
  };

  C2SRestMethodPrototype::~C2SRestMethodPrototype()
  {
    QueryParameterList::iterator qpIt = m_queryParameters.begin();
    for ( ; qpIt != m_queryParameters.end(); ++qpIt )
      delete qpIt->second;
  };

  C2SHttpResponse *C2SRestMethodPrototype::processRequest( const C2SHttpRequest &request )
  {
    const C2SHttpQueryFields &queryFields = request.header().QueryFields;

    //TODO: what to do with query fields that couldn't be handled
    QueryParameterList::iterator qpit = m_queryParameters.begin();
    QueryParameterList::iterator qpend = m_queryParameters.end();
    for ( ; qpit != qpend; ++qpit )
    {
      const std::string &sParameterID = qpit->first;
      C2SRestQueryParameterBase *pParameter = qpit->second;

      C2SHttpQueryFields::const_iterator qfit = queryFields.find( sParameterID );

      if ( qfit == queryFields.end() )
        pParameter->setDefaultValue();
      else
      {
        const std::string &sFieldValue = qfit->second;
        pParameter->setParameterFromString( sFieldValue );
      }
    }

    return this->process();
  }

  C2SHttpResponse *C2SRestMethodPrototype::buildResponse( C2SHttpStatus status ) const
  {
    return C2SHttpResponse::build( status );
  }

  int C2SRestMethodPrototype::getDistanceToPathIDs( const C2SRestPathIDList &pathList ) const
  {
    return m_pathSegments.getDistanceToPathIDs( pathList );
  }

  void C2SRestMethodPrototype::processPathIDs( const C2SRestPathIDList &pathList )
  {
    m_pathSegments.processPathIDs( pathList );
  }

  bool C2SRestMethodPrototype::isMethodType( C2SHttpMethod methodType ) const
  {
    return methodType == m_methodType;
  }

  bool C2SRestMethodPrototype::operator==( const C2SRestMethodPrototype &methodPrototypeToCheck ) const
  {
    return m_pathSegments == methodPrototypeToCheck.m_pathSegments && m_methodType == methodPrototypeToCheck.m_methodType;
  }

}
