#include <cat/snsrgtwy/web/SimpleWebServer.h>
using namespace std;
using namespace cat::snsrgtwy::web::http;

namespace cat { namespace snsrgtwy { namespace web {
	
	SimpleWebServer::SimpleWebServer(string address) {
		this->address = address;
	}
	
	HttpResponse* SimpleWebServer::send(HttpRequest request) {
		HttpResponse* resp = new HttpResponse();
		return resp;
	}
	
	string* SimpleWebServer::debug() {
		return &this->address;
	}
}}}

