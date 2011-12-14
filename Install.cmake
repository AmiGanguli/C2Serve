
set( C2S_INSTALL_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include )

file( COPY
  ${C2S_PROJECT_ROOT_DIR}/c2s-util/GlobalDefines.h
  ${C2S_PROJECT_ROOT_DIR}/c2s-util/GenericException.h
  ${C2S_PROJECT_ROOT_DIR}/c2s-util/io/DateTimeLogger.h
  ${C2S_PROJECT_ROOT_DIR}/c2s-util/io/Logger.h
    DESTINATION
  ${C2S_INSTALL_INCLUDE_DIR}/c2s-util
)

file( COPY
  ${C2S_PROJECT_ROOT_DIR}/c2s-core/C2SSettings.h
  ${C2S_PROJECT_ROOT_DIR}/c2s-core/C2SException.h
  ${C2S_PROJECT_ROOT_DIR}/c2s-core/C2SStringUtils.h
    DESTINATION
  ${C2S_INSTALL_INCLUDE_DIR}/c2s-core
)

file( COPY
  ${C2S_PROJECT_ROOT_DIR}/c2s-http/C2SHttpResourcePrototype.h
  ${C2S_PROJECT_ROOT_DIR}/c2s-http/C2SHttpMethod.h
  ${C2S_PROJECT_ROOT_DIR}/c2s-http/C2SHttpRequest.h
  ${C2S_PROJECT_ROOT_DIR}/c2s-http/C2SHttpResponse.h
  ${C2S_PROJECT_ROOT_DIR}/c2s-http/C2SHttpRequestHeader.h
  ${C2S_PROJECT_ROOT_DIR}/c2s-http/C2SHttpResponseHeader.h
  ${C2S_PROJECT_ROOT_DIR}/c2s-http/C2SHttpEntity.h
  ${C2S_PROJECT_ROOT_DIR}/c2s-http/C2SHttpParser.h
  ${C2S_PROJECT_ROOT_DIR}/c2s-http/C2SHttpServer.h
  ${C2S_PROJECT_ROOT_DIR}/c2s-http/C2SHttpMediaTypeList.h
  ${C2S_PROJECT_ROOT_DIR}/c2s-http/C2SHttpHeaderFields.h
  ${C2S_PROJECT_ROOT_DIR}/c2s-http/C2SHttpQueryFields.h
  ${C2S_PROJECT_ROOT_DIR}/c2s-http/C2SHttpException.h
  ${C2S_PROJECT_ROOT_DIR}/c2s-http/C2SHttpStatus.h
  ${C2S_PROJECT_ROOT_DIR}/c2s-http/C2SHttpMediaType.h
  ${C2S_PROJECT_ROOT_DIR}/c2s-http/C2SHttpEntityStreamerBase.h
  ${C2S_PROJECT_ROOT_DIR}/c2s-http/C2SHttpInvalidEntityType.h
  ${C2S_PROJECT_ROOT_DIR}/c2s-http/C2SHttpEntityStreamerPlainText.h
    DESTINATION
  ${C2S_INSTALL_INCLUDE_DIR}/c2s-http
)

file( COPY
  ${C2S_PROJECT_ROOT_DIR}/c2s-rest/C2SRestResourcePrototype.h
  ${C2S_PROJECT_ROOT_DIR}/c2s-rest/C2SRestMethodPrototype.h
  ${C2S_PROJECT_ROOT_DIR}/c2s-rest/C2SRestMethodPrototypeList.h
  ${C2S_PROJECT_ROOT_DIR}/c2s-rest/C2SRestPathParameter.h
  ${C2S_PROJECT_ROOT_DIR}/c2s-rest/C2SRestQueryParameter.h
  ${C2S_PROJECT_ROOT_DIR}/c2s-rest/C2SRestPathSegmentList.h
  ${C2S_PROJECT_ROOT_DIR}/c2s-rest/C2SRestResourceDescription.h
  ${C2S_PROJECT_ROOT_DIR}/c2s-rest/C2SRestMethodPrototypeGET.h
  ${C2S_PROJECT_ROOT_DIR}/c2s-rest/C2SRestPathSegment.h
  ${C2S_PROJECT_ROOT_DIR}/c2s-rest/C2SRestQueryParameterBase.h
  ${C2S_PROJECT_ROOT_DIR}/c2s-rest/C2SRestParameter.h
  ${C2S_PROJECT_ROOT_DIR}/c2s-rest/C2SRestPathIDList.h
  ${C2S_PROJECT_ROOT_DIR}/c2s-rest/C2SRestParameterBase.h
  ${C2S_PROJECT_ROOT_DIR}/c2s-rest/C2SRestException.h
  ${C2S_PROJECT_ROOT_DIR}/c2s-rest/C2SRestMethodPrototypeResponseEntity.h
    DESTINATION
  ${C2S_INSTALL_INCLUDE_DIR}/c2s-rest
)
