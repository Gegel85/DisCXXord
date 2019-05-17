#ifndef DISCXXORD_ATTACHEMENT_HPP
#define DISCXXORD_ATTACHEMENT_HPP


#include "Snowflake.hpp"
#include "Optional.hpp"

namespace DisCXXord
{
	class Attachement : public Snowflake {
	public:
		std::string	filename;
		size_t		size;
		std::string	url;
		std::string	proxyUrl;
		Optional<size_t>height;
		Optional<size_t>width;

		Attachement(Client &client, json val);
	};
}


#endif //DISCXXORD_ATTACHEMENT_HPP
