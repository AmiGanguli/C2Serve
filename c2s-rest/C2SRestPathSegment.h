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

#ifndef C2SRESTPATHSEGMENT_H_
#define C2SRESTPATHSEGMENT_H_

#include "C2SRestException.h"

#include <string>
#include <vector>

namespace g
{
  namespace c2s
  {

    typedef std::vector<std::string> C2SRestPathIDList;

    class C2SRestPathSegment
    {
    public:

      typedef enum
      {

        ID = 0,
        Parameter = 1

      } EPathSegmentType;

      C2SRestPathSegment( const std::string &sID )
        : m_sID( sID ),
          m_type( ID )
      {};

      C2SRestPathSegment( const std::string &sID , EPathSegmentType type )
        : m_sID( sID ),
          m_type( type )
      {};

      virtual ~C2SRestPathSegment(){};

      virtual void handle( const std::string &sID )
      {
        if ( m_sID != sID )
          throw C2SRestException( "C2SRestPathSegment::handle: " , "Mismatch: \"" + m_sID + "\" != " + "\"" + sID + "\"" , InternalServerError );
      }

      virtual bool isValid( const std::string &sID ) const { return m_sID == sID; }

      const std::string &id() const { return m_sID; }

      EPathSegmentType type() const { return m_type; }

    private:

      C2SRestPathSegment( const C2SRestPathSegment & );

      C2SRestPathSegment &operator=( const C2SRestPathSegment & );

      std::string m_sID;

      EPathSegmentType m_type;

    };

  }
}

#endif /* C2SRESTPATHSEGMENT_H_ */
