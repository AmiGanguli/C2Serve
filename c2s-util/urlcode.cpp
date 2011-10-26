/**

  @brief  Url encoding and decoding of strings.
          Thanks to http://www.geekhideout.com/.

 */

#include "urlcode.h"

#include <cstring>
#include <cstdlib>
#include <cctype>

/**
 *
 * Converts a hex character to its integer value.
 *
 * @param   ch  Hex character to be converted.
 * @return  According integer value.
 *
 */
char from_hex(char ch)
{
  return std::isdigit(ch) ? ch - '0' : std::tolower(ch) - 'a' + 10;
}

/**
 *
 * Converts an integer value to its hex character.
 *
 * @param   ch  Integer to be converted.
 * @return  According hex value.
 *
 */
char to_hex(char code)
{
  static char hex[] = "0123456789abcdef";
  return hex[code & 15];
}

/**
 *
 * Returns a url-encoded version of str.
 * Make sure to free the returned string after use.
 *
 * @param   str   Original string.
 * @return  Url encoded string.
 *
 */
char *url_encode( const char *str )
{
  const char *pstr = str;
  char *buf = (char*)malloc(strlen(str) * 3 + 1), *pbuf = buf;
  while (*pstr) {
    if (isalnum(*pstr) || *pstr == '-' || *pstr == '_' || *pstr == '.' || *pstr == '~')
      *pbuf++ = *pstr;
    else if (*pstr == ' ')
      *pbuf++ = '+';
    else
      *pbuf++ = '%', *pbuf++ = to_hex(*pstr >> 4), *pbuf++ = to_hex(*pstr & 15);
    pstr++;
  }
  *pbuf = '\0';
  return buf;
}

/**
 *
 * Returns a url-decoded version of str.
 * Make sure to free the returned string after use.
 *
 * @param   str   Url encoded string.
 * @return  Decoded string.
 *
 */
char *url_decode( const char *str )
{
  const char *pstr = str;
  char *buf = (char*)malloc(strlen(str) + 1), *pbuf = buf;
  while (*pstr) {
    if (*pstr == '%') {
      if (pstr[1] && pstr[2]) {
        *pbuf++ = from_hex(pstr[1]) << 4 | from_hex(pstr[2]);
        pstr += 2;
      }
    } else if (*pstr == '+') {
      *pbuf++ = ' ';
    } else {
      *pbuf++ = *pstr;
    }
    pstr++;
  }
  *pbuf = '\0';
  return buf;
}
