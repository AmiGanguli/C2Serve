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

#include "C2SSocketClientConnection.h"
#include "C2SSocketClientException.h"
#include "C2SSocketInfo.h"
#include "StringUtils.h"

#include <iostream>
#include <cstring>

namespace c2s
{

  C2SSocketClientConnection::C2SSocketClientConnection( const std::string &sHost , unsigned int iPort )
    : m_sHost( sHost ),
      m_iPort( iPort ),
      m_pSocketInfo( new C2SSocketInfo() )
  {
#ifdef WINXX
    WSADATA wsa;
    int ret;

    //initialize winsock
    //use version 2.0
    //struct wsa will be filled with information about winsock
    ret = WSAStartup( MAKEWORD( 2 , 0 ) , &wsa );
    if ( ret != 0 )
      throw C2SSocketClientException( "C2SSocketClientConnection::C2SSocketClientConnection: Could not initialize winsock! Error: " + c2s::util::toString( ret ) );
#endif

    this->connectSocket();
  }

  C2SSocketClientConnection::~C2SSocketClientConnection()
  {
    this->closeSocket();
    delete m_pSocketInfo;
#ifdef WINXX
    WSACleanup();
#endif
  }

  void C2SSocketClientConnection::writeToSocket( const char *pDataToWriteToSocket , unsigned int iDataLength )
  {
    int iBytesWrittenToSocket = 0;
    while ( iBytesWrittenToSocket < static_cast<int>( iDataLength ) )
    {
      const char *pDataLeftToWriteToSocket = &pDataToWriteToSocket[ iBytesWrittenToSocket ];
      unsigned int iDataLengthLeftToWriteToSocket = iDataLength - iBytesWrittenToSocket;
#ifdef WINXX
      iBytesWrittenToSocket += send( m_pSocketInfo->SocketDescriptor , pDataLeftToWriteToSocket , iDataLengthLeftToWriteToSocket , 0 );
#else
      iBytesWrittenToSocket += write( m_pSocketInfo->SocketDescriptor , pDataLeftToWriteToSocket , iDataLengthLeftToWriteToSocket );
#endif
      if ( iBytesWrittenToSocket < 0 )
        throw C2SSocketClientException( "C2SSocketClientConnection::writeToSocket: ERROR writing to socket" );
    }
#ifdef WINXX
    if ( shutdown( m_pSocketInfo->SocketDescriptor , SD_SEND ) )
      throw C2SSocketClientException( "C2SSocketClientConnection::writeToSocket: Cannot shutdown socket! Error: " + c2s::util::toString( WSAGetLastError() ) );
#else
    if ( shutdown( m_pSocketInfo->SocketDescriptor , SHUT_WR ) )
      throw C2SSocketClientException( "C2SSocketClientConnection::writeToSocket: Cannot shutdown socket!" );
#endif
  }

  unsigned int C2SSocketClientConnection::readFromSocket( char *pBufferToWriteDataReadFromSocket , unsigned int iBufferSize )
  {
    long iNumberOfBytesReadFromSocket = iBufferSize;
#ifdef WINXX
    iNumberOfBytesReadFromSocket = recv( m_pSocketInfo->SocketDescriptor , pBufferToWriteDataReadFromSocket , iBufferSize , MSG_PARTIAL );
#else
    iNumberOfBytesReadFromSocket = read( m_pSocketInfo->SocketDescriptor , pBufferToWriteDataReadFromSocket , iBufferSize );
#endif
    if ( iNumberOfBytesReadFromSocket < 0 )
      throw C2SSocketClientException( "C2SSocketClientConnection::readFromSocket: Error reading from socket!" );
    return iNumberOfBytesReadFromSocket;
  }

  void C2SSocketClientConnection::connectSocket()
  {
    m_pSocketInfo->SocketDescriptor = socket( AF_INET , SOCK_STREAM , 0 );
#ifdef WINXX
    if ( m_pSocketInfo->SocketDescriptor == INVALID_SOCKET )
      throw C2SSocketClientException( "C2SSocketClientConnection::connectSocket: Could not create socket! Error: " + c2s::util::toString( WSAGetLastError() ) );

    //TODO: release addr??
    SOCKADDR_IN addr;
    memset( &addr , 0 , sizeof( SOCKADDR_IN ) );
    addr.sin_family = AF_INET;
    addr.sin_port = htons( static_cast<u_short>( m_iPort ) );
    addr.sin_addr.s_addr = inet_addr( m_sHost.c_str() );

    unsigned long ip = inet_addr( m_sHost.c_str() );
    if( ip != INADDR_NONE )
      addr.sin_addr.s_addr = ip;
    else
    {
      // resolve host name to ip
      //TODO: release he??
      HOSTENT* he = gethostbyname( m_sHost.c_str() );
      if( he == NULL )
        throw C2SSocketClientException( "C2SSocketClientConnection::connectSocket: Cannot resolve host: " + m_sHost );

      memcpy( &( addr.sin_addr ) , he->h_addr_list[ 0 ] , 4 );
    }

    int ret = connect( m_pSocketInfo->SocketDescriptor , ( SOCKADDR* )&addr , sizeof( SOCKADDR ) );

    if( ret == SOCKET_ERROR )
      throw C2SSocketClientException( "C2SSocketClientConnection::connectSocket: Could not connect to server: " + m_sHost + "; Port: " + c2s::util::toString( m_iPort ) + "! Error: " + c2s::util::toString( WSAGetLastError() ) );

#else

    struct sockaddr_in serverAddress;
    struct hostent *server;

    if ( m_pSocketInfo->SocketDescriptor < 0 )
      throw C2SSocketClientException( "C2SSocketClient::connectToSocket: ERROR opening socket" );

    server = gethostbyname( m_sHost.c_str() );
    if ( server == NULL )
      throw C2SSocketClientException( "C2SSocketClient::connectToSocket: ERROR, no such host" );

    bzero( reinterpret_cast<char *>( &serverAddress ) , sizeof( serverAddress ) );
    serverAddress.sin_family = AF_INET;
    bcopy( ( char * ) server->h_addr , ( char * ) &serverAddress.sin_addr.s_addr , server->h_length );

    serverAddress.sin_port = htons( m_iPort );
    if ( connect( m_pSocketInfo->SocketDescriptor , ( struct sockaddr *) &serverAddress , sizeof( serverAddress ) ) < 0 )
      throw C2SSocketClientException( "C2SSocketClient::connectToSocket: ERROR connecting to socket" );

#endif
  }

  void C2SSocketClientConnection::closeSocket()
  {
#ifdef WINXX
    closesocket( m_pSocketInfo->SocketDescriptor );
#else
    close( m_pSocketInfo->SocketDescriptor );
#endif
  }

}
