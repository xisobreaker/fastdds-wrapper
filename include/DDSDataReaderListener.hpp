#pragma once

#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/topic/TopicDescription.hpp>
#include <glog/logging.h>

template <typename T>
class DDSDataReaderListener : public eprosima::fastdds::dds::DataReaderListener
{
    typedef std::function<void(std::shared_ptr<T>)> OnMessageCallback;

protected:
    void on_subscription_matched(eprosima::fastdds::dds::DataReader *reader, const eprosima::fastdds::dds::SubscriptionMatchedStatus &info)
    {
        const eprosima::fastdds::dds::TopicDescription *topic = reader->get_topicdescription();

        if (info.current_count_change == 1) {
            LOG(INFO) << "subscription " << topic->get_name() << " matched";
        } else if (info.current_count_change == -1) {
            LOG(INFO) << "subscription " << topic->get_name() << " unmatched";
        }
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
            m_callback(message);
        } else {
            LOG(ERROR) << "take_next_sample error";
        }
    }

public:
    OnMessageCallback m_callback;
};