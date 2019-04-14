//
// Created by Gegel85 on 13/04/2019.
//

#ifndef DISC_ORD_PLATFORM_HPP
#define DISC_ORD_PLATFORM_HPP

#ifdef _WIN32
#	define PLATFORM_NAME "windows"
#	define _WINDOWS_DISCXXORD
#elif __APPLE__
#	define PLATFORM_NAME "macos"
#	define _MAC_DISCXXORD
#elif __linux__
#	define PLATFORM_NAME "linux"
#	define _OTHER_DISCXXORD
#elif __unix__
#	define PLATFORM_NAME "unix"
#	define _OTHER_DISCXXORD
#elif defined(_POSIX_VERSION)
#	define PLATFORM_NAME "posix"
#	define _OTHER_DISCXXORD
#else
#   error "Unknown platform or compiler"
#endif

#endif //DISC_ORD_PLATFORM_HPP
