#pragma once

#include "DDSParticipantManager.h"
#include <functional>
#include <unordered_map>

class DDSTestHandler : public DDSParticipantManager
{
public:
    DDSTestHandler(int domainId);
    ~DDSTestHandler();

protected:
    ParticipantQosHandler createParticipantQos(std::string participantName) override;
};