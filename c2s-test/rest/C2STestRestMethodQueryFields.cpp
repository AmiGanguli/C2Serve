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

#include "C2STestRestMethodQueryFields.h"

#include <boost/test/unit_test.hpp>

namespace c2s
{
  namespace test
  {

    const std::string C2STestRestMethodQueryFields::sPath = "check-query-fields/";

    const std::string C2STestRestMethodQueryFields::sQueryField1Name = "uri";

    const std::string C2STestRestMethodQueryFields::sQueryField2Name = "cfg";

    const std::string C2STestRestMethodQueryFields::sQueryField1ValueExpected = "12arg:,478859'*##*";

    const std::string C2STestRestMethodQueryFields::sQueryField2ValueExpected = "field2Received";

    const std::string C2STestRestMethodQueryFields::sQueryField2ValueDefault = "field2default";

    C2STestRestMethodQueryFields::C2STestRestMethodQueryFields()
      : C2SRestMethodPrototypeGET<>( sPath )
    {
      C2SRestMethodPrototypeGET<>::addQueryParameter( sQueryField1Name , &m_sQueryField1ValueReceived );
      C2SRestMethodPrototypeGET<>::addQueryParameter( sQueryField2Name , &m_sQueryField2ValueReceived , sQueryField2ValueDefault );
    };

    C2SHttpResponse *C2STestRestMethodQueryFields::process()
    {
      BOOST_MESSAGE( "C2STestRestMethodQueryFields::process: \"" << m_sQueryField1ValueReceived << "\"; \"" << m_sQueryField2ValueReceived << "\"" );
      BOOST_CHECK( m_sQueryField1ValueReceived == sQueryField1ValueExpected );
      BOOST_CHECK( m_sQueryField2ValueReceived == sQueryField2ValueExpected );
      return C2SRestMethodPrototype::buildResponse( OK );
    }

    C2STestRestMethodQueryFields *C2STestRestMethodQueryFields::clone() const
    {
      return new C2STestRestMethodQueryFields();
    }

  }
}
