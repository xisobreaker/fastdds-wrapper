#pragma once

#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/dds/topic/Topic.hpp>
#include <glog/logging.h>

class DDSDataWriterListener : public eprosima::fastdds::dds::DataWriterListener
{
protected:
    void on_publication_matched(eprosima::fastdds::dds::DataWriter *writer, const eprosima::fastdds::dds::PublicationMatchedStatus &info)
    {
        const eprosima::fastdds::dds::Topic *topic = writer->get_topic();

        if (info.current_count_change == 1) {
            LOG(INFO) << "publication " << topic->get_name() << " matched";
        } else if (info.current_count_change == -1) {
            LOG(INFO) << "publication " << topic->get_name() << " unmatched";
        }
    }

    void on_offered_deadline_missed(eprosima::fastdds::dds::DataWriter                        *writer,
                                    const eprosima::fastdds::dds::OfferedDeadlineMissedStatus &status)
    {
        LOG(INFO) << "on_offered_deadline_missed: " << status.total_count;
    }

    void on_liveliness_lost(eprosima::fastdds::dds::DataWriter *writer, const eprosima::fastdds::dds::LivelinessLostStatus &status)
    {
        LOG(INFO) << "on_liveliness_lost: " << status.total_count;
    }
};
