#include "ParticipantQosHandler.h"

#include <fastdds/rtps/transport/TCPv4TransportDescriptor.h>
#include <fastdds/rtps/transport/TCPv6TransportDescriptor.h>
#include <fastdds/rtps/transport/UDPv4TransportDescriptor.h>
#include <fastdds/rtps/transport/UDPv6TransportDescriptor.h>
#include <fastdds/rtps/transport/shared_mem/SharedMemTransportDescriptor.h>
#include <glog/logging.h>

using namespace eprosima::fastdds::dds;
using namespace eprosima::fastdds::rtps;
using namespace eprosima::fastrtps::rtps;

ParticipantQosHandler::ParticipantQosHandler(std::string participant_name)
{
    m_participantQos.wire_protocol().builtin.discovery_config.discoveryProtocol = DiscoveryProtocol_t::SIMPLE;
    m_participantQos.wire_protocol().builtin.discovery_config.use_SIMPLE_EndpointDiscoveryProtocol = true;
    m_participantQos.wire_protocol().builtin.discovery_config.m_simpleEDP.use_PublicationReaderANDSubscriptionWriter = true;
    m_participantQos.wire_protocol().builtin.discovery_config.m_simpleEDP.use_PublicationWriterANDSubscriptionReader = true;
    // RTPSParticipant 发现其他 RTPSParticipant 之后多久开始推送消息
    eprosima::fastrtps::Time_t duration = eprosima::fastrtps::Duration_t(3, 0);
    m_participantQos.wire_protocol().builtin.discovery_config.leaseDuration_announcementperiod = duration;
    // 提示远端认为此 RTPSParticipant 应该存活的时间
    m_participantQos.wire_protocol().builtin.discovery_config.leaseDuration = eprosima::fastrtps::c_TimeInfinite;
    m_participantQos.name(participant_name);
    // 设置为 false 可禁用默认的 UDPv4 传输方式, 默认为true
    m_participantQos.transport().use_builtin_transports = false;
}

ParticipantQosHandler::~ParticipantQosHandler()
{
}

DomainParticipantQos &ParticipantQosHandler::getQos()
{
    return m_participantQos;
}

void ParticipantQosHandler::addSHMTransport(uint32_t segment_size)
{
    auto shm_transport = std::make_shared<SharedMemTransportDescriptor>();
    shm_transport->segment_size(segment_size);
    m_participantQos.transport().user_transports.push_back(shm_transport);
}

void ParticipantQosHandler::addTCPV4Transport(uint16_t listen_port, const std::vector<std::string> &peer_locators)
{
    auto tcp_transport = std::make_shared<TCPv4TransportDescriptor>();
    if (listen_port != 0) {
        tcp_transport->add_listener_port(listen_port);
    }
    for (int i = 0; i < peer_locators.size(); i++) {
        int position = peer_locators[i].find(':');
        if (position != peer_locators[i].npos) {
            std::string ipv4 = peer_locators[i].substr(0, position);
            std::string port = peer_locators[i].substr(position + 1);

            Locator_t initial_peer_locator;
            initial_peer_locator.kind = LOCATOR_KIND_TCPv4;
            IPLocator::setIPv4(initial_peer_locator, ipv4);
            initial_peer_locator.port = std::atoi(port.c_str());
            m_participantQos.wire_protocol().builtin.initialPeersList.push_back(initial_peer_locator);
        }
    }
    m_participantQos.transport().user_transports.push_back(tcp_transport);
}

void ParticipantQosHandler::addTCPV6Transport()
{
}

void ParticipantQosHandler::addUDPV4Transport(uint32_t buffer_size, const std::vector<std::string> &ipaddrs)
{
    auto udp_transport = std::make_shared<UDPv4TransportDescriptor>();

    udp_transport->sendBufferSize = buffer_size;
    udp_transport->receiveBufferSize = buffer_size;
    // 是否使用对 send_to() 的非阻塞调用
    udp_transport->non_blocking_send = true;
    for (int i = 0; i < ipaddrs.size(); i++) {
        Locator_t initial_peer_locator;
        IPLocator::setIPv4(initial_peer_locator, ipaddrs[i]);
        m_participantQos.wire_protocol().builtin.initialPeersList.push_back(initial_peer_locator);
    }
    m_participantQos.transport().user_transports.push_back(udp_transport);
}

void ParticipantQosHandler::addUDPV6Transport(uint32_t buffer_size)
{
    auto udp_transport = std::make_shared<UDPv6TransportDescriptor>();
    udp_transport->sendBufferSize = buffer_size;
    udp_transport->receiveBufferSize = buffer_size;
    udp_transport->non_blocking_send = true;
    m_participantQos.transport().user_transports.push_back(udp_transport);
}