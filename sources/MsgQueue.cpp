/*
** EPITECH PROJECT, 2023
** B-CCP-400-PAR-4-1-theplazza-marouan.bader
** File description:
** MsgQueue
*/

#include "MsgQueue.hpp"
#include <cstring>
#include <unistd.h>

IPCMessageQueue::IPCMessageQueue(const std::string &name, int flag, bool unlink) : queueName(name)
{
    struct mq_attr attr;

    if (unlink)
        mq_unlink(queueName.c_str());
    queueDescriptor = mq_open(queueName.c_str(), flag, 0666, nullptr);
    if (queueDescriptor == -1)
        throw std::runtime_error("Error while creating message queue");
    if (mq_getattr(queueDescriptor, &attr) == -1)
        throw std::runtime_error("Error while creating message queue");
    attr.mq_maxmsg = sysconf(_SC_MQ_PRIO_MAX);
    attr.mq_msgsize = attr.mq_msgsize * attr.mq_maxmsg;
    if (mq_setattr(queueDescriptor, &attr, nullptr) == -1)
        throw std::runtime_error("Error while creating message queue");
}

IPCMessageQueue::~IPCMessageQueue()
{
    mq_close(queueDescriptor);
    mq_unlink(queueName.c_str());
}

void IPCMessageQueue::empty_queue() const
{
    std::string message;

    while (receive(message));
}

void IPCMessageQueue::send(const std::string &message, unsigned int priority)
{
    int ret_value = 0;

    while (1) {
        ret_value = mq_send(queueDescriptor, message.c_str(), message.size(), priority);
        if (ret_value == -1 && errno == EAGAIN) {
            continue;
        } else if (ret_value == -1) {
            throw std::runtime_error("Error while sending message");
        } else
            break;
    }
}

bool IPCMessageQueue::receive(std::string &str) const
{
    char buffer[8192];
    ssize_t receivedSize = mq_receive(queueDescriptor, buffer, sizeof(buffer), nullptr);

    if (receivedSize == -1) {
        if (errno == EAGAIN)
            return false;
        std::cerr << "Failed to receive message: " << strerror(errno) << std::endl;
        throw std::runtime_error("Error while receiving message");
    }
    str = std::string(buffer, receivedSize);
    return true;
}

IPCMessageQueue &IPCMessageQueue::operator>>(std::string &str) const
{
    while (receive(str) == false)
        usleep(100);
    return (const_cast<IPCMessageQueue &>(*this));
}

IPCMessageQueue &IPCMessageQueue::operator<<(const std::string &str)
{
    send(str);
    return *this;
}

std::ostream &operator<<(std::ostream &os, const IPCMessageQueue &queue)
{
    std::string str;

    queue >> str;
    os << str;
    return os;
}
