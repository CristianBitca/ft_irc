/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbitca <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 20:40:13 by cbitca            #+#    #+#             */
/*   Updated: 2026/05/04 20:27:40 by cbitca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() : _fd(-1), _auth(false), _registered(false)
{
}

Client::Client(int clientFd) : _fd(clientFd), _auth(false), _registered(false)
{
}

Client::~Client()
{
}

std::vector<std::string>    Client::appendData(const std::string &data)
{
    _buffer.append(data);
    std::vector<std::string>    lines;
    size_t pos;
    while ((pos = _buffer.find("\n")) != std::string::npos) 
    {
        std::string line = _buffer.substr(0, pos);
        _buffer.erase(0, pos + 1);
        if (!line.empty())
            lines.push_back(line);
    }
    return lines;
}

bool    Client::isRegistred()   const
{
    return _registered;
}

bool    Client::isAuth()    const
{
    return _auth;
}

void    Client::doRegister()
{
     _registered = true;
}

void    Client::doAuth()
{
    _auth = true;
}

std::string Client::getNickname()   const
{
    return _nickname;
}

std::string Client::getUsenname() const
{
    return _username;
}

void    Client::setNickname(const std::string &nickname)
{
    _nickname = nickname;
}

void    Client::setUsername(const std::string &username)
{
    _username = username;
}