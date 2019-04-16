#ifndef DISCXXORD_SNOWFLAKE_HPP
#define DISCXXORD_SNOWFLAKE_HPP


#include <string>
#include <JsonObject.hpp>
#include "Date.hpp"

namespace DisCXXord
{
	class Client;

	class Snowflake {
	protected:
		Client &_parent;
		std::string _id;
		Date _createdAt;

	public:
		Snowflake(Client &client, JsonObject &obj);
		const Date &createdAt() const;
		std::string id() const;
		std::string timestamp() const;
	};
}


#endif //DISCXXORD_SNOWFLAKE_HPP
