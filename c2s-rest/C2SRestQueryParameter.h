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

#ifndef C2SRESTQUERYPARAMETER_H_
#define C2SRESTQUERYPARAMETER_H_

#include "C2SRestParameter.h"

namespace c2s
{

  class C2SRestQueryParameterBase
  {
  public:

    C2SRestQueryParameterBase( const std::string &sID )
      : m_sID( sID )
    {};

    virtual void setDefault() = 0;

    virtual void handle( const std::string &sValue ) = 0;

  protected:

    std::string m_sID;

  };

  template <class Type>
  class C2SRestQueryParameter : public C2SRestQueryParameterBase
  {
  public:

    C2SRestQueryParameter( const std::string &sID , Type *pDest , const Type &defaultv )
      : C2SRestQueryParameterBase( sID ),
        m_parameter( pDest ),
        m_default( defaultv ),
        m_bIsDefaultAllowed( true )
    {};


    C2SRestQueryParameter( const std::string &sID , Type *pDest )
      : C2SRestQueryParameterBase( sID ),
        m_parameter( pDest ),
        m_bIsDefaultAllowed( false )
    {};

    void setDefault()
    {
      if ( m_bIsDefaultAllowed )
        m_parameter.setValue( m_default );
      else
        throw C2SRestException( "C2SRestQueryParameter::setDefault: " , "Query parameter missing: " + m_sID , BadRequest );
    }

    void handle( const std::string &sValue )
    {
      m_parameter.handle( sValue );
    }

  private:

    C2SRestParameter<Type> m_parameter;

    Type m_default;

    bool m_bIsDefaultAllowed;

  };

}

#endif /* C2SRESTQUERYPARAMETER_H_ */
