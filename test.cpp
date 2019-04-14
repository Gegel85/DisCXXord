#include <iostream>
#include "Client.hpp"
#include "token.hpp"

int	main()
{
	DisCXXord::Client	client("./disc++ord.log", DisCXXord::Logger::DEBUG);

	client.run(TOKEN);
	return EXIT_SUCCESS;
}