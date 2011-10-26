#ifndef STRINGUTILS_H_
#define STRINGUTILS_H_

#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

namespace g
{
  namespace util
  {

    template <class ElementT>
    inline std::string toString( ElementT val )
    {
      std::stringstream s;
      s << val;
      return s.str();
    }

    template <class ElementT>
    inline ElementT toNumber( const std::string &s )
    {
      std::stringstream str( s );
      ElementT val;
      str >> val;
      return val;
    }

    std::string urlEncode( const std::string &s );

    std::string urlDecode( const std::string &s );

    bool substract( const std::string &from , const std::string &what , std::string *dest );

    std::string substract( const std::string &from , const std::string &what );

    std::vector<std::string> splitString( const std::string& s , const char sSep , bool bDiscardEmptyStrings = false );

    std::string extractExtension( const std::string &s );

    std::string extractBaseName( const std::string &s );

    unsigned int removeChar( std::string *from , char what );

    inline void toLowerCaseInplace( std::string &s )
    {
      std::transform( s.begin() , s.end() , s.begin() , ::tolower );
    }

    inline std::string toLowerCase( const std::string &s )
    {
      std::string ls = s;
      toLowerCaseInplace( ls );
      return ls;
    }

  }
}

#endif /*STRINGUTILS_H_*/
