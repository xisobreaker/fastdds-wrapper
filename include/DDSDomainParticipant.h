#ifndef DDS_DOMAIN_PARTICIPANT_H_H_H
#define DDS_DOMAIN_PARTICIPANT_H_H_H

#include "DDSTopicDataReader.hpp"
#include "DDSTopicDataWriter.hpp"

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/publisher/qos/DataWriterQos.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>

#include <fastdds/dds/topic/TopicDataType.hpp>
#include <mutex>
#include <unordered_map>

class DDSDomainParticipant
{
public:
    DDSDomainParticipant(int domainId, const eprosima::fastdds::dds::DomainParticipantQos &participantQos);
    virtual ~DDSDomainParticipant();

public:
    template <typename T>
    DDSTopicDataWriter<T> *createDataWriter(
        std::string topicName, eprosima::fastdds::dds::DataWriterQos dataWriterQos = eprosima::fastdds::dds::DATAWRITER_QOS_DEFAULT);

    template <typename T>
    DDSTopicDataReader<T> *createDataReader(
        std::string                                  topicName,
        std::function<void(std::shared_ptr<T>)>      callback,
        const eprosima::fastdds::dds::DataReaderQos &dataReaderQos = eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT);

    bool registerTopic(std::string                             topicName,
                       eprosima::fastdds::dds::TopicDataType  *dataType,
                       const eprosima::fastdds::dds::TopicQos &topicQos = eprosima::fastdds::dds::TOPIC_QOS_DEFAULT);

    bool unregisterTopic(std::string topicName);

private:
    eprosima::fastdds::dds::DomainParticipant                       *m_participant;
    eprosima::fastdds::dds::Subscriber                              *m_subscriber;
    eprosima::fastdds::dds::Publisher                               *m_publisher;
    std::unordered_map<std::string, eprosima::fastdds::dds::Topic *> m_mapTopics;
    std::mutex                                                       m_topicLock;
};

template <typename T>
DDSTopicDataWriter<T> *DDSDomainParticipant::createDataWriter(std::string topicName, eprosima::fastdds::dds::DataWriterQos dataWriterQos)
{
    std::lock_guard<std::mutex> guard(m_topicLock);
    if (m_mapTopics.find(topicName) == m_mapTopics.end())
        return nullptr;

    return new DDSTopicDataWriter<T>(m_publisher, m_mapTopics.at(topicName), dataWriterQos);
}

template <typename T>
DDSTopicDataReader<T> *DDSDomainParticipant::createDataReader(std::string                                  topicName,
                                                              std::function<void(std::shared_ptr<T>)>      callback,
                                                              const eprosima::fastdds::dds::DataReaderQos &dataReaderQos)
{
    std::lock_guard<std::mutex> guard(m_topicLock);
    if (m_mapTopics.find(topicName) == m_mapTopics.end())
        return nullptr;

    return new DDSTopicDataReader<T>(m_subscriber, m_mapTopics.at(topicName), callback, dataReaderQos);
}

#endif