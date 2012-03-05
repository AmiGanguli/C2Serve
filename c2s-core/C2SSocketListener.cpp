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

#include "C2SSocketListener.h"

#include "C2SRuntime.h"
#include "C2SSocketInfo.h"
#include "C2SSocketAcceptHandler.h"
#include "C2SLogInterface.h"

#include "ThreadQueue.h"
#include "StringUtils.h"

#include <cstring>

namespace c2s
{

  C2SSocketListener::C2SSocketListener( const C2SSocketListenerSettings &settings , const C2SDataHandlingInterface &dataHandling , const C2SLogInterface &logInstance )
    : m_settings( settings ),
      m_dataHandling( dataHandling ),
      m_pSocketInfo( new C2SSocketInfo() ),
      m_bKeepRunning( false ),
      m_logInstance( logInstance )
  {
  }

  C2SSocketListener::~C2SSocketListener()
  {
    C2SSocketAcceptHandlerList::iterator it = m_acceptHandlers.begin();
    for ( ; it != m_acceptHandlers.end(); ++it )
      delete ( *it );

    delete m_pSocketInfo;

//    m_type.release();
  }

  void C2SSocketListener::connectAndRun()
  {
    this->connect();
    m_bKeepRunning = true;
    c2s::thread::Mutex *pAcceptMutex = new c2s::thread::Mutex();
    c2s::thread::ThreadQueue<C2SSocketAcceptHandler> *pAcceptThreadQueue = new c2s::thread::ThreadQueue<C2SSocketAcceptHandler>();

    for ( unsigned int i = 0; i < m_settings.iNumThreads; ++i )
    {
      m_acceptHandlers.push_back( new C2SSocketAcceptHandler( *m_pSocketInfo , m_dataHandling , pAcceptMutex ) );
      pAcceptThreadQueue->queue( m_acceptHandlers.back() );
    }

    //TODO: check kill signal
    while ( m_bKeepRunning )
      pAcceptThreadQueue->start();

    pAcceptThreadQueue->join();

    delete pAcceptThreadQueue;
    delete pAcceptMutex;
  }

  void C2SSocketListener::interrupt()
  {
    m_logInstance.note( "Shutdown socket lister, port " + c2s::util::toString( m_settings.iPort ) );

    m_bKeepRunning = false;

    C2SSocketAcceptHandlerList::iterator it = m_acceptHandlers.begin();
    for ( ; it != m_acceptHandlers.end(); ++it )
      ( *it )->interrupt();


#ifdef WINXX

      if ( m_pSocketInfo->SocketDescriptor != INVALID_SOCKET )
        closesocket( m_pSocketInfo->SocketDescriptor );

      WSACleanup();

#else
      //shutdown
      ::shutdown( m_pSocketInfo->SocketDescriptor , 2 );
      close( m_pSocketInfo->SocketDescriptor );
#endif

  }

#ifdef WINXX

  void C2SSocketListener::connect()
  {
    WSADATA wsa;
    int ret;

    //initialize winsock
    //use version 2.0
    //struct wsa will be filled with information about winsock
    ret = WSAStartup( MAKEWORD( 2 , 0 ) , &wsa );
    if ( ret != 0 )
      throw C2SSocketListenerException( "C2SSocketListener::connect: Could not initialize winsock! Error: " + c2s::util::toString( ret ) );

    SOCKADDR_IN addr;

    m_pSocketInfo->SocketDescriptor = socket( AF_INET , SOCK_STREAM , 0 );
    if ( m_pSocketInfo->SocketDescriptor == INVALID_SOCKET )
      throw C2SSocketListenerException( "C2SSocketListener::connect: Could not create socket! Error: " + c2s::util::toString( WSAGetLastError() ) );

    m_logInstance.note( "Binding socket, port " + c2s::util::toString( m_settings.iPort ) );

    memset( &addr , 0 , sizeof( SOCKADDR_IN ) );
    addr.sin_family = AF_INET;
    addr.sin_port = htons( m_settings.iPort );
    addr.sin_addr.s_addr = ADDR_ANY;

    ret = bind( m_pSocketInfo->SocketDescriptor , ( SOCKADDR* ) &addr , sizeof( SOCKADDR_IN ) );
    if( ret == SOCKET_ERROR )
      throw C2SSocketListenerException( "C2SSocketListener::connect: Could not bind socket! Error: " + c2s::util::toString( WSAGetLastError() ) );

    m_logInstance.note( "Creating socket listener (size of backlog queue: " + c2s::util::toString( m_settings.iSizeBacklogQueue ) + ")" );

    ret = listen( m_pSocketInfo->SocketDescriptor , m_settings.iSizeBacklogQueue );
    if ( ret == SOCKET_ERROR )
      throw C2SSocketListenerException( "C2SSocketListener::connect: Could not create listener! Error: " + c2s::util::toString( WSAGetLastError() ) );


  }

#else

  void C2SSocketListener::connect()
  {
    struct sockaddr_in serverAddress;

    //create a new socket
    //AF_INET: address domain of the socket (Internet domain)
    //SOCK_STREAM: socket type
    //third argument: protocol to use.. if 0, it is chosen by the system (will be TCP for stream sockets)
    m_pSocketInfo->SocketDescriptor = socket( AF_INET , SOCK_STREAM , 0 );

    if ( m_pSocketInfo->SocketDescriptor < 0 )
      throw C2SSocketListenerException( "C2SSocketConnectorUNIX::connect: Error opening socket!" );

#ifndef WINXX
    //set server address to zero
    bzero( reinterpret_cast<char*>( &serverAddress ) , sizeof( serverAddress ) );
#endif

    //set address family
    serverAddress.sin_family = AF_INET;

    //set IP address of the host (is the IP of the machine, INADDR_ANY gets IP for for us)
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    //set port number
    //htons: convert port number in host byte order to port number in network byte order
    serverAddress.sin_port = htons( m_settings.iPort );

    m_logInstance.note( "Binding socket, port " + c2s::util::toString( m_settings.iPort ) );

    //bind socket to address
    //TODO: check error codes(??)
    if ( bind( m_pSocketInfo->SocketDescriptor , reinterpret_cast<struct sockaddr*>( &serverAddress ) , sizeof( serverAddress ) ) < 0 )
      throw C2SSocketListenerException( "C2SSocketConnectorUNIX::connect: Error binding socket!" );

    m_logInstance.note( "Creating socket listener (size of backlog queue: " + c2s::util::toString( m_settings.iSizeBacklogQueue ) + ")" );

    //listen for connections on socket
    //second argument: size of the backlog queue(number of connections allowed waiting while process is handling a particular connection)
    //                 should be set to 5 (maximum size permitted on most systems)
    //if socket is valid, this function does not fail
    listen( m_pSocketInfo->SocketDescriptor , m_settings.iSizeBacklogQueue );
  }

#endif

}
