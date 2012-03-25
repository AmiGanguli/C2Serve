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

#include "C2STestSocketListenerDataPull.h"

#include <boost/test/unit_test.hpp>

namespace c2s
{
  namespace test
  {

    unsigned int C2STestSocketListenerDataPull::iNumberOfCompletedDataTransmissions = 0;

    C2STestSocketListenerDataPull::C2STestSocketListenerDataPull( C2SDataPushInterface *pDataPush , const std::string &sMessageExpectedToReadFromSocket , const std::string &sMessageToSendBackToClient )
      : C2SDataPullInterface( pDataPush ),
        m_sMessageExpectedToReadFromSocket( sMessageExpectedToReadFromSocket ),
        m_sMessageToSendBackToClient( sMessageToSendBackToClient )
    {
    }

    C2STestSocketListenerDataPull::~C2STestSocketListenerDataPull()
    {
      BOOST_CHECK( iNumberOfCompletedDataTransmissions == 2 );
    }

    void C2STestSocketListenerDataPull::reset()
    {

    }

    void C2STestSocketListenerDataPull::receive( char *data , unsigned int size )
    {
      m_sMessageReadFromSocket += std::string( data , size );
    }

    void C2STestSocketListenerDataPull::flush()
    {
      //check
      ++iNumberOfCompletedDataTransmissions;
      BOOST_MESSAGE( "Server socket received:\n" << m_sMessageReadFromSocket );
      BOOST_CHECK( m_sMessageReadFromSocket == m_sMessageExpectedToReadFromSocket );
      m_dataPush.push( m_sMessageToSendBackToClient.c_str() , m_sMessageToSendBackToClient.size() );
    }

    bool C2STestSocketListenerDataPull::isComplete() const
    {
      return false;
    }

  }
}
