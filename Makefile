CFLAGS = -std=c++11 -pthread

SERVERSRC = main.cpp ConnectionListener.h ConnectionListener.cpp

all: ipk_chat_server

ipk_chat_server: $(SERVERSRC)
	g++ $(CFLAGS) $^ -o ipk_server

clean:
	rm ipk_server
