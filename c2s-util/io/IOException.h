/*
 * IOException.h
 *
 *  Created on: 26.02.2011
 *      Author: gack
 */

#ifndef IOEXCEPTION_H_
#define IOEXCEPTION_H_

#include "GenericException.h"

namespace g
{
  namespace io
  {

    /** @brief  Exception thrown if an error is caused by an IO operation. */
    class IOException : public GenericException
    {
    public:

      /**
       * @brief   Constructor.
       * @param   msg Error message to throw.
       */
      IOException( const std::string &msg ) : GenericException( msg ) {};

    };

  }
}

#endif /* IOEXCEPTION_H_ */
