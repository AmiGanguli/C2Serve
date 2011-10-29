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
