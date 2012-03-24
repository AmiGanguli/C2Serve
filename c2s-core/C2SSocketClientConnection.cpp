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

namespace c2s
{

  C2SSocketClientConnection::C2SSocketClientConnection( const std::string &sHost , unsigned int iPort )
    : m_sHost( sHost ),
      m_iPort( iPort ),
      m_pSocketInfo( new C2SSocketInfo() )
  {
    this->connectSocket();
  }

  C2SSocketClientConnection::~C2SSocketClientConnection()
  {
    this->closeSocket();
    delete m_pSocketInfo;
  }

  void C2SSocketClientConnection::connectSocket()
  {
    struct sockaddr_in serverAddress;
    struct hostent *server;

    m_pSocketInfo->SocketDescriptor = socket( AF_INET , SOCK_STREAM , 0 );
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
  }

  void C2SSocketClientConnection::closeSocket()
  {
    close( m_pSocketInfo->SocketDescriptor );
  }

}
