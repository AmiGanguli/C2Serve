/*
 * FileExistsException.h
 *
 *  Created on: 09.01.2011
 *      Author: gack
 */

#ifndef FILEEXISTSEXCEPTION_H_
#define FILEEXISTSEXCEPTION_H_

#include "FileUtilsException.h"

namespace g
{

  namespace util
  {

    /** @brief  Exception thrown if an error is caused by the existance of a file. */
    class FileExistsException : public FileUtilsException
    {
    public:

      /**
       * @brief   Constructor.
       * @param   msg Error message to throw.
       */
      FileExistsException( const std::string &msg ) : FileUtilsException( msg ) {};

    };

  }

}

#endif /* FILEEXISTSEXCEPTION_H_ */
