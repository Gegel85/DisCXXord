#ifndef DISCXXORD_CHANNEL_HPP
#define DISCXXORD_CHANNEL_HPP


#include "Snowflake.hpp"
#include "Optional.hpp"
#include "User.hpp"
#include "Message.hpp"
#include "PermissionOverwrite.hpp"
#include "Embed.hpp"

namespace DisCXXord
{
	class Guild;
	class CategoryChannel;

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

		template <typename type>
		bool is() const {
			type *result = dynamic_cast<type *>(this);

			return result != nullptr;
		};

		bool is(Type expected) const;

		template <typename type>
		type &to() {
			return dynamic_cast<type &>(*this);
		};

		virtual Message send(const std::string &content) = 0;
		virtual Message send(Embed embed, const std::string &content = "") = 0;

		Optional<int> pos;
		Optional<int> rateLimit;
		Optional<int> userLimit;
		Optional<int> bitrate;
		Optional<Date> lastPin;
		Optional<bool> nsfw;
		Optional<User> owner;
		Optional<Guild> guild;
		Optional<Message> lastMessage;
		Optional<Snowflake> appID;
		Optional<std::string> name;
		Optional<std::string> topic;
		Optional<std::string> iconHash;
		Optional<CategoryChannel> parent;
		Optional<std::vector<User>> recipients;
		Optional<std::vector<PermissionOverwrite>> permissions;

		explicit Channel(Client &client, json obj, Type type);
		virtual ~Channel() = default;
	};
}

#include "CategoryChannel.hpp"

#endif //DISCXXORD_CHANNEL_HPP
