/**

 		IOUtils.cpp
		
		Created on: 	31.10.2010
	  Author: 			Kay Wolter
		E-Mail:				wolter.kay@freenet.de

		@brief	Add description here!!!

 */

#include "IOUtils.h"

#define BUFFER_SIZE 4096

namespace g
{

  namespace io
  {

    std::vector<std::string> readLines( std::istream &is )
    {
      std::vector<std::string> lines;
      char line[ BUFFER_SIZE ];

      while ( !is.eof() )
      {
        is.getline( line , BUFFER_SIZE );
        lines.push_back( line );
      }
      return lines;
    }

  }

}
