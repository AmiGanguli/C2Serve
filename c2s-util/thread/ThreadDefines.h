/**

 		ThreadDefines.h

		Created on: 	30.01.2011
	  Author: 			Kay Wolter
		E-Mail:				wolter.kay@freenet.de

		@brief	Add description here!!!

 */

#ifndef THREADDEFINES_H_
#define THREADDEFINES_H_

#ifndef USE_BOOST_THREADS

#if defined LINUX || defined DARWIN

#ifndef USE_POSIX_THREADS
#define USE_POSIX_THREADS
#endif //USE_POSIX_THREADS

#else //LINUX || DARWIN

#ifndef USE_BOOST_THREADS
#define USE_BOOST_THREADS
#endif //USE_BOOST_THREADS

#endif //LINUX || DARWIN

#endif //USE_BOOST_THREADS

#endif /* THREADDEFINES_H_ */
