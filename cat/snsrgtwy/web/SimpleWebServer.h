#include <string>
#include <cat/snsrgtwy/web/http/HttpRequest.h>
#include <cat/snsrgtwy/web/http/HttpResponse.h>
using namespace std;
using namespace snsrgtwy::web::http;

namespace cat { namespace snsrgtwy { namespace web {
	
    class SimpleWebServer {
        public:
			SimpleWebServer(string address);
            HttpResponse* send(HttpRequest request);
			string* debug();
		protected:
			string address;
    };  
}}}
