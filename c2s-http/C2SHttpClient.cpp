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

#include "C2SHttpClient.h"
#include "C2SHttpRequest.h"

#include "StringUtils.h"

#include <sys/types.h>
#include <cstring>

#include <iostream>

#include "C2SSocketInfo.h"
//#ifndef WINXX
//
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <netdb.h>
//
//#endif

#ifdef DEBUG
#define BUFFERSIZE 16
#else
#define BUFFERSIZE 4096
#endif

namespace c2s
{

  C2SHttpClient::C2SHttpClient( const std::string &sHost , unsigned short iPortNumber )
    : m_sHost( sHost ),
      m_iPortNumber( iPortNumber )
  {
#ifdef WINXX
    WSADATA wsa;
    int ret;

    //initialize winsock
    //use version 2.0
    //struct wsa will be filled with information about winsock
    ret = WSAStartup( MAKEWORD( 2 , 0 ) , &wsa );
    if ( ret != 0 )
      throw C2SHttpClientException( "C2SHttpClient::C2SHttpClient: Could not initialize winsock! Error: " + c2s::util::toString( ret ) );
#endif
  }

  C2SHttpClient::~C2SHttpClient()
  {
#ifdef WINXX
    WSACleanup();
#endif
  }

  C2SHttpResponse *C2SHttpClient::send( const C2SHttpRequest &request )
  {
    C2SSocketInfo info;

#ifdef WINXX

    SOCKADDR_IN addr;

    info.SocketDescriptor = socket( AF_INET , SOCK_STREAM , 0 );
    if ( info.SocketDescriptor == INVALID_SOCKET )
      throw C2SHttpClientException( "C2SHttpClient::send: Could not create socket! Error: " + c2s::util::toString( WSAGetLastError() ) );

    //TODO: release addr??
    memset( &addr , 0 , sizeof( SOCKADDR_IN ) );
    addr.sin_family = AF_INET;
    addr.sin_port = htons( m_iPortNumber );
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
        throw C2SHttpClientException( "C2SHttpClient::send: Cannot resolve host: " + m_sHost );

      memcpy( &( addr.sin_addr ) , he->h_addr_list[ 0 ] , 4 );
    }

    int ret = connect( info.SocketDescriptor , ( SOCKADDR* )&addr , sizeof( SOCKADDR ) );

    if( ret == SOCKET_ERROR )
      throw C2SHttpClientException( "C2SHttpClient::send: Could not connect to server: " + m_sHost + "; Port: " + c2s::util::toString( m_iPortNumber ) + "! Error: " + c2s::util::toString( WSAGetLastError() ) );

#else //WINXX

    struct sockaddr_in serverAddress;
    struct hostent *server;

    info.SocketDescriptor = socket( AF_INET , SOCK_STREAM , 0 );
    if ( info.SocketDescriptor < 0 )
      throw C2SHttpClientException( "C2SHttpClient::send: ERROR opening socket" );

    server = gethostbyname( m_sHost.c_str() );
    if ( server == NULL )
      throw C2SHttpClientException( "C2SHttpClient::send: ERROR, no such host" );

    bzero( reinterpret_cast<char *>( &serverAddress ) , sizeof( serverAddress ) );
    serverAddress.sin_family = AF_INET;
    bcopy( ( char * ) server->h_addr , ( char * ) &serverAddress.sin_addr.s_addr , server->h_length );

    serverAddress.sin_port = htons( m_iPortNumber );
    if ( connect( info.SocketDescriptor , ( struct sockaddr *) &serverAddress , sizeof( serverAddress ) ) < 0 )
      throw C2SHttpClientException( "C2SHttpClient::send: ERROR connecting to socket" );

#endif //WINXX

    std::string sRequestData = request.header().toString();

    if ( request.header().Fields.getContentLength() )
    {
      if ( request.header().Method == GET )
        throw C2SHttpException( "C2SHttpClient::send: " , "No content allowed for GET request" , BadRequest );

      const C2SHttpEntity &entity = request.entity();
      sRequestData += std::string( entity.data , entity.size );
    }

    //send header
    C2SHttpClient::writeToSocket( sRequestData.c_str() , sRequestData.size() , info );

    //writing to socket is finished
    if ( shutdown( info.SocketDescriptor , SHUT_WR ) )
      throw C2SHttpClientException( "C2SHttpClient::send: Cannot shutdown socket!" );

    C2SHttpResponse *pServerResponse = new C2SHttpResponse();

    //initialize data buffer
    char buffer[ BUFFERSIZE ];
#ifndef WINXX
    bzero( buffer , BUFFERSIZE );
#endif

    long iBytesRead = BUFFERSIZE;
    while ( iBytesRead == BUFFERSIZE )
    {

      //read data from buffer
#ifdef WINXX
      iBytesRead = recv( info.SocketDescriptor , buffer , BUFFERSIZE , MSG_PARTIAL );
#else
      iBytesRead = read( info.SocketDescriptor , buffer , BUFFERSIZE );
#endif
      if ( iBytesRead < 0 )
        throw C2SHttpClientException( "C2SHttpClient::send: Error reading from socket!" );

      pServerResponse->push( buffer , iBytesRead );
    }

#ifdef WINXX
    closesocket( info.SocketDescriptor );
#else
    close( info.SocketDescriptor );
#endif

    pServerResponse->finished();

    return pServerResponse;
  }

  void C2SHttpClient::writeToSocket( const char *data , unsigned int iSize , const C2SSocketInfo &info )
  {
#ifdef WINXX
    int n = ::send( info.SocketDescriptor , data , iSize , 0 );
#else
    int iBytesWritten = 0;
    while ( iBytesWritten < static_cast<int>( iSize ) )
    {
      const char *pDataLeft = &data[ iBytesWritten ];
      unsigned int iSizeLeft = iSize - iBytesWritten;
      iBytesWritten += write( info.SocketDescriptor , pDataLeft , iSizeLeft );
      if ( iBytesWritten < 0 )
        throw C2SHttpClientException( "C2SHttpClient::writeToSocket: ERROR writing to socket" );
    }
#endif

  }

}
