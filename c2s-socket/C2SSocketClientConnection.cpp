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

  C2SSocketClientConnection::C2SSocketClientConnection( const std::string &sHost , unsigned short iPort )
  {
    m_pSocketInfo = new C2SSocketInfo();
    this->connectSocket( sHost , iPort );
  }

  C2SSocketClientConnection::~C2SSocketClientConnection()
  {
    this->closeSocket();
    delete m_pSocketInfo;
  }

  void C2SSocketClientConnection::writeToSocket( const char *pDataToWriteToSocket , unsigned int iDataLength )
  {
    int iBytesWrittenToSocket = 0;
    while ( iBytesWrittenToSocket < static_cast<int>( iDataLength ) )
    {
      const char *pDataLeftToWriteToSocket = &pDataToWriteToSocket[ iBytesWrittenToSocket ];
      unsigned int iDataLengthLeftToWriteToSocket = iDataLength - iBytesWrittenToSocket;
      iBytesWrittenToSocket += this->sendNextChunkOfDataToSocket( pDataLeftToWriteToSocket , iDataLengthLeftToWriteToSocket );
      if ( iBytesWrittenToSocket < 0 )
        throw C2SSocketClientException( "C2SSocketClientConnection::writeToSocket: ERROR writing to socket" );
    }
    this->shutdownSendOperations();
  }

  unsigned int C2SSocketClientConnection::readFromSocket( char *pBufferToWriteDataReadFromSocket , unsigned int iBufferSize )
  {
    long iNumberOfBytesReadFromSocket = iBufferSize;
    iNumberOfBytesReadFromSocket = this->receiveNextChunkOfDataFromSocket( pBufferToWriteDataReadFromSocket , iBufferSize );
    if ( iNumberOfBytesReadFromSocket < 0 )
      throw C2SSocketClientException( "C2SSocketClientConnection::readFromSocket: Error reading from socket!" );
    return iNumberOfBytesReadFromSocket;
  }

}
