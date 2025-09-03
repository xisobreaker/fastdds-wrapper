#pragma once

#include <fastdds/dds/domain/DomainParticipantListener.hpp>

class DDSParticipantListener : public eprosima::fastdds::dds::DomainParticipantListener
{
public:
    void on_participant_discovery(eprosima::fastdds::dds::DomainParticipant           *participant,
                                  eprosima::fastrtps::rtps::ParticipantDiscoveryInfo &&info) override
    {
        if (info.status == eprosima::fastrtps::rtps::ParticipantDiscoveryInfo::DISCOVERED_PARTICIPANT) {
            std::cout << "Discovered participant " << info.info.m_participantName << std::endl;
        } else if (info.status == eprosima::fastrtps::rtps::ParticipantDiscoveryInfo::REMOVED_PARTICIPANT) {
            std::cout << "Removed participant " << info.info.m_participantName << std::endl;
        } else if (info.status == eprosima::fastrtps::rtps::ParticipantDiscoveryInfo::DROPPED_PARTICIPANT) {
            std::cout << "Dropped participant " << info.info.m_participantName << std::endl;
        }
    }
};