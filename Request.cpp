/*
** EPITECH PROJECT, 2019
** DisCXXord
** File description:
** Request.cpp
*/

#include "Request.hpp"
#include "Exceptions.hpp"
#include "SecuredSocket.hpp"

#include <iostream>
namespace DisCXXord
{
	Socket::HttpRequestIn Request::parseUrl(const std::string &url)
	{
		Socket::HttpRequestIn in;
		try {
			std::string proto = url.substr(0, url.find("://"));
			std::string other = url.substr(proto.size() + 3);

			if (proto == "https")
				in.portno = 443;
			else if (proto == "http")
				in.portno = 80;
			else
				throw std::invalid_argument(proto + " is a known protocol");
			in.host = other.substr(0, other.find('/'));
			in.path = other.substr(in.host.size());
			if (in.path.empty())
				in.path = "/";
		} catch (std::exception &e) {
			throw InvalidURLException(e.what());
		}
		return in;
	}

	Request::HttpRequest Request::request(HttpRequest req)
	{
		Socket			sock;
		SecuredSocket		ssock;
		Socket::HttpRequestIn	in = parseUrl(req.url);
		Socket::HttpRequestOut	out;

		in.body = req.body;
		in.header = req.headers;
		in.method = req.method;
		if (in.portno == 443)
			out = ssock.makeHttpRequest(in);
		else
			out = sock.makeHttpRequest(in);
		req.body = out.body;
		req.code = out.returnCode;
		req.headers = out.header;
		req.codeName = out.codeName;
		return req;
	}
}