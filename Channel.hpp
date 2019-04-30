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

		static std::string typeToString(Type e) {
			switch (e) {
			case DM:
				return "Private Channel";
			case GUILD_CATEGORY:
				return "Guild Category Channel";
			case GUILD_TEXT:
				return "Guild Text Channel";
			case GUILD_VOICE:
				return "Guild Voice Channel";
			case GUILD_NEWS:
				return "Guild News Channel";
			case GUILD_STORE:
				return "Guild Store Channel";
			case GROUP_DM:
				return "Private Group Channel";
			default:
				return "Unknown Channel Type";
			}
		}

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

		Message send(const Embed &embed);
		Message send(const std::string &content);
		virtual Message send(const Embed &embed, const std::string &content) = 0;

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
#include "Guild.hpp"

#endif //DISCXXORD_CHANNEL_HPP
