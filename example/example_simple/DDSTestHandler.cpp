#include "DDSTestHandler.h"
#include "DDSConstants.h"
#include "DDSTopicDataReader.hpp"
#include "DDSTopicDataWriter.hpp"
#include "HelloWorldOne.h"
#include "HelloWorldOnePubSubTypes.h"
#include "HelloWorldTwo.h"
#include "HelloWorldTwoPubSubTypes.h"
#include "ParticipantQosHandler.h"
#include <chrono>
#include <fastdds/dds/topic/TopicDataType.hpp>
#include <fastdds/dds/topic/qos/TopicQos.hpp>
#include <functional>
#include <glog/logging.h>
#include <unordered_map>
#include <utility>

using namespace eprosima::fastdds::dds;
using namespace eprosima::fastdds::rtps;
using namespace eprosima::fastrtps::rtps;

DDSTestHandler::DDSTestHandler(int domainId) : DDSParticipantManager(domainId)
{
    addTopicDataTypeCreator(DDS_TOPIC_HELLO_WORLD_ONE, []() {
        return new HelloWorldOnePubSubType();
    });
    addTopicDataTypeCreator(DDS_TOPIC_HELLO_WORLD_TWO, []() {
        return new HelloWorldTwoPubSubType();
    });
}

DDSTestHandler::~DDSTestHandler()
{
}

ParticipantQosHandler DDSTestHandler::createParticipantQos(const std::string              &participant_name,
                                                           uint16_t                        listen_port,
                                                           const std::vector<std::string> &peer_locators)
{
    ParticipantQosHandler participantQos(participant_name);
    participantQos.addUDPV4Transport(1024 * 1024 * 16);
    // participantQos.addUDPV6Transport(1024 * 1024 * 16);
    // participantQos.addSHMTransport(1024 * 1024 * 16);
    // participantQos.addTCPV4Transport(listen_port, peer_locators);
    return participantQos;
}