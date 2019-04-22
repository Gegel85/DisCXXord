#ifndef DISCXXORD_CHANNEL_HPP
#define DISCXXORD_CHANNEL_HPP


#include "Snowflake.hpp"

namespace DisCXXord
{
	class Channel : public Snowflake {
	public:
		enum Type {
			GUILD_TEXT,
			DM,
			GUILD_VOICE,
			GROUP_DM,
			GUILD_CATEGORY,
			GUILD_NEWS,
			GUILD_STORE,
		};

		Type type;

		explicit Channel(Client &client, JsonObject &obj, Type type);
	};
}


#endif //DISCXXORD_CHANNEL_HPP
