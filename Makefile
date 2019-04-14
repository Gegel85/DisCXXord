STATICNAME=	libdisc++ord-static.a

WINSONAME =	libdisc++ord-dynamic.dll

LINSONAME =	libdisc++ord-dynamic.so

SRC =	SecuredSocket.cpp	\
		SecuredWebSocket.cpp\
		Socket.cpp			\
		Logger.cpp			\
		Client.cpp			\

OBJ =	$(SRC:%.cpp=%.o)

CXX =	g++

CXXFLAGS = -W -Wall -Wextra -std=c++17 -fPIC -I lib/json_parser/include

LD_FLAGS = -L lib/json_parser -ljson_parser_cpp -ljson_parser -lssl -lws2_32

help:
	echo use 'make linux' if you are on linux and 'make windows' if on windows

lib/json_parser/libjson_parser_cpp.a:
	$(MAKE) -C lib/json_parser all

$(STATICNAME):	$(OBJ) lib/json_parser/libjson_parser_cpp.a
		$(AR) rc $(STATICNAME) $(OBJ)

$(WINSONAME):	$(OBJ) lib/json_parser/libjson_parser_cpp.a
		$(CXX) -o $(WINSONAME) $(OBJ) -shared $(LD_FLAGS)

$(LINSONAME):	$(OBJ)lib/json_parser/libjson_parser_cpp.a
		$(CXX) -o $(LINSONAME) $(OBJ) -shared $(LD_FLAGS)

windows:	$(STATICNAME) $(WINSONAME)

linux:		$(STATICNAME) $(LINSONAME)

clean:
		$(RM) $(OBJ) $(LINSONAME) $(WINSONAME) $(STATICNAME)

fclean:	clean
		$(MAKE) -C lib/json_parser fclean

dbg:	CXXFLAGS += -g -O0
dbg:	clean windows