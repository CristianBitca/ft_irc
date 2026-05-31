/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbitca <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 12:04:18 by cbitca            #+#    #+#             */
/*   Updated: 2026/05/05 17:59:22 by cbitca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>

class Channel
{
private:
    std::string _name;
    std::string _topic;
    std::string _key;
    std::vector<int> _members;
    std::vector<int> _operators;
    bool    _inviteOnly;
    bool    _topicRestricted;
    int     _userLimits;
public:
    Channel();
    Channel(const std::string &name);
    ~Channel();

    std::string getName()   const;
    std::string getTopic()   const;
    std::string getKey()   const;
    int         getUserLimits() const;
    bool        isInviteOnly() const;
    bool        isTopicRestricted() const;

    bool    isMember(int fd)  const;
    bool    isOperator(int fd) const;
    void    addMember(int fd);
    void    removeMember(int fd);
    void    addOperator(int fd);
    void    removeOperator(int fd);

    void    setTopic(const std::string &topic);
    void    setKey(const std::string &key);
    void    setInviteOnly(bool val);
    void    setTopicRestricted(bool val);
    void    setUserLimit(int limit);
};

#endif