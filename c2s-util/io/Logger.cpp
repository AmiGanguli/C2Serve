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

#include "Logger.h"

#include <iomanip>

#ifndef NDEBUG
#define GIO_DEFAULTLOGGING_LEVEL 4
#else
#define GIO_DEFAULTLOGGING_LEVEL 2
#endif

#define GIO_LOGGINGLEVEL_ERROR    0
#define GIO_LOGGINGLEVEL_WARNING  1
#define GIO_LOGGINGLEVEL_NOTE     2
#define GIO_LOGGINGLEVEL_INFO     3
#define GIO_LOGGINGLEVEL_DEBUG    4
#define GIO_LOGGINGLEVEL_VERBOSE  5

#define GIO_NUM_LEVEL_CHARS 7


namespace c2s
{
  namespace io
  {

    int Logger::m_iLoggingLevel = GIO_DEFAULTLOGGING_LEVEL;

    bool Logger::m_bWasLastLineNoLineBreak = false;

    //TODO: Is this secure?
    std::ostream *Logger::m_pGlobalStream = &( std::cout );

    Logger::Logger( const std::string &id )
      : m_id( id )
    {
    }

    Logger::~Logger()
    {
    }

    void Logger::error( const std::string &msg , bool bLineBreak ) const
    {
      this->print( "Error" , msg , bLineBreak );
    }

    void Logger::warning( const std::string &msg , bool bLineBreak ) const
    {
      if ( m_iLoggingLevel >= GIO_LOGGINGLEVEL_WARNING )
        this->print( "Warning" , msg , bLineBreak );
    }

    void Logger::note( const std::string &msg , bool bLineBreak ) const
    {
      if ( m_iLoggingLevel >= GIO_LOGGINGLEVEL_NOTE )
        this->print( "Note" , msg , bLineBreak );
    }

    void Logger::info( const std::string &msg , bool bLineBreak ) const
    {
      if ( m_iLoggingLevel >= GIO_LOGGINGLEVEL_INFO )
        this->print( "Info" , msg , bLineBreak );
    }

    void Logger::debug( const std::string &msg , bool bLineBreak ) const
    {
      if ( m_iLoggingLevel >= GIO_LOGGINGLEVEL_DEBUG )
        this->print( "Debug" , msg , bLineBreak );
    }

    void Logger::verbose( const std::string &msg , bool bLineBreak ) const
    {
      if ( m_iLoggingLevel >= GIO_LOGGINGLEVEL_VERBOSE )
        this->print( "Verbose" , msg , bLineBreak );
    }

    void Logger::setOutputStream( std::ostream &os )
    {
      m_pGlobalStream = &os;
    }

    void Logger::setLevel( Level level )
    {
      m_iLoggingLevel = Logger::level2int( level );
    }

    void Logger::setLevel( int iLevel )
    {
      m_iLoggingLevel = iLevel;
    }

    Logger::Level Logger::getLevel()
    {
      return Logger::int2level( m_iLoggingLevel );
    }

    void Logger::print( const std::string &level , const std::string &msg , bool bLineBreak ) const
    {
      std::ostream &os = *m_pGlobalStream;

      if ( bLineBreak && m_bWasLastLineNoLineBreak )
        os << std::endl;

      int iNumBackSpace = this->printPrefix( os , level );
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

    int Logger::printPrefix( std::ostream &os , const std::string &level ) const
    {
      os << "[" << level << "][" << m_id << "]";
      return GIO_NUM_LEVEL_CHARS + m_id.size() + 4;
    }

    int Logger::level2int( Level level )
    {
      if ( level == Error )
        return GIO_LOGGINGLEVEL_ERROR;

      if ( level == Warning )
        return GIO_LOGGINGLEVEL_WARNING;

      if ( level == Note )
        return GIO_LOGGINGLEVEL_NOTE;

      if ( level == Info )
        return GIO_LOGGINGLEVEL_INFO;

      if ( level == Debug )
        return GIO_LOGGINGLEVEL_DEBUG;

      if ( level == Verbose )
        return GIO_LOGGINGLEVEL_VERBOSE;

      return GIO_DEFAULTLOGGING_LEVEL;
    }

    Logger::Level Logger::int2level( int iLevel )
    {
      if ( iLevel == GIO_LOGGINGLEVEL_ERROR )
        return Error;

      if ( iLevel == GIO_LOGGINGLEVEL_WARNING )
        return Warning;

      if ( iLevel == GIO_LOGGINGLEVEL_NOTE )
        return Note;

      if ( iLevel == GIO_LOGGINGLEVEL_INFO )
        return Info;

      if ( iLevel == GIO_LOGGINGLEVEL_DEBUG )
        return Debug;

      return Verbose;
    }

  }
}
