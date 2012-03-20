/*
 * C2STestRestMethodAddDuplicatePOST.cpp
 *
 *  Created on: Mar 20, 2012
 *      Author: gack
 */

#include "C2STestRestMethodAddDuplicatePOST.h"
#include "C2STestRestMethodAdd.h"

#include "C2STestRestEntityStreamerXML.h"
#include "C2STestRestEntityStreamerJSON.h"

#include <boost/test/unit_test.hpp>

namespace c2s
{
  namespace test
  {

  C2STestRestMethodAddDuplicatePOST::C2STestRestMethodAddDuplicatePOST()
      : C2SRestMethodPrototypePOST<unsigned int>( C2STestRestMethodAdd::sPath ),
        m_iArg1( 0 ),
        m_iArg2( 0 )
    {
      C2SRestMethodPrototypePOST<unsigned int>::addPathParameter( "arg1" , &m_iArg1 );
      C2SRestMethodPrototypePOST<unsigned int>::addPathParameter( "arg2" , &m_iArg2 );
      C2SRestMethodPrototypePOST<unsigned int>::installEntityStreamer( new C2STestRestEntityStreamerXML<unsigned int>() );
      C2SRestMethodPrototypePOST<unsigned int>::installEntityStreamer( new C2STestRestEntityStreamerJSON<unsigned int>() );
    };

    C2SHttpResponse *C2STestRestMethodAddDuplicatePOST::process()
    {
      BOOST_MESSAGE( "C2STestRestMethodAdder::process: " << m_iArg1 << " + " << m_iArg2 );
      unsigned int iResult = m_iArg1 + m_iArg2;
      return C2SRestMethodPrototypePOST<unsigned int>::buildResponse( Created , iResult );
    }

    C2STestRestMethodAddDuplicatePOST *C2STestRestMethodAddDuplicatePOST::clone() const
    {
      return new C2STestRestMethodAddDuplicatePOST();
    }

  }
}
