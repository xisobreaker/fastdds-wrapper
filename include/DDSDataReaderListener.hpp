// =====================================================================================
//  Copyright (C) 2024 by Jiaxing Shao. All rights reserved
//
//  文 件 名:  DDSDataReaderListener.hpp
//  作    者:  Jiaxing Shao, 980853650@qq.com
//  创建时间:  2024-11-22 15:46:03
//  描    述:
// =====================================================================================

#ifndef DDS_DATAREADER_LISTENER_H_H_H
#define DDS_DATAREADER_LISTENER_H_H_H

#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/topic/TopicDescription.hpp>

#define GLOG_USE_GLOG_EXPORT
#include <glog/logging.h>

template <typename T>
class DDSDataReaderListener : public eprosima::fastdds::dds::DataReaderListener
{
    typedef std::function<void(const std::string &, std::shared_ptr<T>)> OnMessageCallback;

protected:
    void on_subscription_matched(eprosima::fastdds::dds::DataReader *reader, const eprosima::fastdds::dds::SubscriptionMatchedStatus &info)
    {
        const eprosima::fastdds::dds::TopicDescription *topic = reader->get_topicdescription();
        LOG(INFO) << "last_publication_handle: " << info.last_publication_handle;
        LOG(INFO) << "subscription " << topic->get_name() << (info.current_count_change == 1 ? " matched" : " unmatched");
    }

    void on_liveliness_changed(eprosima::fastdds::dds::DataReader *reader, const eprosima::fastrtps::LivelinessChangedStatus &status)
    {
        LOG(INFO) << "on_liveliness_changed: " << status.alive_count;
    }

    void on_data_available(eprosima::fastdds::dds::DataReader *reader)
    {
        eprosima::fastdds::dds::SampleInfo info;
        std::shared_ptr<T>                 message = std::make_shared<T>();
        ReturnCode_t                       retCode = reader->take_next_sample(message.get(), &info);
        if (retCode == ReturnCode_t::RETCODE_OK && info.valid_data) {
            m_callback(reader->get_topicdescription()->get_name(), message);
        } else {
            LOG(ERROR) << "take_next_sample error";
        }
    }

public:
    OnMessageCallback m_callback;
};

#endif