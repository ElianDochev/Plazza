#include <iostream>
#include "../includes/MsgQueue.hpp"
#include <unistd.h>

int main()
{
    IPCMessageQueue queue("/test");

    if (fork())
    {
        std::string str;
        queue >> str;
        std::cout << str << std::endl;
        if (queue.receive(str) == false)
            std::cout << "No message" << std::endl;
        else
            std::cout << "First MSG: " << str << " Second msg: " << queue << std::endl;
    }
    else
    {
        sleep(1);
        queue.send("Hello");
        queue.send("World");
        queue.send("!");
    }
    return 0;
}