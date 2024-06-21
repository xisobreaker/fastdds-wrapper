#pragma once

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
    virtual ParticipantQosHandler          createParticipantQos(std::string participantName) = 0;

public:
    bool initDomainParticipant(std::string participantName);

    template <typename T>
    DDSTopicDataWriter<T> *createDataWriter(std::string topicName);

    template <typename T>
    DDSTopicDataReader<T> *createDataReader(std::string topicName, std::function<void(std::shared_ptr<T>)> callback);

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
DDSTopicDataReader<T> *DDSParticipantManager::createDataReader(std::string topicName, std::function<void(std::shared_ptr<T>)> callback)
{
    if (!m_participant->registerTopic(topicName, getTopicDataType(topicName)))
        return nullptr;

    return m_participant->createDataReader(topicName, callback);
}