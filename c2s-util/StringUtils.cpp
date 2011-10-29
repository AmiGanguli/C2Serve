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

#include "StringUtils.h"
#include "GenericException.h"

#include "urlcode.h"

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

namespace g
{
  namespace util
  {

    std::string urlEncode( const std::string &s )
    {
      char *cr = url_encode( s.c_str() );
      std::string r = cr;
      std::free( cr );
      return r;
    }

    std::string urlDecode( const std::string &s )
    {
      char *cr = url_decode( s.c_str() );
      std::string r = cr;
      std::free( cr );
      return r;
    }

    std::vector<std::string> splitString( const std::string& s , const char sSep , bool bDiscardEmptyStrings )
    {
      std::vector<std::string> vStrings;

      for (unsigned int i=0;i<s.size();)
      {
        std::string newString="";
        while (i<s.size() && s[i]==sSep)
          ++i;
        while (i<s.size() && s[i]!=sSep)
          newString+=s[i++];

        if ( !bDiscardEmptyStrings || newString.size() )
          vStrings.push_back(newString);
        ++i;
      }
      return vStrings;
    }

    std::string extractExtension(const std::string& s)
    {
      std::string ext;
      unsigned int pos = s.rfind(".");
      if (pos!=s.size())
        ext = s.substr(pos+1,s.size());
      return ext;
    }

    std::string extractBaseName( const std::string &s )
    {
      std::string basename;
      //TODO: check windows
      unsigned int pos = s.rfind( "/" );
      if ( pos!=s.size() )
        basename = s.substr( pos+1 , s.size() );
      return basename;
    }

    std::string substract( const std::string &from , const std::string &what )
    {
      std::string s;
      if ( !substract( from , what , &s ) )
        throw GenericException( "gutils::substract() Cannot substract \"" + what + "\" from \"" + from + "\"!" );
      return s;
    }

    bool substract( const std::string &from , const std::string &what , std::string *dest )
    {
      std::size_t pos = from.find( what );
      if ( pos == std::string::npos )
        return false;

      std::string sStart = from.substr( 0 , pos );
      std::string sEnd = from.substr( pos + what.size() , from.size() - 1 );
      ( *dest ) = sStart + sEnd;

      return true;
    }

    unsigned int removeChar( std::string *from , char what )
    {
      std::string to;
      unsigned int iNumCharsFound = 0;
      for ( unsigned int i = 0; i < from->size(); ++i )
      {
        if ( from->operator[]( i ) != what )
          to += from->operator[]( i );
        else
          ++iNumCharsFound;
      }

      *from = to;
      return iNumCharsFound;
    }

  }
}
