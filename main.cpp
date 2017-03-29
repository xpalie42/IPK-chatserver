#include <iostream>
#include "ConnectionListener.h"
#include <thread>
#include <mutex>
#include <vector>
#include <time.h>

#define RESET   "\033[0m"
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */

using namespace std;

string colors[6] = {BOLDRED, BOLDGREEN, BOLDYELLOW, BOLDBLUE, BOLDMAGENTA, BOLDCYAN};

mutex *syn; // vylucny pristup ku zapisu na sockety
vector<int> fds; // file descriptory klientov

/* Repeats sending bytes until entire buffer is sent */
void send_buffer(int fd, const char *buffer, size_t size) {
    while (size > 0) {
        ssize_t retval = send(fd, buffer, size, 0);
        if (retval <= 0)
            throw string("Error writing to socket.");
        else {
            size -= retval;
            buffer += retval;
        }
    }
}

string read_until_crlf(int fd) {
    string message = "";
    char c;
    int state = 0;
    syn = new mutex();
    while((recv(fd, &c, 1, 0)) > 0) {
        message.append(&c, 1);
        if(c == '\r' && state == 0)
            state++;
        else if(c == '\n' && state == 1) {
            return message;
        } else if(state > 0)
            state = 0;
    }
    throw string("Socket read error.");
}

void clientThreadHandler(int fd) {
    string msg;
    srand(time(NULL));
    string color = colors[rand() % 6];
    while(true) {
        try {
            msg = read_until_crlf(fd);
        } catch(string e) {
            syn->lock();
            // najde seba v zozname descriptorov a odoberie sa
            for(int i = 0; i < fds.size(); i++)
                if(fds[i] == fd) {
                    fds.erase(fds.begin() + i);
                    break;
                }
            syn->unlock();
            return;
        }
        size_t colonindex = msg.find(": ");
        if(colonindex != string::npos) {
            msg = color + msg.substr(0, colonindex) + RESET + msg.substr(colonindex);
        }
        syn->lock();
            cout << msg;
            // posle prijatu spravu vsetkym klientom
            for(int i = 0; i < fds.size(); i++) {
                if(fds[i] != fd && fds[i] >= 0)
                    send_buffer(fds[i], msg.c_str(), msg.size());
            }
        syn->unlock();
    }
}

int main() {
    ConnectionListener listener(21011);
    listener.start();
    int newfd;
    thread *userThread;
    syn = new mutex();
    while((newfd = listener.acceptConnection())) {
        syn->lock();
            fds.push_back(newfd);
        syn->unlock();
        userThread = new thread(clientThreadHandler, newfd);
        userThread->detach();
    }
    return 0;
}
