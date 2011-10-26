/**

 		TaskQueue.h
		
		Created on: 	14.03.2011
	  Author: 			Kay Wolter
		E-Mail:				wolter.kay@freenet.de

		@brief	Add description here!!!

 */

#ifndef TASKQUEUE_H_
#define TASKQUEUE_H_

#include "ThreadQueue.h"

namespace g
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
