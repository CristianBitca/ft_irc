/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbitca <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 13:11:29 by cbitca            #+#    #+#             */
/*   Updated: 2026/05/05 18:01:15 by cbitca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel()
{
}

Channel::Channel(const std::string &name)
    : _name(name),
    _inviteOnly(false),
    _topicRestricted(false),
    _userLimits(0) 
{
    
}

Channel::~Channel()
{
}

std::string Channel::getName()  const
{
    return _name;
}

void    Channel::addMember(int fd)
{
    _members.push_back(fd);
}

void    Channel::removeMember(int fd)
{
    for (size_t i = 0; i < _members.size(); i++)
    {
        if (_members[i] == fd)
        {
            _members.erase(_members.begin() + i);
            break ;
        }
    }   
}

bool    Channel::isMember(int fd)    const
{
    for (size_t i = 0; i < _members.size(); i++)
    {
        if (_members[i] == fd)
        {
            return true;
        }
    }
    return false;
}

void    Channel::addOperator(int fd)
{
    _operators.push_back(fd);
}

void    Channel::removeOperator(int fd)
{
    for (size_t i = 0; i < _operators.size(); i++)
    {
        if (_operators[i] == fd)
        {
            _operators.erase(_operators.begin() + i);
            break ;
        }
    }   
}

bool    Channel::isOperator(int fd)    const
{
    for (size_t i = 0; i < _operators.size(); i++)
    {
        if (_operators[i] == fd)
        {
            return true;
        }
    }
    return false;
}
