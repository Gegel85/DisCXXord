#ifndef DISCXXORD_REQUEST_HPP
#define DISCXXORD_REQUEST_HPP


#include <vector>
#include "Socket.hpp"

namespace DisCXXord
{
	class Request {
	public:
		struct HttpRequest {
			int code = 0;
			std::string method = "GET";
			std::string url;
			std::string codeName = "";
			std::string body = "";
			std::map<std::string, std::string> headers = {};
		};

		static Socket::HttpRequestIn parseUrl(const std::string &url);
		static HttpRequest request(HttpRequest req);
	};
}


#endif //DISCXXORD_REQUEST_HPP
