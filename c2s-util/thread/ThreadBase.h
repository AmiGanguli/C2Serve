/*
 * threadbase.h
 *
 *  Created on: 18.10.2009
 *      Author: gack
 */

#ifndef THREADBASE_H_
#define THREADBASE_H_

#include "Thread.h"

namespace g
{
  namespace thread
  {

    class ThreadBase
    {
    public:
      virtual ~ThreadBase(){};

      void operator()() { this->run(); }

      virtual void run() = 0;

      void start() { m_thread.start(); }

    protected:
      ThreadBase() { m_thread.set( *this ); }

    private:
      Thread<ThreadBase> m_thread;
    };

  }
}

#endif /* THREADBASE_H_ */
