#ifndef MSGQUEUE_HPP_
#define MSGQUEUE_HPP_

#include <iostream>
#include <string>
#include <stdexcept>

#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

class IPCMessageQueue
{
private:
    mqd_t queueDescriptor;
    std::string queueName;

public:
    IPCMessageQueue(const std::string &name, int flag = O_RDWR | O_CREAT | O_NONBLOCK, bool unlink = true);

    ~IPCMessageQueue();

    void send(const std::string &message, unsigned int priority = 0);
    IPCMessageQueue &operator>>(std::string &str) const;
    IPCMessageQueue &operator<<(const std::string &str);
    bool receive(std::string &str) const;
    void empty_queue() const;
};

std::ostream &operator<<(std::ostream &os, const IPCMessageQueue &queue);

#endif /* !MSGQUEUE_HPP_ */