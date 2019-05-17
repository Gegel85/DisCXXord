#ifndef DISCXXORD_PARTIALUSER_HPP
#define DISCXXORD_PARTIALUSER_HPP


#include "Snowflake.hpp"
#include "Optional.hpp"
#include "nlohmann/json.hpp"

namespace DisCXXord
{
	class PartialUser : public Snowflake {
	public:
		std::string username;
		Optional<std::string> avatarHash;
		bool bot = true;

		PartialUser(Client &client, json val);
	};
}


#endif //DISCXXORD_PARTIALUSER_HPP
