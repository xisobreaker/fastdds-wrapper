// =====================================================================================
//  Copyright (C) 2024 by Jiaxing Shao. All rights reserved
//
//  文 件 名:  DDSTopicDataReader.hpp
//  作    者:  Jiaxing Shao, 980853650@qq.com
//  创建时间:  2024-11-22 16:19:16
//  描    述:
// =====================================================================================

#ifndef DDS_TOPIC_DATAREADER_H_H_H
#define DDS_TOPIC_DATAREADER_H_H_H

#include "DDSDataReaderListener.hpp"
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/topic/Topic.hpp>

template <typename T>
class DDSTopicDataReader
{
    typedef std::function<void(const std::string &, std::shared_ptr<T>)> OnMessageCallback;

public:
    DDSTopicDataReader(eprosima::fastdds::dds::Subscriber          *subscriber,
                       eprosima::fastdds::dds::Topic               *topic,
                       OnMessageCallback                            callback,
                       const eprosima::fastdds::dds::DataReaderQos &dataReaderQos);
    ~DDSTopicDataReader();

private:
    DDSDataReaderListener<T>            m_readerListener;
    eprosima::fastdds::dds::DataReader *m_dataReader;
    eprosima::fastdds::dds::Subscriber *m_subscriber;
};

template <typename T>
DDSTopicDataReader<T>::DDSTopicDataReader(eprosima::fastdds::dds::Subscriber          *subscriber,
                                          eprosima::fastdds::dds::Topic               *topic,
                                          OnMessageCallback                            callback,
                                          const eprosima::fastdds::dds::DataReaderQos &dataReaderQos) : m_subscriber(subscriber)
{
    m_readerListener.m_callback = callback;
    m_dataReader = subscriber->create_datareader(topic, dataReaderQos, &m_readerListener);
}

template <typename T>
DDSTopicDataReader<T>::~DDSTopicDataReader()
{
    if (m_dataReader)
        m_subscriber->delete_datareader(m_dataReader);
}

#endif