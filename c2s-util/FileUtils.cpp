/*
 * filetools.cpp
 *
 *  Created on: 16.11.2009
 *      Author: gack
 */

#include "FileUtils.h"
#include "StringUtils.h"
#include "FileExistsException.h"

#include <sys/stat.h>

#ifdef WINXX
  #include <direct.h>
#else
  #include <unistd.h>
  #include <dirent.h>
#endif


namespace g
{
  namespace util
  {

    void mkDir( const std::string &directory )
    {
#ifdef WINXX
      throw FileUtilsException( "mkDir: On windows not yet supported (directory: " + directory + ")!" );
#else
      struct stat st;
      if ( stat( directory.c_str() , &st ) == 0 && S_ISDIR( st.st_mode ) )
        throw FileExistsException( "mkDir: Directory \"" + directory + "\" already exists!" );

      int check = mkdir( directory.c_str() , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );
      if ( check != 0 )
        throw FileUtilsException( "mkDir: Cannot create directory \"" + directory + "\"!" );
#endif
    }

    std::string getCWD()
    {
#ifdef WINXX
      char* buffer;

      if( (buffer = _getcwd( NULL, 0 )) == NULL )
        throw FileUtilsException( "getCWD: Error!" );

      std::string sCWD = buffer;
      free(buffer);
      return sCWD;
#else
      char buffer[ 4096 ];
      if ( !getcwd( buffer , sizeof( buffer ) ) )
        throw FileUtilsException( "getCWD: Error!" );

      std::string sCWD = buffer;
      return sCWD;
#endif
    }

  }
}
