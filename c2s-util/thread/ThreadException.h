/**

 		ThreadException.h
		
		Created on: 	02.02.2011
	  Author: 			Kay Wolter
		E-Mail:				wolter.kay@freenet.de

		@brief	Add description here!!!

 */

#ifndef THREADEXCEPTION_H_
#define THREADEXCEPTION_H_

#include "GenericException.h"

namespace g
{

  namespace thread
  {

    class ThreadException : public GenericException
    {
    public:

      ThreadException( const std::string &msg ) : GenericException( msg ) {};

    };

  }

}

#endif /* THREADEXCEPTION_H_ */
