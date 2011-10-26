/**

 		Mutex.h
		
		Created on: 	28.01.2011
	  Author: 			Kay Wolter
		E-Mail:				wolter.kay@freenet.de

		@brief	Add description here!!!

 */

#ifndef MUTEX_H_
#define MUTEX_H_

#include "ThreadDefines.h"
#include "GenericException.h"

#ifdef USE_POSIX_THREADS
#include <pthread.h>
#endif

#ifdef USE_BOOST_THREADS
#include <boost/thread.hpp>
#endif

#include <iostream>

namespace g
{

  namespace thread
  {

#ifdef USE_BOOST_THREADS

    typedef boost::mutex Mutex;

#endif //USE_BOOST_THREADS

#ifdef USE_POSIX_THREADS

    class MutexException : public GenericException
    {
    public:

      MutexException( const std::string &msg ) : GenericException( msg ) {};

    };

    //TODO: doesn't work on forrest
    class Mutex
    {
    public:

      Mutex();

      ~Mutex();

      inline void lock();

      int trylock();

      inline void unlock();

    private:

      Mutex( const Mutex &mutex );

      Mutex &operator=( const Mutex &mutex );

      pthread_mutex_t m_mutex;

    };

    inline Mutex::Mutex()
    {
      int status = pthread_mutex_init( &m_mutex , NULL );
      if ( status )
        throw MutexException( "Mutex::Mutex: Cannot initialize mutex!" );
    };

    inline void Mutex::lock()
    {
      int status = pthread_mutex_lock( &m_mutex );
      if ( status )
        throw MutexException( "Mutex::lock: Cannot lock mutex!" );
    }

    inline void Mutex::unlock()
    {
      int status = pthread_mutex_unlock( &m_mutex );
      if ( status )
        throw MutexException( "Mutex::unlock: Cannot unlock mutex!" );
    }

    inline Mutex::~Mutex()
    {
      int status = pthread_mutex_destroy( &m_mutex );
      if ( status )
        std::cerr << "Mutex::~Mutex: Error destroying mutex!" << std::endl;
    };

    inline int Mutex::trylock()
    {
      return pthread_mutex_trylock( &m_mutex );
    }

#endif //USE_POSIX_THREADS

  }

}

#endif /* MUTEX_H_ */
