EXAMPLENAME=	example

STATICNAME =	libdisc++ord-static.a

WINSONAME  =	libdisc++ord-dynamic.dll

LINSONAME  =	libdisc++ord-dynamic.so

SRC =	SecuredSocket.cpp		\
		SecuredWebSocket.cpp	\
		Socket.cpp				\
		Logger.cpp				\
		Client.cpp				\
		User.cpp				\
		Snowflake.cpp			\
		Date.cpp				\
		Guild.cpp				\
		Role.cpp				\
		Member.cpp				\
		Request.cpp				\
		Permissions.cpp			\
		Embed.cpp				\
		PartialUser.cpp			\
		Attachement.cpp			\
		Reaction.cpp			\
		Message.cpp				\
		TextChannel.cpp			\
		CategoryChannel.cpp		\
		VoiceChannel.cpp		\
		Channel.cpp				\
		PrivateChannel.cpp		\
		PermissionOverwrite.cpp	\
		SharedPtr.cpp			\

OBJ =	$(SRC:%.cpp=src/%.o)

CXX =	g++

CXXFLAGS = -W -Wall -Wextra -fPIC

LD_FLAGS = -lssl

help:
	echo use 'make linux' if you are on linux and 'make windows' if on windows

$(EXAMPLENAME):
		$(CXX) -o examples/test.o -c examples/test.cpp $(CXXFLAGS) -I include
		$(CXX) -o $(EXAMPLENAME) examples/test.o -L. -ldisc++ord-dynamic

$(STATICNAME):	$(OBJ)
		$(AR) rc $(STATICNAME) $(OBJ)

$(WINSONAME):	$(OBJ)
		$(CXX) -o $(WINSONAME) $(OBJ) -shared $(LD_FLAGS) -lws2_32

$(LINSONAME):	$(OBJ)
		$(CXX) -o $(LINSONAME) $(OBJ) -shared $(LD_FLAGS)

windows:	$(STATICNAME) $(WINSONAME) $(EXAMPLENAME)

linux:		$(STATICNAME) $(LINSONAME) $(EXAMPLENAME)

clean:
		$(RM) $(OBJ) $(LINSONAME) $(WINSONAME) $(STATICNAME)

fclean:	clean

dbgw:	CXXFLAGS += -g -O0
dbgw:	clean windows

dbgl:	CXXFLAGS += -g -O0
dbgl:	clean linux
