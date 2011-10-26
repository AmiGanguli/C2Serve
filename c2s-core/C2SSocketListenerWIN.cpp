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

#include "C2SSocketListenerWIN.h"

#include "ForRestServerType.h"

#ifdef DEBUG
#define BUFFERSIZE 16
#else
#define BUFFERSIZE 4096
#endif

namespace g
{

  namespace c2s
  {

    void C2SSocketListenerWINDataCallback::handle( const char *data , unsigned int size )
    {
      if ( m_socketInfo.SocketDescriptor == INVALID_SOCKET )
        throw C2SSocketListenerException( "C2SSocketListenerWINDataCallback::handle: Invalid socket descriptor!" );

      int iBytesWritten = send( m_socketInfo.SocketDescriptor , data , size , 0 );
      if ( iBytesWritten < 0 )
        throw C2SSocketListenerException( "C2SSocketListenerWINDataCallback::handle: Error writing to socket!" );
    }

    C2SSocketListenerWIN::C2SSocketListenerWIN( const ForRestSocketInfo &socketInfo , const ForRestServerType &type , g::thread::Mutex *pAcceptMutex )
      : C2SSocketListenerBase( socketInfo , pAcceptMutex ),
        m_bInterrupted( false )
    {
      m_pDataHandler = type.createDataHandler( &m_dataCallback );
    }

    C2SSocketListenerWIN::~C2SSocketListenerWIN()
    {
    }

    void C2SSocketListenerWIN::interrupt()
    {
      //TODO: check and block if accept() is processing a requests
      m_bInterrupted = true;
    }

    void C2SSocketListenerWIN::accept()
    {
      {
        g::thread::Lock lock( &m_acceptMutex );

        if ( m_bInterrupted )
          return;

        //block process, until client connects
        //returns new socket file descriptor which should be used for communication
        m_connectionSocketInfo.SocketDescriptor = ::accept( m_socketInfo.SocketDescriptor , NULL , NULL );
      }

      if ( m_bInterrupted )
        return;

      if ( m_connectionSocketInfo.SocketDescriptor == INVALID_SOCKET )
        throw C2SSocketListenerException( "C2SSocketListenerWIN::listen: Error on accept!" );

      m_dataCallback.setSocketInfo( m_connectionSocketInfo );

      m_pDataHandler->reset();

      //initialize data buffer
      char buffer[ BUFFERSIZE ];
//      bzero( buffer , BUFFERSIZE );

      long iBytesRead = BUFFERSIZE;
      while ( iBytesRead == BUFFERSIZE )
      {
        //read data from buffer
        iBytesRead = recv( m_connectionSocketInfo.SocketDescriptor , buffer , BUFFERSIZE , 0 );
        if ( iBytesRead < 0 )
          throw C2SSocketListenerException( "C2SSocketListenerWIN::listen: Error reading from socket!" );

        m_pDataHandler->handle( buffer , iBytesRead );
      }

      m_pDataHandler->process();

      //close socket
      closesocket( m_connectionSocketInfo.SocketDescriptor );
    }

  }

}
