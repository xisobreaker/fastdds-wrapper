#include "DDSParticipantManager.h"
#include "DDSDomainParticipant.h"
#include "ParticipantQosHandler.h"
#include <memory>

DDSParticipantManager::DDSParticipantManager(int domainId) : m_domainId(domainId)
{
}

DDSParticipantManager::~DDSParticipantManager()
{
}

bool DDSParticipantManager::initDomainParticipant(std::string participantName)
{
    if (!m_participant) {
        eprosima::fastdds::dds::DomainParticipantQos participantQos = createParticipantQos(participantName).getQos();
        m_participant = std::make_shared<DDSDomainParticipant>(m_domainId, participantQos);
    }
    return true;
}

eprosima::fastdds::dds::TopicDataType *DDSParticipantManager::getTopicDataType(std::string topicName)
{
    if (m_topicTypes.find(topicName) == m_topicTypes.end()) {
        LOG(ERROR) << "Cann't found topic: " << topicName;
        return nullptr;
    }
    return m_topicTypes.at(topicName)();
}

void DDSParticipantManager::addTopicDataTypeCreator(std::string topicName, TopicDataTypeCreator creator)
{
    if (m_topicTypes.find(topicName) == m_topicTypes.end())
        m_topicTypes.insert(std::make_pair(topicName, creator));
}