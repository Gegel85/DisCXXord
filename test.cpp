#include <iostream>
#include "Client.hpp"
#include "token.hpp"

using namespace DisCXXord;

void	ready(Client &client)
{
	const User &me = client.me();

	std::cout << "Connected on " << me.tag() << std::endl;
	std::cout << "My avatar url is " << me.avatarURL() << std::endl;
}

int	main()
{
	Client	client("./disc++ord.log", Logger::DEBUG);

	client.setHandlers({
		.ready = ready
	});
	client.run(TOKEN);
	return EXIT_SUCCESS;
}