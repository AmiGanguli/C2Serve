/*
 * urlcode.h
 *
 *  Created on: 08.06.2011
 *      Author: gack
 */

#ifndef URLCODE_H_
#define URLCODE_H_

/**
 *
 * Returns a url-encoded version of str.
 * Make sure to free the returned string after use.
 *
 * @param   str   Original string.
 * @return  Url encoded string.
 *
 */
char *url_encode( char const *str );

/**
 *
 * Returns a url-decoded version of str.
 * Make sure to free the returned string after use.
 *
 * @param   str   Url encoded string.
 * @return  Decoded string.
 *
 */
char *url_decode( char const *str );

#endif /* URLCODE_H_ */
