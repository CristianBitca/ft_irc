/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbitca <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 11:02:04 by cbitca            #+#    #+#             */
/*   Updated: 2026/05/05 11:56:12 by cbitca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>
#include <algorithm>
#include <signal.h>

int main(int argc, char **argv)
{
    if (argc != 3){
        std::cerr << "Error: Expected format ./ircserv <port> <password>" << std::endl;
        return (1);
    }
    int port = std::atoi(argv[1]);
    std::string pass = argv[2];
    if (port <= 1024 || port > 65535)
    {
        std::cerr << "Error: Port must be between 1024 and 65535" << std::endl;
        return (1);
    }
    try
    {
        Server ircserv(port, pass);
        ircserv.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return (0);
}