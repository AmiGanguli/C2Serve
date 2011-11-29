
cmake_minimum_required(VERSION 2.6)

add_library( c2s_http
	C2SHttpServer
    C2SHttpServerType
    C2SHttpDataPull
    C2SHttpHeaderFields
    C2SHttpQueryFields
    C2SHttpRequest
    C2SHttpRequestHeader
    C2SHttpResponse
    C2SHttpResponseHeader
    C2SHttpResourcePrototype
    C2SHttpParser
    C2SHttpStatus
    C2SHttpMediaType
    C2SHttpMediaTypeList
    C2SHttpResourceManager )

add_library( c2s_httpclient
    C2SHttpClient )
    
install( TARGETS 
	c2s_http
	c2s_httpclient
	DESTINATION lib )
	