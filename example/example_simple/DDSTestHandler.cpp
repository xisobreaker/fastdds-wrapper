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

ParticipantQosHandler DDSTestHandler::createParticipantQos(std::string participantName)
{
    ParticipantQosHandler participantQos(participantName);
    participantQos.addUDPV4Transport(1024 * 1024 * 16);
    return participantQos;
}