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

#ifndef C2SMUTEX_H_
#define C2SMUTEX_H_

#include "GenericException.h"

#include <pthread.h>

#include <iostream>

namespace c2s
{

  namespace thread
  {

    class C2SMutexException : public GenericException
    {
    public:

      C2SMutexException( const std::string &msg ) : GenericException( msg ) {};

    };

    class C2SMutex
    {
    public:

      C2SMutex();

      ~C2SMutex();

      inline void lock();

      int trylock();

      inline void unlock();

    private:

      C2SMutex( const C2SMutex &mutex );

      C2SMutex &operator=( const C2SMutex &mutex );

      pthread_mutex_t m_mutex;

    };

    inline C2SMutex::C2SMutex()
    {
      int status = pthread_mutex_init( &m_mutex , NULL );
      if ( status )
        throw C2SMutexException( "C2SMutex::C2SMutex: Cannot initialize mutex!" );
    };

    inline void C2SMutex::lock()
    {
      int status = pthread_mutex_lock( &m_mutex );
      if ( status )
        throw C2SMutexException( "C2SMutex::lock: Cannot lock mutex!" );
    }

    inline void C2SMutex::unlock()
    {
      int status = pthread_mutex_unlock( &m_mutex );
      if ( status )
        throw C2SMutexException( "C2SMutex::unlock: Cannot unlock mutex!" );
    }

    inline C2SMutex::~C2SMutex()
    {
      int status = pthread_mutex_destroy( &m_mutex );
      if ( status )
        std::cerr << "C2SMutex::~C2SMutex: Error destroying mutex!" << std::endl;
    };

    inline int C2SMutex::trylock()
    {
      return pthread_mutex_trylock( &m_mutex );
    }

  }

}

#endif /* C2SMUTEX_H_ */
