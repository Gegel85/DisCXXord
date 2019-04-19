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
	for (const std::string &id : client.guilds())
		std::cout << "\t" << id << std::endl;
}

int	main()
{
	Client	client("./disc++ord.log", Logger::DEBUG);

	client.setHandlers({
		.ready = ready
	});
	client.run("Bot " TOKEN);
	return EXIT_SUCCESS;
}