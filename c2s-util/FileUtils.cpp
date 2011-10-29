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
