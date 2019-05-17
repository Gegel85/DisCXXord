//
// Created by Gegel85 on 13/04/2019.
//

#ifndef DISC_ORD_PLATFORM_HPP
#define DISC_ORD_PLATFORM_HPP

#ifdef _WIN32
#	define PLATFORM_NAME "windows"
#	define _WINDOWS_DISCXXORD
#elif defined __APPLE__
#	define PLATFORM_NAME "macos"
#	define _MAC_DISCXXORD
#elif defined __linux__
#	define PLATFORM_NAME "linux"
#	define _OTHER_DISCXXORD
#elif defined __unix__
#	define PLATFORM_NAME "unix"
#	define _OTHER_DISCXXORD
#elif defined(_POSIX_VERSION)
#	define PLATFORM_NAME "posix"
#	define _OTHER_DISCXXORD
#else
#	warning "Unknown platform or compiler"
#	define PLATFORM_NAME "unknown"
#	define _OTHER_DISCXXORD
#endif

#endif //DISC_ORD_PLATFORM_HPP
