/*
 * genericexception.h
 *
 *  Created on: 14.11.2009
 *      Author: gack
 */

#ifndef GENERICEXCEPTION_H_
#define GENERICEXCEPTION_H_

#include <string>
#include <exception>

namespace g
{

  class GenericException : public std::exception
  {
  public:
    GenericException( const std::string &msg )
      : m_msg( msg )
    {};

    virtual ~GenericException() throw() {};

    virtual const std::string &msg() const throw() { return m_msg; }

    virtual const char* what() const throw() { return m_msg.c_str(); }

  private:
    std::string m_msg;
  };

}

#endif /* GENERICEXCEPTION_H_ */
