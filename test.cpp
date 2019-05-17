#include <iostream>
#include "Client.hpp"
#include "Request.hpp"
#include "token.hpp"

using namespace DisCXXord;

void	ready(Client &client)
{
	const User &me = client.me();

	std::cout << "Connected on " << me.tag() << std::endl;
	std::cout << "My avatar url is " << me.avatarURL() << std::endl;
	std::cout << "I'm on " << client.guilds().size() << " server(s) !" << std::endl;
	std::cout << "Here is the ids of all those servers: " << std::endl;
	for (const std::string &id : client.guilds()) {
		std::cout << "\t" << id << std::endl;
		std::cout << "\tClients list: " << std::endl;
		for (Member &member : client.getGuild(id).members) {
			std::cout << "\t\tid: " << member.user.id << std::endl;
			std::cout << "\t\tname: " << member.nick << "(" << member.user.username << ")" << std::endl;
		}
	}
}

int	main()
{
	Client	client("./disc++ord.log", Logger::DEBUG);

	client.setHandlers({
		.ready = ready,
		.messageCreate = [](Client &client, Message &message) {
			if (message.content == "!disconnect") {
				message.reply({
					.content = "",
					.embed = {
						"Disconnecting",          //title
						"Leaving discord",        //description
						"",                       //url
						Date::now(),              //timestamp
						0xFF0000,                 //color
						EmbedFooter{              //footer
							message.author.username,
							message.user->avatarURL()
						}
					}
				});
				client.disconnect();
			} else if (message.content == "!type") {
				message.channel.startTyping();
			}
		}
	});
	client.run("Bot " TOKEN);
	return EXIT_SUCCESS;
}
