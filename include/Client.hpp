/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbitca <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 20:40:07 by cbitca            #+#    #+#             */
/*   Updated: 2026/05/05 18:16:45 by cbitca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <vector>

class Client
{
private:
    int         _fd;
    std::string _nickname;
    std::string _username;
    bool        _auth;
    bool        _registered;
    std::vector<std::string>    _channels;
    std::string _buffer;
public:
    Client();
    Client(int clientFd);
    ~Client();

    std::vector<std::string>  appendData(const std::string &data);
    bool    isRegistred()   const;
    bool    isAuth()    const;
    bool    isInChannel(const std::string &name)   const;
    void    doRegister();
    void    doAuth();
    void    addChannel(const std::string &name);
    void    removeChannel(const std::string &name);

    std::string getNickname()   const;
    std::string getUsenname()   const;
    std::vector<std::string>    getChannels()   const;

    void    setNickname(const std::string &nickname);
    void    setUsername(const std::string &username);
};

#endif