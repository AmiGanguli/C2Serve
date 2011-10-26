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

#include "C2SHttpMediaType.h"
#include "C2SHttpException.h"
#include "C2SStringUtils.h"
#include "StringUtils.h"

#define MEDIA_TYPE_QUALITY_DEFAULT 1.f

namespace g
{

  namespace c2s
  {

    const std::string C2SHttpMediaType::wildcard = "*/*";
    const std::string C2SHttpMediaType::text__css = "text/css";
    const std::string C2SHttpMediaType::text__html = "text/html";
    const std::string C2SHttpMediaType::application__xhtml_xml = "application/xhtml+xml";
    const std::string C2SHttpMediaType::application__xml = "application/xml";
    const std::string C2SHttpMediaType::application__json = "application/json";
    const std::string C2SHttpMediaType::application__x_www_form_urlencoded = "application/x-www-form-urlencoded";

    struct C2SHttpQualityInterpreter
    {

      C2SHttpQualityInterpreter()
        : iArgIdx( 0 ),
          fValue( MEDIA_TYPE_QUALITY_DEFAULT )
      {};

      void operator()( const char *data , unsigned int size )
      {
        if ( iArgIdx )
          fValue = g::util::toNumber<float>( std::string( data , size ) );

        ++iArgIdx;
      }

      unsigned int iArgIdx;

      float fValue;

    };

    C2SHttpMediaType::C2SHttpMediaType( const std::string &sType )
      : Type( sType ),
        fQ( MEDIA_TYPE_QUALITY_DEFAULT ),
        iArgIdx( 0 )
    {};

    C2SHttpMediaType::C2SHttpMediaType()
      : Type( C2SHttpMediaType::wildcard ),
        fQ( MEDIA_TYPE_QUALITY_DEFAULT ),
        iArgIdx( 0 )
    {};

    void C2SHttpMediaType::detect( const char *data , unsigned int size )
    {
      if ( !iArgIdx )
        Type = std::string( data , size );

      if ( iArgIdx > 1 )
        throw C2SHttpException( "C2SHttpMediaType::operator():" , "Maximum arguments for media type is 2!" , BadRequest );

      if ( iArgIdx )
        this->detectQualityValue( data , size );

      ++iArgIdx;
    }

    std::string C2SHttpMediaType::toString() const
    {
      std::stringstream strs;

      strs << Type;
      if ( fQ != MEDIA_TYPE_QUALITY_DEFAULT )
        strs << ";q=" << fQ;

      return strs.str();
    }

    void C2SHttpMediaType::operator()( const char *data , unsigned int size )
    {
      this->detect( data , size );
    }

    void C2SHttpMediaType::detectQualityValue( const char *data , unsigned int size )
    {
      C2SHttpQualityInterpreter qv;
      splitNhandle( data , size , '=' , &qv );
      fQ = qv.fValue;
    }

  }

}
