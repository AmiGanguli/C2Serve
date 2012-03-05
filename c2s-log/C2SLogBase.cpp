/**

 Copyright (c) 2012, C2Serve (http://www.c2serve.eu)
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

#include "C2SLogBase.h"

#include "C2SLogMutex.h"
#include "C2SLogPublicationInterface.h"

#include "Lock.h"

namespace c2s
{

  ELogLevel C2SLogBase::LogLevel = Warning;

  C2SLogBase::C2SLogBase( const C2SLogPublicationInterface &logPublication )
    : m_logPublication( logPublication ),
      m_pLogMutex( new C2SLogMutex() )
  {
  }

  C2SLogBase::~C2SLogBase()
  {
    delete m_pLogMutex;
  }

  void C2SLogBase::error( const std::string &sLogMessage ) const
  {
    this->publishMessageSynchronized( sLogMessage );
  }

  void C2SLogBase::warning( const std::string &sLogMessage ) const
  {
    if ( LogLevel >= Warning )
      this->publishMessageSynchronized( sLogMessage );
  }

  void C2SLogBase::note( const std::string &sLogMessage ) const
  {
    if ( LogLevel >= Note )
      this->publishMessageSynchronized( sLogMessage );
  }

  void C2SLogBase::info( const std::string &sLogMessage ) const
  {
    if ( LogLevel >= Info )
      this->publishMessageSynchronized( sLogMessage );
  }

  void C2SLogBase::debug( const std::string &sLogMessage ) const
  {
    if ( LogLevel >= Debug )
      this->publishMessageSynchronized( sLogMessage );
  }

  void C2SLogBase::verbose( const std::string &sLogMessage ) const
  {
    if ( LogLevel >= Verbose )
      this->publishMessageSynchronized( sLogMessage );
  }

  void C2SLogBase::publishMessageSynchronized( const std::string &sLogMessage ) const
  {
    thread::Lock<C2SLogMutex> m_lock( m_pLogMutex );
    m_logPublication.publishMessage( sLogMessage );
  }

}