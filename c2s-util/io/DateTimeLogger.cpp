/**

 DateTimeLogger.cpp
 
 Created on: 	  02.10.2010
 Author: 			  Kay Wolter
 E-Mail:				wolter.kay@freenet.de

 @brief DateTimeLogger implementation file.

 */

#include "DateTimeLogger.h"

#include "IOException.h"

#include <sstream>
#include <iomanip>

#ifndef WINXX
#include <sys/time.h>
#endif

namespace g
{
  namespace io
  {

    bool DateTimeLogger::m_bShowMilliSec = false;

    DateTimeLogger::DateTimeLogger( const std::string &id )
      : Logger( id )
    {
    }

    DateTimeLogger::~DateTimeLogger()
    {
    }

    void DateTimeLogger::setShowMilliSeconds( bool bShowMilliSeconds )
    {
      m_bShowMilliSec = bShowMilliSeconds;
    }

    void DateTimeLogger::print( const std::string &level , const std::string &msg , bool bLineBreak ) const
    {
      std::ostream &os = *m_pGlobalStream;

      if ( bLineBreak && m_bWasLastLineNoLineBreak )
        os << std::endl;

      std::string sTime = DateTimeLogger::getCurrentDateAndTimeAsString();
      int iNumBackSpace = sTime.size() + 2;
      os << "[" << sTime << "]";

      iNumBackSpace += this->printPrefix( os , level );
      iNumBackSpace += msg.size() +1;

      os << " " << msg;

      if ( bLineBreak )
      {
        os << std::endl;
        m_bWasLastLineNoLineBreak = false;
      }
      else
      {
        os << "\r" << std::flush;
        m_bWasLastLineNoLineBreak = true;
      }
    }

    std::string DateTimeLogger::getCurrentDateAndTimeAsString()
    {
#ifdef WINXX
      throw IOException( "TimeLogger::getCurrentTimeAsString: Not yet supported on windows" );
#else
      struct timeval time_ms;
      struct timezone tzp;
      time_t rawtime;
      struct tm *timeinfo;

      //get current time
      gettimeofday ( &time_ms , &tzp );
      time ( &rawtime );
      timeinfo = localtime ( &rawtime );

      std::stringstream strs;
      strs << std::setfill( '0' );
      //date
      strs << std::setw( 4 ) << timeinfo->tm_year + 1900 << "-" << std::setw( 2 ) << timeinfo->tm_mon + 1 << "-" << std::setw( 2 ) << timeinfo->tm_mday << "][";
      //time
      strs << std::setw( 2 ) << timeinfo->tm_hour << ":" << std::setw( 2 ) << timeinfo->tm_min << ":" << std::setw( 2 ) << timeinfo->tm_sec;

      //millisec
      if ( m_bShowMilliSec )
        strs << ":" << std::setw( 3 ) << time_ms.tv_usec / 1000;

      return strs.str();
#endif
    }
  }
}
