/*
** EPITECH PROJECT, 2019
** DisCXXord
** File description:
** Embed.hpp
*/

#ifndef DISCXXORD_EMBED_HPP
#define DISCXXORD_EMBED_HPP


#include "Date.hpp"

namespace DisCXXord
{
	struct EmbedFooter {
		std::string		text = "";
		Optional<std::string>	iconUrl = {};
		Optional<std::string>	proxyIconUrl = {};
	};

	struct EmbedImage {
		Optional<std::string>	url = {};
		Optional<std::string>	proxyUrl = {};
		Optional<int>		height = {};
		Optional<int>		width = {};
	};

	struct EmbedVideo {
		Optional<std::string>	url = {};
		Optional<int>		height = {};
		Optional<int>		width = {};
	};

	struct EmbedProvider {
		Optional<std::string>	url = {};
		Optional<std::string>	name = {};
	};

	struct EmbedAuthor {
		Optional<std::string>	iconUrl = {};
		Optional<std::string>	proxyIconUrl = {};
		Optional<std::string>	url = {};
		Optional<std::string>	name = {};
	};

	struct EmbedField {
		std::string	name = "";
		std::string	value = "";
		Optional<bool>	isInline;
	};

	struct Embed {
		Optional<std::string>			title = {};
		Optional<std::string>			type = {};
		Optional<std::string>			description = {};
		Optional<std::string>			url = {};
		Optional<Date>				timestamp = {};
		Optional<int>				color = {};
		Optional<EmbedFooter>			footer = {};
		Optional<EmbedImage>			image = {};
		Optional<EmbedImage>			thumbnail = {};
		Optional<EmbedVideo>			video = {};
		Optional<EmbedProvider>			provider = {};
		Optional<EmbedAuthor>			author = {};
		Optional<std::vector<EmbedField>>	fields = {};

		Embed() = default;
	};
}


#endif //DISCXXORD_EMBED_HPP
