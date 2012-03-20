/*
 * C2STestRestMethodDELETE.cpp
 *
 *  Created on: Mar 20, 2012
 *      Author: gack
 */

#include "C2STestRestMethodDELETE.h"

namespace c2s
{

  namespace test
  {

    const std::string C2STestRestMethodDELETE::sPath = "delete";

    C2STestRestMethodDELETE::C2STestRestMethodDELETE()
      : C2SRestMethodPrototypeDELETE( sPath )
    {
    }

    C2STestRestMethodDELETE::~C2STestRestMethodDELETE()
    {
    }

    C2SHttpResponse *C2STestRestMethodDELETE::process()
    {
      return C2SHttpResponse::build( c2s::Created );
    }

    C2SRestMethodPrototypeDELETE *C2STestRestMethodDELETE::clone() const
    {
      return new C2STestRestMethodDELETE();
    }

  }

}
