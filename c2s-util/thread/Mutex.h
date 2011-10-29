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

namespace c2s
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
