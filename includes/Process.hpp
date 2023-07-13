/*
** EPITECH PROJECT, 2023
** B-CCP-400-PAR-4-1-theplazza-marouan.bader
** File description:
** Process
*/

#ifndef PROCESS_HPP_
    #define PROCESS_HPP_
    #include <sys/types.h>
    #include <unistd.h>
    #include "MsgQueue.hpp"
    #include <iostream>
    #include <signal.h>

class Process {
    public:
        IPCMessageQueue _returnQueue;
        class ProcessException : public std::exception {
            public:
                ProcessException(std::string msg) noexcept : _msg(msg) {}
                const char *what() const noexcept override {return _msg.c_str();};
            private:
                std::string _msg;
        };
        Process(size_t kitchen_id) : _returnQueue("/returnQueue" + std::to_string(kitchen_id)) {
            _pid = fork();
            if (_pid == -1)
                throw ProcessException("Process error: Can't create process for la pizza de la mama!");
        }
        bool is_child() const {return _pid == 0;}
        pid_t get_pid() const {return _pid;}
        bool child_is_alive() const {
            if (is_child())
                throw ProcessException("Process error: Can't kill child because calling process is child");
            if (kill(_pid, 0) == 0)
                return true;
            else
                return false;
        }
        void terminate_child() {
            if (is_child())
                throw ProcessException("Process error: Can't kill child because calling process is child");
            if (kill(_pid, SIGTERM) == 0)
                return;
            else
                throw ProcessException("Process error: Error while killing the child");
        }
        void close_process() {
            exit(0);
        }
        ~Process() = default;
    private:
        pid_t _pid;
};

#endif /* !PROCESS_HPP_ */
