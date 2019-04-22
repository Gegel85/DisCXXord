//
// Created by Gegel85 on 22/04/2019.
//

#ifndef DISC_ORD_PERMISSIONS_HPP
#define DISC_ORD_PERMISSIONS_HPP

namespace DisCXXord
{
	class Permissions {
	public:
		unsigned value;

		enum Permission {
			NONE =			0,
			CREATE_INSTANT_INVITE =	1U << 1U,
			KICK_MEMBERS =		1U << 2U,
			BAN_MEMBERS =		1U << 3U,
			ADMINISTRATOR =		1U << 4U,
			MANAGE_CHANNELS  =	1U << 5U,
			MANAGE_GUILD  =		1U << 6U,
			ADD_REACTIONS =		1U << 7U,
			VIEW_AUDIT_LOG =	1U << 8U,
			VIEW_CHANNEL =		1U << 9U,
			SEND_MESSAGES =		1U << 10U,
			SEND_TTS_MESSAGES =	1U << 11U,
			MANAGE_MESSAGES  =	1U << 12U,
			EMBED_LINKS =		1U << 13U,
			ATTACH_FILES =		1U << 14U,
			READ_MESSAGE_HISTORY =	1U << 15U,
			MENTION_EVERYONE =	1U << 16U,
			USE_EXTERNAL_EMOJIS =	1U << 17U,
			CONNECT =		1U << 18U,
			SPEAK =			1U << 19U,
			MUTE_MEMBERS =		1U << 20U,
			DEAFEN_MEMBERS =	1U << 21U,
			MOVE_MEMBERS =		1U << 22U,
			USE_VAD =		1U << 23U,
			PRIORITY_SPEAKER =	1U << 24U,
			CHANGE_NICKNAME =	1U << 25U,
			MANAGE_NICKNAMES =	1U << 26U,
			MANAGE_ROLES =		1U << 27U,
			MANAGE_WEBHOOKS =	1U << 28U,
			MANAGE_EMOJIS =		1U << 29U,
		};

		Permissions(unsigned val = NONE);
		bool has(unsigned perm);
	};
}

#endif //DISC_ORD_PERMISSIONS_HPP
