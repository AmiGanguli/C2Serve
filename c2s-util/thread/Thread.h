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

#ifndef THREAD_H_
#define THREAD_H_

#include "ThreadDefines.h"
#include "ThreadException.h"
#include "Lock.h"

#ifdef USE_BOOST_THREADS
#include <boost/thread.hpp>
#endif

#ifdef USE_POSIX_THREADS
#include <pthread.h>
#endif

#include <assert.h>

namespace c2s
{
  namespace thread
  {

    template <class Runnable>
    class Thread
    {
    public:

      Thread( Runnable *runnable )
        : m_runnable( runnable ),
          m_pMutex( new Mutex ),
          m_bDestroyMutex( true )
#ifdef USE_POSIX_THREADS
          , m_pthread( 0 )
#endif
      {};

      Thread()
        : m_runnable( NULL ),
          m_pMutex( new Mutex ),
          m_bDestroyMutex( true )
#ifdef USE_POSIX_THREADS
          , m_pthread( 0 )
#endif
      {};

      void set( Runnable &runnable ) { m_runnable = &runnable; }

      virtual ~Thread()
      {
#ifdef USE_POSIX_THREADS
        if ( m_pthread )
          pthread_detach( m_pthread );
#endif
        if ( m_bDestroyMutex )
          delete m_pMutex;
      };

      inline void start();

#ifdef USE_BOOST_THREADS

      void operator()();

      Thread( const Thread &t );

#endif

    private:

      void run();

      Thread &operator=( const Thread &t );

      Runnable *m_runnable;

      Mutex *m_pMutex;

      bool m_bDestroyMutex;

#ifdef USE_POSIX_THREADS

      Thread( const Thread &t );

      void operator()();

      static void *run( void *data );

      pthread_t m_pthread;

#endif //USE_POSIX_THREADS

    };

    template <class Runnable>
    Thread<Runnable>::Thread( const Thread &t )
      : m_runnable( t.m_runnable ),
        m_pMutex( t.m_pMutex ),
        m_bDestroyMutex( false )
    {}

    template <class Runnable>
    inline void Thread<Runnable>::run()
    {
#ifdef USE_BOOST_THREADS
      Lock<Mutex> lock( m_pMutex );
#endif

      assert( m_runnable != NULL );
      m_runnable->run();
    }

    template <class Runnable>
    inline void Thread<Runnable>::operator()()
    {
      this->run();
    }

#ifdef USE_POSIX_THREADS

    template <class Runnable>
    void *Thread<Runnable>::run( void *data )
    {
      Thread *pThread = reinterpret_cast<Thread*>( data );

      {
        Lock<Mutex> lock( pThread->m_pMutex );

        pThread->run();
      }

      pthread_exit( NULL );
    }

    template <class Runnable>
    inline void Thread<Runnable>::start()
    {
      if ( m_pthread )
      {
        Lock<Mutex> lock( m_pMutex );

        int status = pthread_detach( m_pthread );
        if ( status )
          throw ThreadException( "Thread::start: Cannot detach thread" );
      }

      int status = pthread_create( &m_pthread , NULL , Thread::run , (void *) this );
      if ( status )
        throw ThreadException( "Thread::start: Cannot create thread" );
    }

#endif //USE_POSIX_THREADS

#ifdef USE_BOOST_THREADS

    template <class Runnable>
    inline void Thread<Runnable>::start()
    {
      Lock<Mutex> lock( m_pMutex );

      boost::thread athread( *this );
    }

#endif //USE_BOOST_THREADS

  }
}


#endif /* THREAD_H_ */
