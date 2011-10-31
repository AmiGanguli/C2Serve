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

#include "C2SRestMatchMethodToRequest.h"
#include "C2SRestMethodPrototype.h"

#include "C2SHttpRequest.h"
#include "C2SStringUtils.h"

#include "StringUtils.h"

namespace c2s
{

  C2SRestMatchMethodToRequest::C2SRestMatchMethodToRequest( const std::string &sContextRootOfResource , const C2SRestMethodPrototypeList &listOfAvailableMethodPrototypes , const C2SHttpRequest &requestToMatch )
    : m_sContextRootOfResource( sContextRootOfResource ),
      m_listOfAvailableMethodPrototypes( listOfAvailableMethodPrototypes ),
      m_requestToMatch( requestToMatch )
  {
  }

  C2SRestMatchMethodToRequest::~C2SRestMatchMethodToRequest()
  {
  }

  C2SRestPathIDList C2SRestMatchMethodToRequest::createPathIDListFromRequest() const
  {
    const std::string &sCompleteRequestURI = m_requestToMatch.header().URI;
    int iSkipIdx = c2s::uriSkip( sCompleteRequestURI.c_str() , sCompleteRequestURI.size() , m_sContextRootOfResource.c_str() , m_sContextRootOfResource.size() , false );

    if ( iSkipIdx < 0 )
      throw C2SRestException( "C2SRestMatchMethodToRequest::createPathIDListFromRequest: " , "Cannot parse resource URI: " + sCompleteRequestURI , InternalServerError );

    return c2s::util::splitString( sCompleteRequestURI.substr( iSkipIdx , sCompleteRequestURI.size() ) , '/' , true );
  }

  std::map<int,C2SRestMethodPrototype*> C2SRestMatchMethodToRequest::getMethodCandidatesSortedByDistanceToPathIDs( const C2SRestPathIDList &listOfRequestPathIDs ) const
  {
    std::map<int,C2SRestMethodPrototype*> listOfMethodCandidatesSortedByDistanceToPathIDs;
    C2SRestMethodPrototypeList::const_iterator methodPrototypeIt = m_listOfAvailableMethodPrototypes.begin();
    C2SRestMethodPrototypeList::const_iterator methodPrototypeEnd = m_listOfAvailableMethodPrototypes.end();
    for ( ; methodPrototypeIt != methodPrototypeEnd; ++methodPrototypeIt )
    {
      C2SRestMethodPrototype *pCurrenMethodPrototypeObserved = *methodPrototypeIt;
      int iDistanceToListOfPathIDs = pCurrenMethodPrototypeObserved->getDistanceToPathIDs( listOfRequestPathIDs );

      if ( iDistanceToListOfPathIDs < 0 ) //mismatch
        continue;

      //this must be prevented when adding methods to a resource
      if ( listOfMethodCandidatesSortedByDistanceToPathIDs.find( iDistanceToListOfPathIDs ) != listOfMethodCandidatesSortedByDistanceToPathIDs.end() )
        throw C2SRestException( "C2SRestMatchMethodToRequest::getMethodCandidatesSortedByDistanceToPathIDs: " , "Detected duplicate path distance, two or more methods match this call: " + m_requestToMatch.header().URI , InternalServerError );

      listOfMethodCandidatesSortedByDistanceToPathIDs[ iDistanceToListOfPathIDs ] = pCurrenMethodPrototypeObserved;
    }

    return listOfMethodCandidatesSortedByDistanceToPathIDs;
  }

  C2SRestMethodPrototype *C2SRestMatchMethodToRequest::getPrototypeClosestToRequestByConsideringMediaType( const std::map<int,C2SRestMethodPrototype*> &listOfMethodCandidatesSortedByDistanceToPathIDs ) const
  {
    std::map<int,C2SRestMethodPrototype*>::const_iterator cit = listOfMethodCandidatesSortedByDistanceToPathIDs.begin();
    std::map<int,C2SRestMethodPrototype*>::const_iterator cend = listOfMethodCandidatesSortedByDistanceToPathIDs.end();
    for ( ; cit != cend; ++cit )
    {
      C2SRestMethodPrototype *pCurrenMethodPrototypeObserved = cit->second;
      if ( !pCurrenMethodPrototypeObserved->isMethodType( m_requestToMatch.header().Method ) )
        continue;

      return pCurrenMethodPrototypeObserved;
    }

    throw C2SRestException( "C2SRestMatchMethodToRequest::getPrototypeClosestToRequestByConsideringMediaType: " , "Method not allowed" , MethodNotAllowed );
  }

  C2SRestMethodPrototype *C2SRestMatchMethodToRequest::getPrototypeWithBestMatchAndPrepareFromURI() const
  {
    if ( !m_listOfAvailableMethodPrototypes.size() )
      return NULL;

    C2SRestPathIDList pathList = this->createPathIDListFromRequest();
    std::map<int,C2SRestMethodPrototype*> listOfMethodCandidatesSortedByDistanceToPathIDs = this->getMethodCandidatesSortedByDistanceToPathIDs( pathList );

    if ( !listOfMethodCandidatesSortedByDistanceToPathIDs.size() )
      return NULL;

    C2SRestMethodPrototype *pPrototypeWithBestMatch = this->getPrototypeClosestToRequestByConsideringMediaType( listOfMethodCandidatesSortedByDistanceToPathIDs );
    pPrototypeWithBestMatch->processPathIDs( pathList );
    return pPrototypeWithBestMatch;
  }

}
