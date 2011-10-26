/**

 		Lock.h
		
		Created on: 	30.01.2011
	  Author: 			Kay Wolter
		E-Mail:				wolter.kay@freenet.de

		@brief	Add description here!!!

 */

#ifndef LOCK_H_
#define LOCK_H_

#include "Mutex.h"

namespace g
{

  namespace thread
  {

    class Lock
    {
    public:

      Lock( Mutex *pMutex )
        : m_mutex( *pMutex )
      {
        m_mutex.lock();
      }

      virtual ~Lock()
      {
        m_mutex.unlock();
      }

    private:

      Lock( const Lock &lock );

      Lock &operator=( const Lock &lock );

      Mutex &m_mutex;

    };

  }

}

#endif /* LOCK_H_ */
