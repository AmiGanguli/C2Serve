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

#ifndef C2SRESTPARAMETER_H_
#define C2SRESTPARAMETER_H_

#include <sstream>

namespace g
{
  namespace c2s
  {

    class C2SRestParameterBase
    {
    public:

      C2SRestParameterBase() {};

      virtual ~C2SRestParameterBase(){};

      virtual bool isValid( const std::string &sID ) const = 0;

      virtual void handle( const std::string &sID ) = 0;

    private:

      C2SRestParameterBase( const C2SRestParameterBase & );

      C2SRestParameterBase &operator=( const C2SRestParameterBase & );

    };

    template <class Type>
    class C2SRestParameter : public C2SRestParameterBase
    {
    public:

      C2SRestParameter( Type *pDest )
        : m_pDest( pDest )
      {};

      virtual ~C2SRestParameter(){};

      virtual bool isValid( const std::string &sID ) const;

      virtual void handle( const std::string &sID );

      void setValue( const Type &value ) { *m_pDest = value; }

    protected:

      Type *m_pDest;

    };

    template <class Type>
    bool C2SRestParameter<Type>::isValid( const std::string &sID ) const
    {
      std::istringstream strs( sID );
      Type dummy = Type( 0 );
      return ( strs >> dummy ) != 0;
    }

    //specialized for strings
    template <>
    inline bool C2SRestParameter<std::string>::isValid( const std::string & ) const
    {
      //will always match
      return true;
    }

    template <class Type>
    void C2SRestParameter<Type>::handle( const std::string &sID )
    {
      std::istringstream strs( sID );
      if( !( strs >> ( *m_pDest ) ) )
        throw C2SRestException( "C2SRestPathParameter::handle: " , "Cannot cast parameter \"" + sID + "\"" , BadRequest );
    }

    //specialized for strings
    template <>
    void inline C2SRestParameter<std::string>::handle( const std::string &sID )
    {
      *m_pDest = sID;
    }

  }
}

#endif /* C2SRESTPARAMETER_H_ */
