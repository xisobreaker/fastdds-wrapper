// =====================================================================================
//  Copyright (C) 2024 by Jiaxing Shao. All rights reserved
//
//  文 件 名:  DDSParticipantManager.h
//  作    者:  Jiaxing Shao, 980853650@qq.com
//  创建时间:  2024-11-22 16:19:05
//  描    述:
// =====================================================================================

#ifndef DDS_PARTICIPANT_MANAGER_H_H_H
#define DDS_PARTICIPANT_MANAGER_H_H_H

#include "DDSDomainParticipant.h"
#include "DDSTopicDataReader.hpp"
#include "DDSTopicDataWriter.hpp"
#include "ParticipantQosHandler.h"
#include <fastdds/dds/topic/TopicDataType.hpp>
#include <memory>

class DDSParticipantManager
{
    typedef std::function<eprosima::fastdds::dds::TopicDataType *()> TopicDataTypeCreator;

public:
    DDSParticipantManager(int domainId);
    virtual ~DDSParticipantManager();

protected:
    eprosima::fastdds::dds::TopicDataType *getTopicDataType(std::string topicName);
    void                                   addTopicDataTypeCreator(std::string topicName, TopicDataTypeCreator creator);
    virtual ParticipantQosHandler          createParticipantQos(const std::string              &participant_name,
                                                                uint16_t                        listen_port,
                                                                const std::vector<std::string> &peer_locators) = 0;

public:
    bool initDomainParticipant(const std::string              &participant_name,
                               uint16_t                        listen_port = 0,
                               const std::vector<std::string> &peer_locators = {});

    template <typename T>
    DDSTopicDataWriter<T> *createDataWriter(std::string topicName);

    template <typename T>
    DDSTopicDataReader<T> *createDataReader(std::string topicName, std::function<void(const std::string &, std::shared_ptr<T>)> callback);

private:
    int                                   m_domainId;
    std::shared_ptr<DDSDomainParticipant> m_participant;

private:
    std::unordered_map<std::string, std::function<eprosima::fastdds::dds::TopicDataType *()>> m_topicTypes;
};

template <typename T>
DDSTopicDataWriter<T> *DDSParticipantManager::createDataWriter(std::string topicName)
{
    if (!m_participant->registerTopic(topicName, getTopicDataType(topicName)))
        return nullptr;

    return m_participant->createDataWriter<T>(topicName);
}

template <typename T>
DDSTopicDataReader<T> *DDSParticipantManager::createDataReader(std::string                                                  topicName,
                                                               std::function<void(const std::string &, std::shared_ptr<T>)> callback)
{
    if (!m_participant->registerTopic(topicName, getTopicDataType(topicName)))
        return nullptr;

    return m_participant->createDataReader(topicName, callback);
}

#endif