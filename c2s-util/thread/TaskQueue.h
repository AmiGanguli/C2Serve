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

#ifndef TASKQUEUE_H_
#define TASKQUEUE_H_

#include "ThreadQueue.h"

namespace c2s
{
  namespace thread
  {

    class TaskBase
    {
    public:

      TaskBase(){};

      virtual ~TaskBase(){};

      virtual void run() = 0;

    };

    class TaskQueue;

    class TaskQueueRunnable
    {
    public:

      TaskQueueRunnable( TaskQueue *pQueue )
        : m_pQueue( pQueue )
      {};

      inline void run();

    private:

      TaskQueue *m_pQueue;

    };

    class TaskQueue
    {
    public:

      TaskQueue( unsigned int iNumThreads )
        : m_bIsRunning( false )
      {
        for ( unsigned int i = 0; i < iNumThreads; ++i )
        {
          m_runnables.push_back( TaskQueueRunnable( this ) );
          m_workers.queue( &( m_runnables.back() ) );
        }

        m_pControllerThread = new Thread<TaskQueue>( this );
      }

      virtual ~TaskQueue()
      {
        this->join();
        delete m_pControllerThread;
      }

      void join()
      {
        Lock lock( &m_runningMutex );
        m_workers.join();
      }

      void queue( TaskBase *task )
      {
        Lock lock( &m_mutex );
        m_tasks.push_back( task );
        if ( !m_bIsRunning )
        {
          m_runningMutex.lock();
          m_bIsRunning = true;
          m_pControllerThread->start();
        }
      }

      void run()
      {
        volatile bool bContinue = true;

        while( bContinue )
        {
          m_detachMutex.lock();

          if ( !m_tasks.size() )
          {
            m_detachMutex.unlock();
            break;
          }

          m_workers.start();
        }

        m_bIsRunning = false;
        m_runningMutex.unlock();
      }

    protected:

      friend class TaskQueueRunnable;

      TaskBase *detach()
      {
        Lock lock( &m_mutex );

        assert( m_tasks.size() );

        TaskBase *pNextTask = *( m_tasks.begin() );
        m_tasks.pop_front();

        m_detachMutex.unlock();

        return pNextTask;
      }

    private:

      std::list<TaskQueueRunnable> m_runnables;

      std::list<TaskBase*> m_tasks;

      ThreadQueue<TaskQueueRunnable> m_workers;

      Thread<TaskQueue> *m_pControllerThread;

      volatile bool m_bIsRunning;

      Mutex m_mutex;

      Mutex m_runningMutex;

      Mutex m_detachMutex;

    };

    inline void TaskQueueRunnable::run()
    {
      TaskBase *pTask = m_pQueue->detach();
      pTask->run();
    }

  }
}

#endif /* TASKQUEUE_H_ */
