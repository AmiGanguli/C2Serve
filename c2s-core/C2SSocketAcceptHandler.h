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

#ifndef C2SSOCKETACCEPTHANDLER_H_
#define C2SSOCKETACCEPTHANDLER_H_

#include "C2SException.h"
#include "C2SSocketInfo.h"
#include "C2SDataPushInterface.h"

#include "Mutex.h"

namespace c2s
{
  class C2SDataPullInterface;
  class C2SDataHandlingInterface;

  class C2SSocketAcceptHandlerException : public C2SException
  {
  public:

    C2SSocketAcceptHandlerException( const std::string &msg ) : C2SException( msg ) {};

  };

  class C2SSocketAcceptHandlerDataPushImpl : public C2SDataPushInterface
  {
  public:

    virtual void push( const char *data , unsigned int size );

    void setSocketInfo( const C2SSocketInfo &socketInfo ) { m_socketInfo = socketInfo; }

  private:

    C2SSocketInfo m_socketInfo;

  };

  class C2SSocketAcceptHandler
  {
  public:

    C2SSocketAcceptHandler( const C2SSocketInfo &socketInfo , const C2SDataHandlingInterface &dataHandling , c2s::thread::Mutex *pAcceptMutex );

    virtual ~C2SSocketAcceptHandler();

    void run() { this->accept(); }

    void interrupt();

  private:

    void accept();

    C2SSocketAcceptHandler( const C2SSocketAcceptHandler & );

    C2SSocketAcceptHandler &operator=( const C2SSocketAcceptHandler & );

    C2SSocketInfo m_socketInfo;

    C2SSocketInfo m_connectionSocketInfo;

    C2SDataPullInterface *m_pDataPull;

    C2SSocketAcceptHandlerDataPushImpl m_dataPush;

    bool m_bInterrupted;

    c2s::thread::Mutex &m_acceptMutex;

  };

}

#endif /* C2SSOCKETACCEPTHANDLER_H_ */
