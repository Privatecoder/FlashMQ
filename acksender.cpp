/*
This file is part of FlashMQ (https://www.flashmq.org)
Copyright (C) 2021-2023 Wiebe Cazemier

FlashMQ is free software: you can redistribute it and/or modify
it under the terms of The Open Software License 3.0 (OSL-3.0).

See LICENSE for license details.
*/

#include "acksender.h"

#include "mqttpacket.h"
#include "client.h"

AckSender::AckSender(uint8_t qos, uint16_t packetId, ProtocolVersion protocolVersion, std::shared_ptr<Client> &client) :
    qos(qos),
    packetId(packetId),
    protocolVersion(protocolVersion),
    client(client)
{

}

AckSender::~AckSender()
{
    if (!sent)
        sendNow();
}

void AckSender::sendNow()
{
    this->sent = true;

    if (qos == 0)
        return;

    const PacketType responseType = qos == 1 ? PacketType::PUBACK : PacketType::PUBREC;
    PubResponse pubAck(this->protocolVersion, responseType, ackCode, packetId);
    MqttPacket response(pubAck);
    client->writeMqttPacket(response);
}

void AckSender::setAckCode(ReasonCodes ackCode)
{
    this->ackCode = ackCode;
}
