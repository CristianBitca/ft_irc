/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbitca <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 11:02:07 by cbitca            #+#    #+#             */
/*   Updated: 2026/05/05 18:02:18 by cbitca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <unistd.h>
#include <vector>
#include <map>
#include <iostream>
#include <stdexcept>
#include <string>
#include <fcntl.h>
#include <signal.h>
#include <sstream>
#include "Client.hpp"
#include "Channel.hpp"

class Server
{
private:
    std::string                    _password;
    int                            _serverSocket;
    sockaddr_in                    _serverAddres;
    pollfd                         _serverPollFd;
    std::vector<pollfd>            _fds;
    std::map<int, Client>          _clients;
    std::map<std::string, Channel> _channels;

    void    handleNewClient();
    bool    handleClientData(size_t index);
    void    disconnectClient(size_t index);
    void    handleCommand(size_t index, const std::string &line);
    void    checkRegister(size_t index);

    void    handlePass(size_t index, const std::vector<std::string> &tokens);
    void    handleNick(size_t index, const std::vector<std::string> &tokens);
    void    handleUser(size_t index, const std::vector<std::string> &tokens);
    void    handleJoin(size_t index, const std::vector<std::string> &tokens);
    // void    handle(size_t index, const std::vector<std::string> &tokens);
    // void    handle(size_t index, const std::vector<std::string> &tokens);
    // void    handle(size_t index, const std::vector<std::string> &tokens);
    // void    handle(size_t index, const std::vector<std::string> &tokens);
    // void    handle(size_t index, const std::vector<std::string> &tokens);
    // void    handle(size_t index, const std::vector<std::string> &tokens);
    
public:
    Server(int port,const std::string &pass);
    ~Server();

    void    run();
};

#endif