#ifndef DISCXXORD_SNOWFLAKE_HPP
#define DISCXXORD_SNOWFLAKE_HPP


#include <string>
#include "nlohmann/json.hpp"
#include "Date.hpp"

using json = nlohmann::json;

namespace DisCXXord
{
	class Client;

	class Snowflake {
	protected:
		Client &_parent;

	public:
		const std::string id;
		const Date createdAt;

		Snowflake(Client &client, json obj);
		std::string timestamp() const;
		bool operator==(Snowflake &);
	};
}


#endif //DISCXXORD_SNOWFLAKE_HPP
