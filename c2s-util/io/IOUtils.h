/**

 		IOUtils.h
		
		Created on: 	31.10.2010
	  Author: 			Kay Wolter
		E-Mail:				wolter.kay@freenet.de

		@brief	Add description here!!!

 */

#ifndef IOUTILS_H_
#define IOUTILS_H_

#include <string>
#include <vector>
#include <iostream>

namespace g
{

  namespace io
  {

    std::vector<std::string> readLines( std::istream &is );

  }

}

#endif /* IOUTILS_H_ */
