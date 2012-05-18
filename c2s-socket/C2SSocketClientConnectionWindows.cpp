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

#include "C2SSocketClientConnectionWindows.h"
#include "C2SSocketClientException.h"
#include "C2SSocketInfo.h"
#include "StringUtils.h"

namespace c2s
{

  C2SSocketClientConnectionWindows::C2SSocketClientConnectionWindows()
  {
    WSADATA wsa;
    int ret;

    //initialize winsock
    //use version 2.0
    //struct wsa will be filled with information about winsock
    ret = WSAStartup( MAKEWORD( 2 , 0 ) , &wsa );
    if ( ret != 0 )
      throw C2SSocketClientException( "C2SSocketClientConnectionWindows::C2SSocketClientConnectionWindows: Could not initialize winsock! Error: " + c2s::util::toString( ret ) );
  }

  C2SSocketClientConnectionWindows::~C2SSocketClientConnectionWindows()
  {
    WSACleanup();
  }
  int C2SSocketClientConnectionWindows::sendNextChunkOfDataToSocket( const char *pDataToWriteToSocket , unsigned int iDataLengthToWriteToSocket )
  {
    return send( m_pSocketInfo->SocketDescriptor , pDataToWriteToSocket , iDataLengthToWriteToSocket , 0 );
  }

  int C2SSocketClientConnectionWindows::receiveNextChunkOfDataFromSocket( char *pBufferToWriteDataReadFromSocket , unsigned int iBufferSize )
  {
    return recv( m_pSocketInfo->SocketDescriptor , pBufferToWriteDataReadFromSocket , iBufferSize , MSG_PARTIAL );
  }

  void C2SSocketClientConnectionWindows::shutdownSendOperations()
  {
    if ( shutdown( m_pSocketInfo->SocketDescriptor , SD_SEND ) )
      throw C2SSocketClientException( "C2SSocketClientConnectionWindows::shutdownSendOperations: Cannot shutdown socket! Error: " + c2s::util::toString( WSAGetLastError() ) );
  }

  void C2SSocketClientConnectionWindows::connectSocket( const std::string &sHost , unsigned short iPort )
  {
    m_pSocketInfo->SocketDescriptor = socket( AF_INET , SOCK_STREAM , 0 );
    if ( m_pSocketInfo->SocketDescriptor == INVALID_SOCKET )
      throw C2SSocketClientException( "C2SSocketClientConnectionWindows::connectSocket: Could not create socket! Error: " + c2s::util::toString( WSAGetLastError() ) );

    //TODO: release addr??
    SOCKADDR_IN addr;
    memset( &addr , 0 , sizeof( SOCKADDR_IN ) );
    addr.sin_family = AF_INET;
    addr.sin_port = htons( static_cast<u_short>( iPort ) );
    addr.sin_addr.s_addr = inet_addr( sHost.c_str() );

    unsigned long ip = inet_addr( sHost.c_str() );
    if( ip != INADDR_NONE )
      addr.sin_addr.s_addr = ip;
    else
    {
      // resolve host name to ip
      //TODO: release he??
      HOSTENT* he = gethostbyname( sHost.c_str() );
      if( he == NULL )
        throw C2SSocketClientException( "C2SSocketClientConnectionWindows::connectSocket: Cannot resolve host: " + sHost );

      memcpy( &( addr.sin_addr ) , he->h_addr_list[ 0 ] , 4 );
    }

    int ret = connect( m_pSocketInfo->SocketDescriptor , ( SOCKADDR* )&addr , sizeof( SOCKADDR ) );

    if( ret == SOCKET_ERROR )
      throw C2SSocketClientException( "C2SSocketClientConnectionWindows::connectSocket: Could not connect to server: " + sHost + "; Port: " + c2s::util::toString( iPort ) + "! Error: " + c2s::util::toString( WSAGetLastError() ) );
  }

  void C2SSocketClientConnectionWindows::closeSocket()
  {
    closesocket( m_pSocketInfo->SocketDescriptor );
  }

}
