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

#include "C2SHttpStatus.h"
#include "C2SHttpException.h"

namespace g
{
  namespace c2s
  {

    C2SHttpStatus fromString( const std::string &status )
    {
      //success
      if ( status == "200" ) return OK;
      if ( status == "201" ) return Created;
      if ( status == "202" ) return Accepted;
      if ( status == "203" ) return NonAuthoritativeInformation;
      if ( status == "204" ) return NoContent;
      if ( status == "205" ) return ResetContent;
      if ( status == "206" ) return PartialContent;

      //redirection
      if ( status == "300" ) return MultipleChoices;
      if ( status == "301" ) return MovedPermanently;
      if ( status == "302" ) return Found;
      if ( status == "303" ) return SeeOther;
      if ( status == "304" ) return NotModified;
      if ( status == "305" ) return UseProxy;
      if ( status == "307" ) return TemporaryRedirect;

      //client error
      if ( status == "400" ) return BadRequest;
      if ( status == "401" ) return Unauthorized;
      if ( status == "402" ) return PaymentRequired;
      if ( status == "403" ) return Forbidden;
      if ( status == "404" ) return NotFound;
      if ( status == "405" ) return MethodNotAllowed;
      if ( status == "406" ) return NotAcceptable;
      if ( status == "407" ) return ProxyAuthenticationRequired;
      if ( status == "408" ) return RequestTimeout;
      if ( status == "409" ) return Conflict;
      if ( status == "410" ) return Gone;
      if ( status == "411" ) return LengthRequired;
      if ( status == "412" ) return PreconditionFailed;
      if ( status == "413" ) return RequestEntityTooLarge;
      if ( status == "414" ) return RequestURITooLong;
      if ( status == "415" ) return UnsupportedMediaType;
      if ( status == "416" ) return RequestedRangeNotSatisfiable;
      if ( status == "417" ) return ExpectationFailed;

      //server error
      if ( status == "500" ) return InternalServerError;
      if ( status == "501" ) return NotImplemented;
      if ( status == "502" ) return BadGateway;
      if ( status == "503" ) return ServiceUnavailable;
      if ( status == "504" ) return GatewayTimeout;
      if ( status == "505" ) return HTTPVersionNotSupported;

      throw C2SHttpException( "fromString: " , "Unknown HTTP status code: " + status , InternalServerError );
    }

  }
}
