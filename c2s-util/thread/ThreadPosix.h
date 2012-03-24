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


#ifndef THREADPOSIX_H_
#define THREADPOSIX_H_

#include "ThreadException.h"
#include "Lock.h"

#include <pthread.h>

#include <assert.h>

namespace c2s
{
  namespace thread
  {

    class ThreadPosix
    {
    public:

      ThreadPosix()
        : m_pMutex( new Mutex ),
          m_bDestroyMutex( true ),
          m_pthread( 0 )
      {};

      virtual ~ThreadPosix()
      {
        if ( m_pthread )
          pthread_detach( m_pthread );

        if ( m_bDestroyMutex )
          delete m_pMutex;
      };

      inline void start();

    protected:

      virtual void doWork() = 0;

      Mutex *m_pMutex;

    private:

      ThreadPosix &operator=( const ThreadPosix &t );

      bool m_bDestroyMutex;

      ThreadPosix( const ThreadPosix &t );

      static void *run( void *data );

      pthread_t m_pthread;

    };

    inline ThreadPosix::ThreadPosix( const ThreadPosix &t )
      : m_pMutex( t.m_pMutex ),
        m_bDestroyMutex( false )
    {}

    inline void *ThreadPosix::run( void *data )
    {
      ThreadPosix *pThreadPosix = reinterpret_cast<ThreadPosix*>( data );

      {
        Lock<Mutex> lock( pThreadPosix->m_pMutex );
        pThreadPosix->doWork();
      }

      pthread_exit( NULL );
      return NULL;
    }

    inline void ThreadPosix::start()
    {
      if ( m_pthread )
      {
        Lock<Mutex> lock( m_pMutex );

        int status = pthread_detach( m_pthread );
        if ( status )
          throw ThreadException( "ThreadPosix::start: Cannot detach thread" );
      }

      int status = pthread_create( &m_pthread , NULL , ThreadPosix::run , (void *) this );
      if ( status )
        throw ThreadException( "ThreadPosix::start: Cannot create thread" );
    }

  }
}


#endif /* THREADPOSIX_H_ */
