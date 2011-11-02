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

#include "C2STestRestMethodAdd.h"

#include "C2STestRestEntityStreamerXML.h"
#include "C2STestRestEntityStreamerJSON.h"

#include <boost/test/unit_test.hpp>

namespace c2s
{
  namespace test
  {

    const std::string C2STestRestMethodAdd::sPath = "math/add";

    C2STestRestMethodAdd::C2STestRestMethodAdd()
      : C2SRestEntityMethodPrototype<unsigned int>( GET , sPath ),
        m_iArg1( 0 ),
        m_iArg2( 0 ),
        m_iMultiplier( 0 )
    {
      C2SRestEntityMethodPrototype<unsigned int>::addPathParameter( "arg1" , &m_iArg1 );
      C2SRestEntityMethodPrototype<unsigned int>::addPathParameter( "arg2" , &m_iArg2 );
      C2SRestEntityMethodPrototype<unsigned int>::addQueryParameter( "multiplier" , &m_iMultiplier , m_iMultiplier );
      C2SRestEntityMethodPrototype<unsigned int>::installEntityStreamer( new C2STestRestEntityStreamerXML<unsigned int>() );
      C2SRestEntityMethodPrototype<unsigned int>::installEntityStreamer( new C2STestRestEntityStreamerJSON<unsigned int>() );
    };

    C2SHttpResponse *C2STestRestMethodAdd::process()
    {
      BOOST_MESSAGE( "C2STestRestMethodAdder::process: " << m_iArg1 << " + " << m_iArg2 << " * " << m_iMultiplier );
      unsigned int iResult = ( m_iArg1 + m_iArg2 ) * m_iMultiplier;
      return C2SRestEntityMethodPrototype<unsigned int>::buildResponse( OK , iResult );
    }

    C2STestRestMethodAdd *C2STestRestMethodAdd::clone() const
    {
      return new C2STestRestMethodAdd();
    }

  }
}

