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

#include "C2SHttpParser.h"

#include "C2SHttpParseURI.h"
#include "C2SHttpParseResource.h"
#include "C2SHttpParseResponse.h"
#include "C2SHttpParseQueryFields.h"

#include "C2SHttpDefines.h"
#include "C2SHttpQueryFields.h"
#include "C2SHttpRequestHeader.h"
#include "C2SHttpResponseHeader.h"
#include "C2SHttpParseHeaderField.h"

#include "StringUtils.h"

#include <cassert>
#include <cstring>

#define METHOD_POST "POST "
#define METHOD_GET "GET "
#define METHOD_PUT "PUT "
#define METHOD_DELETE "DELETE "

#define FIELD_ACCEPT "Accept: "
#define FIELD_CONTENT_TYPE "Content-Type: "
#define FIELD_CONTENT_LENGTH "Content-Length: "

namespace c2s
{

  C2SHttpParser::C2SHttpParser()
    : m_iTotalLines( 0 ),
      m_iNumAdjacentCRLF( 0 ),
      m_bHeaderFinished( false ),
      m_body( NULL ),
      m_iCurrentBodyIdx( 0 ),
      m_iContentSize( 0 )
  {
  }

  C2SHttpParser::C2SHttpParser( const C2SHttpParser &p )
    : m_sLastIncompleteLine( p.m_sLastIncompleteLine ),
      m_iTotalLines( p.m_iTotalLines ),
      m_iNumAdjacentCRLF( p.m_iNumAdjacentCRLF ),
      m_bHeaderFinished( p.m_bHeaderFinished ),
      m_body( NULL ),
      m_iCurrentBodyIdx( p.m_iCurrentBodyIdx ),
      m_iContentSize( p.m_iContentSize )
  {
    if ( p.m_body )
    {
      m_body = new char[ m_iContentSize ];
      memcpy( m_body , p.m_body , m_iContentSize );
    }
  }

  C2SHttpParser::~C2SHttpParser()
  {
    delete[] m_body;
  }

  C2SHttpParser &C2SHttpParser::operator=( const C2SHttpParser &p )
  {
    if ( this != &p )
    {
      m_sLastIncompleteLine = p.m_sLastIncompleteLine;
      m_iTotalLines = p.m_iTotalLines;
      m_iNumAdjacentCRLF = p.m_iNumAdjacentCRLF;
      m_bHeaderFinished = p.m_bHeaderFinished;
      m_iCurrentBodyIdx = p.m_iCurrentBodyIdx;
      m_iContentSize = p.m_iContentSize;

      if ( m_body )
        delete[] m_body;
      m_body = NULL;

      if ( p.m_body )
      {
        m_body = new char[ m_iContentSize ];
        memcpy( m_body , p.m_body , m_iContentSize );
      }
    }
    return *this;
  }

  char *C2SHttpParser::entity( unsigned int size )
  {
    if ( !m_body )
      throw C2SHttpException( "C2SHttpParser::body: " , "Data is NULL!" , InternalServerError );

    if ( size > m_iContentSize )
      throw C2SHttpException( "C2SHttpParser::body: " , "Requested size (" + c2s::util::toString( size ) + ") exceeds actual size (" + c2s::util::toString( m_iContentSize ) + ")!" , InternalServerError );

    if ( m_iCurrentBodyIdx < size )
      throw C2SHttpException( "C2SHttpParser::body: " , "Actual body size (" + c2s::util::toString( m_iCurrentBodyIdx ) + ") is smaller than requested size (" + c2s::util::toString( size ) + ")!" , InternalServerError );

    return m_body;
  }


  bool C2SHttpParser::isAllRequestDataReceived() const
  {
    if ( !m_bHeaderFinished )
      return false;

    if ( m_iContentSize > m_iCurrentBodyIdx )
      return false;

    return true;
  }

  void handleFirstLine( const char *data , unsigned int size , C2SHttpResponseHeader *pHeader )
  {
    C2SHttpParseResponse rp;
    splitNhandle( data , size , ' ' , &rp );

    pHeader->Version = rp.fHttpVerstion;
    pHeader->Status = rp.Status;
    pHeader->ReasonPhrase = rp.sReasonPhrase;
  }

