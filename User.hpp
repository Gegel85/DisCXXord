//
// Created by Gegel85 on 15/04/2019.
//

#ifndef DISC_ORD_USER_HPP
#define DISC_ORD_USER_HPP


#include <string>
#include <optional>
#include <JsonParser.hpp>

namespace DisCXXord
{
	class User {
	private:
		bool _bot;
		int _flags;
		bool _mfaEnabled;
		std::string _id;
		std::string _username;
		std::string _discriminator;
		std::optional<bool> _verified;
		std::optional<int> _premium_type;
		std::optional<std::string> _email;
		std::optional<std::string> _locale;
		std::optional<std::string> _avatarHash;

	public:
		explicit User(JsonObject &obj);
		int defaultAvatar();
		time_t createdAt();
		bool mfaEnabled();
		bool bot();
		int flags();
		std::string id();
		std::string tag();
		std::string username();
		std::string avatarURL();
		std::string timestamp();
		std::string discriminator();
		std::string mentionString();
		std::string defaultAvatarURL();
		std::optional<bool> verified();
		std::optional<int> premium_type();
		std::optional<std::string> email();
		std::optional<std::string> locale();
		std::optional<std::string> avatar();
		std::string getDefaultAvatarURL(int size = 128);
		std::string getAvatarURL(int size = 128, const std::string &format = "png");
	};
}


#endif //DISC_ORD_USER_HPP
