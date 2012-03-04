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

#ifndef THREADQUEUE_H_
#define THREADQUEUE_H_

#include "Lock.h"
#include "Thread.h"

#include <list>

namespace c2s
{

  namespace thread
  {

    template <class Runnable> class ThreadQueue;

    class ThreadQueueException : public ThreadException
    {
    public:

      ThreadQueueException( const std::string &msg ) : ThreadException( msg ) {};

    };

    template <class Runnable>
    class ThreadQueue
    {
    public:

      ThreadQueue();

      virtual ~ThreadQueue();

      void queue( Runnable *pRunnable );

      //start next available thread
      void start();

      void join();

    private:

      class ThreadQueueRunner
      {
      public:

        ThreadQueueRunner( Runnable *pRunnable , ThreadQueue<Runnable> *pQueue )
          : m_runnable( *pRunnable ),
            m_pQueue( pQueue )
        {
          m_pThread = new Thread<ThreadQueueRunner>( this );
        };
            
        ~ThreadQueueRunner()
        {
          delete m_pThread;
        }

        void run();

        //TODO: Exception handling
        void start();

      private:

        ThreadQueueRunner( const ThreadQueueRunner &r );

        ThreadQueueRunner &operator=( const ThreadQueueRunner &r );

        Runnable &m_runnable;

        Thread<ThreadQueueRunner> *m_pThread;

        ThreadQueue<Runnable> *m_pQueue;

        Mutex m_mutex;

      };

    protected:

      friend class ThreadQueueRunner;

      void reregister( ThreadQueueRunner *pRunner );

    private:

      std::list<ThreadQueueRunner*> m_threads;

      unsigned int m_iSize;

      Mutex m_writeMutex;

      Mutex m_occupiedMutex;

      Mutex m_busyMutex;

#ifdef DEBUG
      bool m_bLocked;
#endif

    };

    template <class Runnable>
    ThreadQueue<Runnable>::ThreadQueue()
      : m_iSize( 0 )
#ifdef DEBUG
        , m_bLocked( false )
#endif
    {

    }

    template <class Runnable>
    ThreadQueue<Runnable>::~ThreadQueue()
    {
      typename std::list<ThreadQueueRunner*>::iterator it = m_threads.begin();
      for ( ; it != m_threads.end(); ++it )
        delete ( *it );
    }

    template <class Runnable>
    void ThreadQueue<Runnable>::queue( Runnable *pRunnable )
    {
      Lock<Mutex> writeLock( &m_writeMutex );
      ++m_iSize;
      m_threads.push_back( new ThreadQueueRunner( pRunnable , this ) );
    }

    template <class Runnable>
    void ThreadQueue<Runnable>::start()
    {
      //TODO: This won't block on windows
      m_occupiedMutex.lock();

      Lock<Mutex> writeLock( &m_writeMutex );

#ifdef DEBUG
      assert( !m_bLocked );
      m_bLocked = true;
#endif

      if ( !m_threads.size() )
      {
#ifdef DEBUG
        m_bLocked = false;
#endif
        m_occupiedMutex.unlock();
        throw ThreadQueueException( "ThreadQueue::start: Thread queue is empty!" );
      }

      assert( m_threads.size() <= m_iSize );

      if ( m_iSize == m_threads.size() )
        m_busyMutex.lock();

      ThreadQueueRunner &runner = *( m_threads.front() );
      m_threads.pop_front();

      if ( m_threads.size() )
        m_occupiedMutex.unlock();

#ifdef DEBUG
      m_bLocked = false;
#endif

      runner.start();
    }

    template <class Runnable>
    void ThreadQueue<Runnable>::join()
    {
      Lock<Mutex> lock( &m_busyMutex );
    }

    template <class Runnable>
    void ThreadQueue<Runnable>::reregister( ThreadQueueRunner *pRunner )
    {
      Lock<Mutex> writeLock( &m_writeMutex );

#ifdef DEBUG
      assert( !m_bLocked );
      m_bLocked = true;
#endif

      bool bUnlockOccupied = false;
      if ( !m_threads.size() )
        bUnlockOccupied = true;

      m_threads.push_back( pRunner );

#ifdef DEBUG
      m_bLocked = false;
#endif

      if ( bUnlockOccupied )
        m_occupiedMutex.unlock();

      if ( m_iSize == m_threads.size() )
        m_busyMutex.unlock();
    }

    template <class Runnable>
    void ThreadQueue<Runnable>::ThreadQueueRunner::start()
    {
      m_mutex.lock();
      m_mutex.unlock();

      if ( !m_pQueue )
        throw ThreadQueueException( "ThreadQueueRunner::start: Parent thread queue is not set!" );

      m_pThread->start();
    }

    template <class Runnable>
    void ThreadQueue<Runnable>::ThreadQueueRunner::run()
    {
      Lock<Mutex> lock( &m_mutex );

      m_runnable.run();
      assert( m_pQueue );
      m_pQueue->reregister( this );
    }

  }

}

#endif /* THREADQUEUE_H_ */