  void handleFirstLine( const char *data , unsigned int size , C2SHttpRequestHeader *pHeader )
  {
    unsigned int iRefSize = 0;

    if ( startsWith( data , size , METHOD_POST , std::strlen( METHOD_POST ) ) )
    {
      iRefSize = std::strlen( METHOD_POST );
      pHeader->Method = POST;
    }
    else if ( startsWith( data , size , METHOD_GET , std::strlen( METHOD_GET ) ) )
    {
      iRefSize = std::strlen( METHOD_GET );
      pHeader->Method = GET;
    }
    else if ( startsWith( data , size , METHOD_PUT , std::strlen( METHOD_PUT ) ) )
    {
      iRefSize = std::strlen( METHOD_PUT );
      pHeader->Method = PUT;
    }
    else if ( startsWith( data , size , METHOD_DELETE , std::strlen( METHOD_DELETE ) ) )
    {
      iRefSize = std::strlen( METHOD_DELETE );
      pHeader->Method = DELETE;
    }
    else
      throw C2SHttpException( "C2SHttpParser::handleFirstLine: " , "Cannot read header: " + std::string( data , size ) , BadRequest );

    C2SHttpParseResource resource( pHeader );
    splitNhandle( data + iRefSize , ( size - iRefSize ) , ' ' , &resource );
//      pHeader->URI = resource.sURI;
//      pHeader->Version = resource.fHttpVerstion;
//      exit( 0 );
  }

  template <class HeaderType>
  void handleField( const char *data , unsigned int size , HeaderType *pHeader )
  {
    C2SHttpParseHeaderField field;
    splitNhandle( data , size , ':' , &field );

    if ( !field.sName.size() )
      throw C2SHttpException( "C2SHttpParser::handleField: " , "Cannot parse field: " + std::string( data , size ) , BadRequest );

    pHeader->Fields.set( field.sName , field.sValue );
  }

/*    bool handle( const char *data , unsigned int size , EC2SHttpHeaderFieldType field , C2SHttpRequestHeader *pHeader )
  {
    const char *ref = NULL;

    if ( field == FieldMethodGet )
      ref = METHOD_GET;
    else if ( field == FieldMethodPost )
      ref = METHOD_POST;
    else if ( field == FieldAccept )
      ref = FIELD_ACCEPT;
    else if ( field == FieldContentType )
      ref = FIELD_CONTENT_TYPE;
    else if ( field == FieldContentLength )
      ref = FIELD_CONTENT_LENGTH;
    else
      throw C2SHttpException( "C2SHttpParser::handle:" , "Unknown field type (1)!" , InternalServerError );

    unsigned int iRefSize = std::strlen( ref );

    if ( size < iRefSize )
      return false;

    for ( unsigned int i = 0; i < iRefSize; ++i, ++data, ++ref )
    {
      if ( *data != *ref )
        return false;
    }

    if ( field == FieldMethodGet )
    {
      exit( - 1 );
      pHeader->Method = C2SHttpMethod::GET;
      C2SHttpResourceParser resource;
      splitNhandle( data , ( size - iRefSize ) , ' ' , &resource );
      pHeader->URI = resource.sURI;
      pHeader->Version = resource.fHttpVerstion;
    }
    else if ( field == FieldMethodPost )
    {
      exit( - 1 );
      pHeader->Method = C2SHttpMethod::POST;
      C2SHttpResourceParser resource;
      splitNhandle( data , ( size - iRefSize ) , ' ' , &resource );
      pHeader->URI = resource.sURI;
      pHeader->Version = resource.fHttpVerstion;
    }
    else if ( field == FieldAccept )
      pHeader->Accept = C2SHttpMediaTypeList::detect( data , ( size - iRefSize ) );
    else if ( field == FieldContentType )
      pHeader->ContentType.detect( data , ( size - iRefSize ) );
    else if ( field == FieldContentLength )
      pHeader->ContentLength = c2s::utils::toNumber<unsigned int>( std::string ( data , size - iRefSize ) );
    else
      throw C2SHttpException( "C2SHttpParser::handle:" , "Unknown field type (2)!" , InternalServerError );

    return true;
  }
*/

