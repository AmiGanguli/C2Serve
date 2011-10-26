/**

 TimeLogger.cpp
 
 Created on: 	  02.10.2010
 Author: 			  Kay Wolter
 E-Mail:				wolter.kay@freenet.de

 @brief TimeLogger implementation file.

 */

#include "TimeLogger.h"

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

    bool TimeLogger::m_bShowMilliSec = true;

    TimeLogger::TimeLogger( const std::string &id  )
      : Logger( id )
    {
    }

    TimeLogger::~TimeLogger()
    {
    }

    void TimeLogger::setShowMilliSeconds( bool bShowMilliSeconds )
    {
      m_bShowMilliSec = bShowMilliSeconds;
    }

    void TimeLogger::print( const std::string &level , const std::string &msg , bool bLineBreak ) const
    {
      std::ostream &os = *m_pGlobalStream;

      if ( bLineBreak && m_bWasLastLineNoLineBreak )
        os << std::endl;

      std::string sTime = TimeLogger::getCurrentTimeAsString();
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

    std::string TimeLogger::getCurrentTimeAsString()
    {
#ifdef WINXX
      throw IOException( "TimeLogger::getCurrentTimeAsString: Not yet supported on windows" );
#else
      struct timeval time_ms;
      struct timezone tzp;

      time_t rawtime;
      struct tm *timeinfo;

      gettimeofday ( &time_ms , &tzp );
      time ( &rawtime );

      timeinfo = localtime ( &rawtime );
      std::stringstream strs;
      strs << std::setfill( '0' );

      //time
      strs << std::setw( 2 ) << timeinfo->tm_hour << ":" << std::setw( 2 ) << timeinfo->tm_min << ":" << std::setw( 2 ) << timeinfo->tm_sec;

      if ( m_bShowMilliSec )
        strs << ":" << std::setw( 3 ) << time_ms.tv_usec / 1000;

      return strs.str();
#endif
    }

  }
}
