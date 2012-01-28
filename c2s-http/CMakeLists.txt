
cmake_minimum_required(VERSION 2.6)

include_directories(
  ${C2S_PROJECT_ROOT_DIR}/c2s-core
  ${C2S_PROJECT_ROOT_DIR}/c2s-util
  ${C2S_PROJECT_ROOT_DIR}/c2s-util/io
)

add_library( c2s_http
	C2SHttpServer
  C2SHttpDataPull
  C2SHttpHeaderFields
  C2SHttpQueryFields
  C2SHttpRequest
  C2SHttpRequestHeader
  C2SHttpResponse
  C2SHttpResponseHeader
  C2SHttpResourcePrototype
  C2SHttpParser
  C2SHttpParseResponse
  C2SHttpParseQueryFields
  C2SHttpParseQueryFieldAndAddToList
  C2SHttpStatus
  C2SHttpMatching
  C2SHttpMediaType
  C2SHttpMediaTypeList
  C2SHttpResourceManager
  C2SHttpDataHandlingImpl
  C2SHttpResourcePrototypeList
  C2SHttpResourceManagerResponseHandler
 )

add_library( c2s_httpclient
  C2SHttpClient
  )
    
install( TARGETS 
	c2s_http
	c2s_httpclient
	DESTINATION lib )
	