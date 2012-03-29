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

#include "C2STestRestMethodMediaTypeConverter.h"

#include "C2STestRestEntityStreamerXML.h"
#include "C2STestRestEntityStreamerJSON.h"
#include "C2STestRestEntityUnstreamerXML.h"
#include "C2STestRestEntityUnstreamerJSON.h"

#include <boost/test/unit_test.hpp>

namespace c2s
{
  namespace test
  {

    const std::string C2STestRestMethodMediaTypeConverter::sPathToMethod = "media-type";
    const std::string C2STestRestMethodMediaTypeConverter::sPathSegment = "/convert/xml-or-json/";

    C2STestRestMethodMediaTypeConverter::C2STestRestMethodMediaTypeConverter()
      : C2SRestMethodPrototypePOST<std::string>( sPathToMethod ),
        m_pRequestEntityUnstreamerXML( new C2STestRestEntityUnstreamerXML() ),
        m_pRequestEntityUnstreamerJSON( new C2STestRestEntityUnstreamerJSON() )
    {
      C2SRestMethodPrototypePOST<std::string>::addPathSegment( sPathSegment );
      C2SRestMethodPrototypePOST<std::string>::installRequestEntityUnstreamer( m_pRequestEntityUnstreamerXML );
      C2SRestMethodPrototypePOST<std::string>::installRequestEntityUnstreamer( m_pRequestEntityUnstreamerJSON );
      C2SRestMethodPrototypePOST<std::string>::installEntityStreamer( new C2STestRestEntityStreamerXML<std::string>() );
      C2SRestMethodPrototypePOST<std::string>::installEntityStreamer( new C2STestRestEntityStreamerJSON<std::string>() );
    }

    C2STestRestMethodMediaTypeConverter::~C2STestRestMethodMediaTypeConverter()
    {
      delete m_pRequestEntityUnstreamerXML;
      delete m_pRequestEntityUnstreamerJSON;
    }

    C2SRestMethodPrototype *C2STestRestMethodMediaTypeConverter::clone() const
    {
      return new C2STestRestMethodMediaTypeConverter();
    }

    C2SHttpResponse *C2STestRestMethodMediaTypeConverter::process()
    {
      if ( m_pRequestEntityUnstreamerXML->isDataReceived() )
        return this->processXML();

      return this->processJSON();
    }

    C2SHttpResponse *C2STestRestMethodMediaTypeConverter::processXML()
    {
      BOOST_CHECK( m_pRequestEntityUnstreamerXML->isDataReceived() );
      BOOST_CHECK( !m_pRequestEntityUnstreamerJSON->isDataReceived() );
      const std::string &sContentReceived = m_pRequestEntityUnstreamerXML->getReceivedContent();
      m_pRequestEntityUnstreamerXML->setIsDataReceived( false );
      return C2SRestMethodPrototypePOST<std::string>::buildResponse( Created , sContentReceived );
    }

    C2SHttpResponse *C2STestRestMethodMediaTypeConverter::processJSON()
    {
      BOOST_CHECK( m_pRequestEntityUnstreamerJSON->isDataReceived() );
      BOOST_CHECK( !m_pRequestEntityUnstreamerXML->isDataReceived() );
      const std::string &sContentReceived = m_pRequestEntityUnstreamerJSON->getReceivedContent();
      m_pRequestEntityUnstreamerJSON->setIsDataReceived( false );
      return C2SRestMethodPrototypePOST<std::string>::buildResponse( Created , sContentReceived );
    }

  }
}
