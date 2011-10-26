/*
 * globaldefines.h
 *
 *  Created on: 26.10.2009
 *      Author: gack
 */

#ifndef GLOBALDEFINES_H_
#define GLOBALDEFINES_H_

#ifndef WINXX

#include <inttypes.h>
#include <unistd.h>

#else

  #include <windows.h>

#endif

namespace g
{

#ifndef WINXX

  typedef int64_t int64;
  typedef int32_t int32;

  typedef uint64_t uint64;
  typedef uint32_t uint32;

  inline void m_sleep( unsigned int msec ) { usleep( msec * 1000 ); }

#else

  #include <windows.h>

  typedef unsigned __int32 uint32;
  typedef unsigned __int64 uint64;

  typedef __int32 int32;
  typedef __int64 int64;

  typedef int ssize_t;

  inline void m_sleep( unsigned int msec ) { Sleep( msec ); }

#endif

}


#endif /* GLOBALDEFINES_H_ */
