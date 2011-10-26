/*
 * FileUtilsException.h
 *
 *  Created on: 09.01.2011
 *      Author: gack
 */

#ifndef FILEUTILSEXCEPTION_H_
#define FILEUTILSEXCEPTION_H_

#include "GenericException.h"

namespace g
{

  namespace util
  {

    /** @brief  Exception thrown by file utils. */
    class FileUtilsException : public GenericException
    {
    public:

      /**
       * @brief   Constructor.
       * @param   msg Error message to throw.
       */
      FileUtilsException( const std::string &msg ) : GenericException( msg ) {};

    };

  }

}

#endif /* FILEUTILSEXCEPTION_H_ */
