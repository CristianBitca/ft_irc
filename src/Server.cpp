/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbitca <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 11:01:56 by cbitca            #+#    #+#             */
/*   Updated: 2026/05/05 18:36:36 by cbitca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool s_running = true;

void signalHandler(int sig)
{
    (void)sig;
    s_running = false;
}

Server::Server(int port, const std::string &pass) : _password(pass)
{
    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverSocket < 0)
        throw std::runtime_error("Error in creating the server");
    fcntl(_serverSocket, F_SETFL, O_NONBLOCK);
    int opt = 1;
    setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    _serverAddres.sin_family = AF_INET;
    _serverAddres.sin_port = htons(port);
    _serverAddres.sin_addr.s_addr = INADDR_ANY;
    if (bind(_serverSocket, (struct sockaddr *) &_serverAddres, sizeof(_serverAddres)) < 0)
        throw std::runtime_error("Failed to bind");
    if (listen(_serverSocket, 50))
         throw std::runtime_error("Failed to listen");
    _serverPollFd.fd = _serverSocket;
    _serverPollFd.events = POLLIN;
    _fds.push_back(_serverPollFd);
}

Server::~Server()
{
    for (size_t i = 0; i < _fds.size(); i++)
        close(_fds[i].fd);
}

void    Server::run()
{
    signal(SIGINT, signalHandler);
    while(s_running)
    {
        poll(_fds.data(), _fds.size(), -1);
        for (size_t i = 0; i < _fds.size(); i++)
        {
            if (_fds[i].revents & POLLIN)
            {
                if (_fds[i].fd == _serverSocket)
                    handleNewClient();
                else
                {
                    if (handleClientData(i))
                        i--;
                }
            }
            _fds[i].revents = 0;
        }
    }
}

void    Server::handleNewClient()
{
    int client = accept(_serverSocket, 0, 0);
    if (client < 0)
    {
        std::cerr << "Failed to accept client" << std::endl;
        return ;
    }
    pollfd  clientFd;
    clientFd.fd = client;
    clientFd.events = POLLIN;
    _fds.push_back(clientFd);
    _clients.insert(std::make_pair(clientFd.fd, Client(clientFd.fd)));
}

bool    Server::handleClientData(size_t i)
{
    char buffer[512];
    int bytes;
    bytes = recv(_fds[i].fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes > 0)
    {
        buffer[bytes] = 0;
        std::vector<std::string> lines = _clients[_fds[i].fd].appendData(buffer);
        for (size_t j = 0; j < lines.size(); j++)
        {
            handleCommand(i, lines[j]);
            if (_clients.find(_fds[i].fd) == _clients.end())
                return true;
        }
    }
    else if (bytes == 0)
    {
        close(_fds[i].fd);
        _clients.erase(_fds[i].fd);
        _fds.erase(_fds.begin() + i);
        return true;
    }
    return false;
}

void    Server::disconnectClient(size_t i)
{
    close(_fds[i].fd);
    _clients.erase(_fds[i].fd);
    _fds.erase(_fds.begin() + i);

}

void    Server::handleCommand(size_t i, const std::string &line)
{
    std::istringstream  stream(line);
    std::vector<std::string>    tokens;
    std::string token;

    while (stream >> token)
    {
        if (token[0] == ':')
        {
            std::string trailing;
            std::getline(stream, trailing);
            tokens.push_back(token.substr(1) + trailing);
            break;
        }
        tokens.push_back(token);
    }
    if (tokens.empty())
        return ;
    std::string command = tokens[0];
    if (command == "PASS")
        handlePass(i, tokens);
    if (command == "NICK")
        handleNick(i, tokens);
    if (command == "USER")
        handleUser(i, tokens);
    if (command == "JOIN")
        handleJoin(i, tokens);
}

void    Server::checkRegister(size_t i)
{
    Client &client = _clients[_fds[i].fd];
    if (client.isAuth() && !client.getNickname().empty() && !client.getUsenname().empty())
    {
        client.doRegister();
        std::string msg = ":server 001 " + client.getNickname() + " :Welcome to the IRC server!\n";
        send(_fds[i].fd, msg.c_str(), msg.size(), 0);
        return ;
    }
}

void    Server::handlePass(size_t i, const std::vector<std::string> &tokens)
{
    Client &client = _clients[_fds[i].fd];

    if (client.isRegistred())
    {
        std::string msg = ":server 462 * :You may not reregister\n";
        send(_fds[i].fd, msg.c_str(), msg.size(), 0);
        return ;
    }
    if (tokens.size() < 2)
    {
        std::string msg = ":server 461 PASS :Not enough parameters\n";
        send(_fds[i].fd, msg.c_str(), msg.size(), 0);
        return ;
    }
    if (tokens[1] != _password)
    {
        std::string msg = ":server 464 * :password incorrect\n";
        send(_fds[i].fd, msg.c_str(), msg.size(), 0);
        disconnectClient(i);
        return ;
    }
    client.doAuth();
}

void    Server::handleNick(size_t i, const std::vector<std::string> &tokens)
{
    Client &client = _clients[_fds[i].fd];

    if (!client.isAuth())
    {
        std::string msg = ":server 464 * :password required\n";
        send(_fds[i].fd, msg.c_str(), msg.size(), 0);
        return ;
    }
    if (tokens.size() < 2)
    {
        std::string msg = ":server 431 * :No nickname given\n";
        send(_fds[i].fd, msg.c_str(), msg.size(), 0);
        return ;
    }
    std::map<int, Client>::iterator it;
    for (it = _clients.begin(); it != _clients.end(); it++)
    {
        if (it->first != _fds[i].fd && it ->second.getNickname() == tokens[1])
        {
            std::string msg = "server 433 * " + tokens[1] + " :Nickname already in use\n";
            send(_fds[i].fd, msg.c_str(), msg.size(), 0);
            return ;
        }
    }
    client.setNickname(tokens[1]);
    checkRegister(i);
}

void    Server::handleUser(size_t i, const std::vector<std::string> &tokens)
{
    Client &client = _clients[_fds[i].fd];

    if (!client.isAuth())
    {
        std::string msg = ":server 464 * :password required\n";
        send(_fds[i].fd, msg.c_str(), msg.size(), 0);
        return ;
    }
    if (tokens.size() < 2)
    {
        std::string msg = ":server 431 * :No username given\n";
        send(_fds[i].fd, msg.c_str(), msg.size(), 0);
        return ;
    }
    client.setUsername(tokens[1]);
    checkRegister(i);   
}

void    Server::handleJoin(size_t i, const std::vector<std::string> &tokens)
{
    Client &client = _clients[_fds[i].fd];

    if (!client.isRegistred())
    {
        std::string msg = ":server 464 * :registration required\n";
        send(_fds[i].fd, msg.c_str(), msg.size(), 0);
        return ;
    }
    if (tokens.size() < 2)
    {
        std::string msg = ":server 431 * :No channel name given\n";
        send(_fds[i].fd, msg.c_str(), msg.size(), 0);
        return ;
    }
    if (tokens[1].empty() || tokens[1][0] != '#')
    {
        std::string msg = ":server 403 " + client.getNickname() + " " + tokens[1] + " :No such channel\n";
        send(_fds[i].fd, msg.c_str(), msg.size(), 0);
        return ;
    }
    if (_channels.find(tokens[1]) == _channels.end())
    {
        
    }
    
}