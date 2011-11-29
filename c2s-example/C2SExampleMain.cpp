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

#include "C2SGlobalSettings.h"
#include "C2SRestResourcePrototype.h"
#include "C2SHttpServer.h"

#include "C2SExampleRestMethodPrototypeSayHello.h"

const unsigned int iServerPort = 33456;

const std::string sExampleRESTResourceContextRoot = "c2s-example";

int main( int , char ** )
{
  using namespace c2s;
  using namespace c2s::example;

  //define server port
  C2SGlobalSettings &globalServerSettings = C2SGlobalSettings::Settings();
  globalServerSettings.iPort = iServerPort;

  //create new REST resource
  C2SRestResourcePrototype *pResource = C2SRestResourcePrototype::createRestResourceWithContextRoot( sExampleRESTResourceContextRoot );

  //register method prototypes
  //for each server thread, a copy of a method prototype is created (by calling the clone() method)
  //this ensures that different process don't share the same path and query parameters
  pResource->registerMethodPrototype( new C2SExampleRestMethodPrototypeSayHello() );

  //register REST resource at the http resource manager
  C2SHttpServer::registerResourcePrototype( pResource );

  //start socket listener
  //this method blocks the program
  C2SHttpServer::run();
}
