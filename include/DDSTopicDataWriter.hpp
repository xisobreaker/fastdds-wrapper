// =====================================================================================
//  Copyright (C) 2024 by Jiaxing Shao. All rights reserved
//
//  文 件 名:  DDSTopicDataWriter.hpp
//  作    者:  Jiaxing Shao, 980853650@qq.com
//  创建时间:  2024-11-22 16:19:22
//  描    述:
// =====================================================================================

#ifndef DDS_TOPIC_DATAWRITER_H_H_H
#define DDS_TOPIC_DATAWRITER_H_H_H

#include "DDSDataWriterListener.hpp"
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/qos/DataWriterQos.hpp>

template <typename T>
class DDSTopicDataWriter
{
public:
    DDSTopicDataWriter(eprosima::fastdds::dds::Publisher           *publisher,
                       eprosima::fastdds::dds::Topic               *topic,
                       const eprosima::fastdds::dds::DataWriterQos &dataWriterQos);
    ~DDSTopicDataWriter();

public:
    bool writeMessage(const T &message);

private:
    DDSDataWriterListener               m_writerListener;
    eprosima::fastdds::dds::DataWriter *m_dataWriter;
    eprosima::fastdds::dds::Publisher  *m_publisher;
};

template <typename T>
DDSTopicDataWriter<T>::DDSTopicDataWriter(eprosima::fastdds::dds::Publisher           *publisher,
                                          eprosima::fastdds::dds::Topic               *topic,
                                          const eprosima::fastdds::dds::DataWriterQos &dataWriterQos) :
    m_publisher(publisher)
{
    m_dataWriter = publisher->create_datawriter(topic, dataWriterQos, &m_writerListener);
}

template <typename T>
DDSTopicDataWriter<T>::~DDSTopicDataWriter()
{
    m_publisher->delete_datawriter(m_dataWriter);
}

template <typename T>
bool DDSTopicDataWriter<T>::writeMessage(const T &message)
{
    return m_dataWriter->write((void *)&message);
}

#endif