  template void C2SHttpParser::parse( const char *data , unsigned int size , C2SHttpRequestHeader *pHeader );
  template void C2SHttpParser::parse( const char *data , unsigned int size , C2SHttpResponseHeader *pHeader );
  template <class HeaderType>
  void C2SHttpParser::parse( const char *data , unsigned int size , HeaderType *pHeader )
  {
    const char *c = data;
    const char *currentLine = c;
    unsigned int iCurrentSize = 1;
    unsigned int iCurrentLines = 0;
    unsigned int idx = 0;
    for ( ; idx < size && !m_bHeaderFinished; ++idx, ++c, ++iCurrentSize )
    {
      if ( m_iNumAdjacentCRLF == 2 )
      {
        m_bHeaderFinished = true;
        break;
      }

//#ifdef LINUX
      //we have CR character, next should be LF
      if ( *c == 0x0D )
      {
        ++idx;

        //end reached (unlikely)
        if ( idx == size )
          break;

        ++c;
      }

      if ( *c == 0x0A )
//#endif
      {
        //ignore empty lines at the beginning (due to recommendation of http standard)
        if ( iCurrentSize == 1 && !m_iTotalLines && !m_sLastIncompleteLine.size() )
        {
          iCurrentSize = 0;
          continue;
        }

        ++m_iNumAdjacentCRLF;

        if ( iCurrentSize > 1 )
        {
          //check if we have remaining stuff from the last chunk (should hardly occur)
          if ( !iCurrentLines && m_sLastIncompleteLine.size() )
          {
            m_sLastIncompleteLine += std::string( currentLine , iCurrentSize - 1 );
            this->parseLine( m_sLastIncompleteLine.c_str() , m_sLastIncompleteLine.size() , pHeader );
            m_sLastIncompleteLine = "";
          }
          else
          {
            while( ( *currentLine == 0x0D || *currentLine == 0x0A ) && ( iCurrentSize - 1 ) > 0 )
            {
              ++currentLine;
//                --iCurrentSize;
            }

            this->parseLine( currentLine , iCurrentSize - 1 , pHeader );
          }

          ++iCurrentLines;
        }
        else if ( !iCurrentLines && m_sLastIncompleteLine.size()  )
        {
          //last line was complete (but cut off at CR/LF)
          this->parseLine( m_sLastIncompleteLine.c_str() , m_sLastIncompleteLine.size() , pHeader );

//            ++currentLine;
//            this->parseLine( currentLine , iCurrentSize - 2 , pHeader );
          m_sLastIncompleteLine = "";
        }

        currentLine = c + 1;
        iCurrentSize = 0;
        ++m_iTotalLines;
      }
      else
      {
        m_iNumAdjacentCRLF = 0;
      }
    }

    if ( m_iNumAdjacentCRLF == 2 )
      m_bHeaderFinished = true;

    //remark last line for handling the next chunk
    if ( iCurrentSize > 1 && !m_bHeaderFinished )
    {
      while( ( *currentLine == 0x0D || *currentLine == 0x0A ) && ( iCurrentSize - 1 ) > 0 )
      {
        ++currentLine;
//          --iCurrentSize;
      }

      m_sLastIncompleteLine += std::string( currentLine , iCurrentSize - 1 );
    }

    if ( m_bHeaderFinished && pHeader->Fields.getContentLength() )
    {
      unsigned int iChunkLength = size - idx;
      assert( ( iChunkLength ) <= pHeader->Fields.getContentLength() );

      if ( !m_body )
      {
        m_body = new char[ pHeader->Fields.getContentLength() ];
        m_iContentSize = pHeader->Fields.getContentLength();
      }

      //copy data
      assert( iChunkLength + m_iCurrentBodyIdx <= m_iContentSize );
      char *currentBody = m_body + m_iCurrentBodyIdx;
      memcpy( currentBody , currentLine , iChunkLength );
      m_iCurrentBodyIdx += iChunkLength;
    }

  }

  template <class HeaderType>
  void C2SHttpParser::parseLine( const char *data , unsigned int size , HeaderType *pHeader )
  {
    if ( !m_iTotalLines )
      handleFirstLine( data , size , pHeader );
    else
      handleField( data , size , pHeader );

/*      if ( handle( data , size , FieldMethodGet , pHeader ) )
      return;

    if ( handle( data , size , FieldMethodPost , pHeader ) )
      return;

    if ( handle( data , size , FieldAccept , pHeader ) )
      return;

    if ( handle( data , size , FieldContentType , pHeader ) )
      return;

    if ( handle( data , size , FieldContentLength , pHeader ) )
      return;*/
  }

}
