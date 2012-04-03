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

#include "C2SSocketAcceptHandler.h"
#include "C2SDataPullInterface.h"
#include "C2SDataHandlingInterface.h"

#include "C2SLock.h"
#include "C2SMutex.h"

#ifndef WINXX
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#include <cstring>

#include <iostream>

#define BUFFERSIZE 4096

namespace c2s
{

  void C2SSocketAcceptHandlerDataPushImpl::push( const char *data , unsigned int size )
  {
    if ( m_socketInfo.SocketDescriptor < 0 )
      throw C2SSocketAcceptHandlerException( "C2SSocketAcceptHandlerDataCallback::handle: Invalid socket file descriptor!" );

#ifdef WINXX
    int iBytesWritten = send( m_socketInfo.SocketDescriptor , data , size , 0 );
#else
    int iBytesWritten = write( m_socketInfo.SocketDescriptor , data , size );
#endif
    if ( iBytesWritten < 0 )
      throw C2SSocketAcceptHandlerException( "C2SSocketAcceptHandlerDataCallback::handle: Error writing to socket!" );
  }

  //pDataHandler will be deleted
  C2SSocketAcceptHandler::C2SSocketAcceptHandler( const C2SSocketInfo &socketInfo , const C2SDataHandlingInterface &dataHandling , c2s::thread::C2SMutex *pAcceptMutex )
    : m_socketInfo( socketInfo ),
      m_pDataPull( NULL ),
      m_bInterrupted( false ),
      m_acceptMutex( *pAcceptMutex )
  {
    m_pDataPull = dataHandling.createDataPullForDataPush( &m_dataPush );
  }

  C2SSocketAcceptHandler::~C2SSocketAcceptHandler()
  {
    delete m_pDataPull;
  }

  void C2SSocketAcceptHandler::interrupt()
  {
    //TODO: check and block if accept() is processing a requests
    m_bInterrupted = true;
  }

  void C2SSocketAcceptHandler::accept()
  {
#ifndef WINXX
    //address of the client
    struct sockaddr_in clientAddress;
    socklen_t iClientAddressLength = sizeof( clientAddress );
#endif

    {
      c2s::thread::C2SLock<c2s::thread::C2SMutex> lock( &m_acceptMutex );

      if ( m_bInterrupted )
        return;

      //block process, until client connects
      //returns new socket file descriptor which should be used for communication
#ifdef WINXX
      m_connectionSocketInfo.SocketDescriptor = ::accept( m_socketInfo.SocketDescriptor , NULL , NULL );
#else
      m_connectionSocketInfo.SocketDescriptor = ::accept( m_socketInfo.SocketDescriptor , ( struct sockaddr* ) &clientAddress , &iClientAddressLength );
#endif
    }

    if ( m_bInterrupted )
      return;

    if ( m_connectionSocketInfo.SocketDescriptor < 0 )
      throw C2SSocketAcceptHandlerException( "C2SSocketAcceptHandler::listen: Error on accept!" );

    m_dataPush.setSocketInfo( m_connectionSocketInfo );

    m_pDataPull->reset();

    //initialize data buffer
    char buffer[ BUFFERSIZE ];

#ifndef WINXX
    bzero( buffer , BUFFERSIZE );
#endif

    int iBytesRead = BUFFERSIZE;
//    while ( iBytesRead == BUFFERSIZE )
    //TODO: This is a workaround: Some clients may not call shutdown after writing to socket is completed, so we have to check ourselves if the request is complete
    //      What if the client is buggy? Timeout?
    while ( iBytesRead > 0 && !m_pDataPull->isComplete() )
    {
      //read data from buffer
#ifdef WINXX
      iBytesRead = recv( m_connectionSocketInfo.SocketDescriptor , buffer , BUFFERSIZE , 0 );
#else
      iBytesRead = recv( m_connectionSocketInfo.SocketDescriptor , buffer , BUFFERSIZE , 0 );
#endif

      if ( iBytesRead < 0 )
        throw C2SSocketAcceptHandlerException( "C2SSocketAcceptHandler::listen: Error reading from socket!" );

      m_pDataPull->receive( buffer , iBytesRead );
    }

    try
    {
    	m_pDataPull->flush();
    }
    catch( const C2SException &e )
    {
      //TODO: Should be handled by server type (return appropriate response)
      std::cerr << "C2SSocketAcceptHandler:accept: ERROR: " << e.what() << std::endl;
    }

    //close socket
#ifdef WINXX
    closesocket( m_connectionSocketInfo.SocketDescriptor );
#else
    close( m_connectionSocketInfo.SocketDescriptor );
#endif
  }

}
