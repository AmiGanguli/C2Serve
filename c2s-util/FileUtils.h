#ifndef FILETOOLS_H_
#define FILETOOLS_H_

#include <string>

namespace g
{
  namespace util
  {

    /**
     *
     * Create a directory.
     * This method throws an exception of type FileExistsException, if the directory already exists.
     * If the directory does not yet exist and it still cannot be created, an exception of type FileUtilsException is thrown.
     *
     * @param   directory   Directory to be created.
     *
     */
    void mkDir( const std::string &directory );

    /**
     *
     * Get the current working directory.
     *
     * @return  Current working directory.
     *
     */
    std::string getCWD();

  }
}

#endif /*FILETOOLS_H_*/
