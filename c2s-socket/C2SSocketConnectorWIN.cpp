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

#include "C2SSocketConnectorWIN.h"
#include "C2SGlobalSettings.h"

#include "StringUtils.h"

#define BACKLOG_QUEUE_SIZE 5

namespace c2s
{

  C2SSocketConnectorWIN::C2SSocketConnectorWIN()
  {
  }

  C2SSocketConnectorWIN::~C2SSocketConnectorWIN()
  {
  }

  ForRestSocketInfo C2SSocketConnectorWIN::connect()
  {
    ForRestSocketInfo info;
    WSADATA wsa;
    int ret;

    m_pLogger->debug( "Create socket" );

    //initialize winsock
    //use version 2.0
    //struct wsa will be filled with information about winsock
    ret = WSAStartup( MAKEWORD( 2 , 0 ) , &wsa );
    if ( ret != 0 )
      throw C2SSocketConnectorException( "C2SSocketConnectorWIN::connect: Could not initialize winsock! Error: " + c2s::utils::toString( ret ) );

    SOCKADDR_IN addr;

    info.SocketDescriptor = socket( AF_INET , SOCK_STREAM , 0 );
    if ( info.SocketDescriptor == INVALID_SOCKET )
      throw C2SSocketConnectorException( "C2SSocketConnectorWIN::connect: Could not create socket! Error: " + c2s::utils::toString( WSAGetLastError() ) );

    m_pLogger->debug( "Bind socket; port: " + c2s::utils::toString( C2SGlobalSettings::Settings().iPortNumber ) );

    memset( &addr , 0 , sizeof( SOCKADDR_IN ) );
    addr.sin_family = AF_INET;
    addr.sin_port = htons( C2SGlobalSettings::Settings().iPortNumber );
    addr.sin_addr.s_addr = ADDR_ANY;

    ret = bind( info.SocketDescriptor , ( SOCKADDR* ) &addr , sizeof( SOCKADDR_IN ) );
    if( ret == SOCKET_ERROR )
      throw C2SSocketConnectorException( "C2SSocketConnectorWIN::connect: Could not bind socket! Error: " + c2s::utils::toString( WSAGetLastError() ) );

    m_pLogger->debug( "Create listener; size of backlog queue: " + c2s::utils::toString<int>( BACKLOG_QUEUE_SIZE ) );

    ret = listen( info.SocketDescriptor , BACKLOG_QUEUE_SIZE );
    if ( ret == SOCKET_ERROR )
      throw C2SSocketConnectorException( "C2SSocketConnectorWIN::connect: Could not create listener! Error: " + c2s::utils::toString( WSAGetLastError() ) );

    return info;
  }

}
