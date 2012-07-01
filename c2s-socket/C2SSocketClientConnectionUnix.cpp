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

#include "C2SSocketClientConnectionUnix.h"
#include "C2SSocketClientException.h"
#include "C2SSocketInfo.h"

#include <cstring>

namespace c2s
{

  C2SSocketClientConnectionUnix::C2SSocketClientConnectionUnix()
  {
  }

  C2SSocketClientConnectionUnix::~C2SSocketClientConnectionUnix()
  {
  }

  int C2SSocketClientConnectionUnix::sendNextChunkOfDataToSocket( const char *pDataToWriteToSocket , unsigned int iDataLengthToWriteToSocket )
  {
    return write( m_pSocketInfo->SocketDescriptor , pDataToWriteToSocket , iDataLengthToWriteToSocket );
  }

  int C2SSocketClientConnectionUnix::receiveNextChunkOfDataFromSocket( char *pBufferToWriteDataReadFromSocket , unsigned int iBufferSize )
  {
    return read( m_pSocketInfo->SocketDescriptor , pBufferToWriteDataReadFromSocket , iBufferSize );
  }

  void C2SSocketClientConnectionUnix::shutdownSendOperations()
  {
    if ( shutdown( m_pSocketInfo->SocketDescriptor , SHUT_WR ) )
      throw C2SSocketClientException( "C2SSocketClientConnectionUnix::shutdownSendOperations: Cannot shutdown socket!" );
  }

  void C2SSocketClientConnectionUnix::connectSocket( const std::string &sHost , unsigned short iPort )
  {
    m_pSocketInfo->SocketDescriptor = socket( AF_INET , SOCK_STREAM , 0 );

    struct sockaddr_in serverAddress;
    struct hostent *server;

    if ( m_pSocketInfo->SocketDescriptor < 0 )
      throw C2SSocketClientException( "C2SSocketClientConnectionUnix::connectToSocket: ERROR opening socket" );

    server = gethostbyname( sHost.c_str() );
    if ( server == NULL )
      throw C2SSocketClientException( "C2SSocketClientConnectionUnix::connectToSocket: ERROR, no such host" );

    bzero( reinterpret_cast<char *>( &serverAddress ) , sizeof( serverAddress ) );
    serverAddress.sin_family = AF_INET;
    bcopy( ( char * ) server->h_addr , ( char * ) &serverAddress.sin_addr.s_addr , server->h_length );

    serverAddress.sin_port = htons( iPort );
    if ( connect( m_pSocketInfo->SocketDescriptor , ( struct sockaddr *) &serverAddress , sizeof( serverAddress ) ) < 0 )
      throw C2SSocketClientException( "C2SSocketClientConnectionUnix::connectToSocket: ERROR connecting to socket" );
  }

  void C2SSocketClientConnectionUnix::closeSocket()
  {
    close( m_pSocketInfo->SocketDescriptor );
  }

}
