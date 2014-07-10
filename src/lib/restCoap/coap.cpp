/*
*
* Copyright 2014 Telefonica Investigacion y Desarrollo, S.A.U
*
* This file is part of Orion Context Broker.
*
* Orion Context Broker is free software: you can redistribute it and/or
* modify it under the terms of the GNU Affero General Public License as
* published by the Free Software Foundation, either version 3 of the
* License, or (at your option) any later version.
*
* Orion Context Broker is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero
* General Public License for more details.
*
* You should have received a copy of the GNU Affero General Public License
* along with Orion Context Broker. If not, see http://www.gnu.org/licenses/.
*
* For those usages not covered by this license please contact with
* fermin at tid dot es
*
* Author: TID Developer
*/

#include "coap.h"

#include "logMsg/logMsg.h"
#include "logMsg/traceLevels.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#define __USE_POSIX 1
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <string>
#include <string.h>

#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "HttpProxy.h"
#include "HttpMessage.h"


/* ****************************************************************************
*
* callback -
*/
int Coap::callback(CoapPDU *request, int sockfd, struct sockaddr_storage *recvFrom)
{
  socklen_t addrLen = sizeof(struct sockaddr_in);
  if (recvFrom->ss_family == AF_INET6) {
    addrLen = sizeof(struct sockaddr_in6);
  }

  // Translate request from CoAP to HTTP and send it to MHD through loopback
  std::string httpResponse;
  httpResponse = sendHttpRequest(host, httpPort, request);
  if (httpResponse == "")
  {
    // Could not get an answer from HTTP module
    boost::scoped_ptr<CoapPDU> res(new CoapPDU());

    res->setVersion(1);
    res->setMessageID((request->getMessageID()));
    res->setCode(CoapPDU::COAP_INTERNAL_SERVER_ERROR);
    res->setType(CoapPDU::COAP_ACKNOWLEDGEMENT);
    res->setToken(request->getTokenPointer(), request->getTokenLength());

    ssize_t sent = sendto(sockfd, res->getPDUPointer(), res->getPDULength(), 0, (sockaddr*)recvFrom, addrLen);

    if (sent < 0)
    {
      LM_T(LmtCoap, ("Error sending packet: %ld.",sent));
      return 1;
    }
    else
    {
      LM_T(LmtCoap, ("Sent: %ld.",sent));
    }
    return 0;
  }

  boost::scoped_ptr<HttpMessage> hm(new HttpMessage(httpResponse));

  // CoAP message is too big, must send error to requester
  if (hm->contentLength() > COAP_BUFFER_SIZE)
  {
    boost::scoped_ptr<CoapPDU> res;

    res->setVersion(1);
    res->setMessageID((request->getMessageID()));
    res->setCode(CoapPDU::COAP_REQUEST_ENTITY_TOO_LARGE);
    res->setType(CoapPDU::COAP_ACKNOWLEDGEMENT);
    res->setToken(request->getTokenPointer(), request->getTokenLength());

    ssize_t sent = sendto(sockfd, res->getPDUPointer(), res->getPDULength(), 0, (sockaddr*)recvFrom, addrLen);

    if (sent < 0)
    {
      //LM_V(("Error sending packet: %ld.",sent));
      perror(NULL);
      return 1;
    }
    else
    {
      //LM_V(("Sent: %ld",sent));
    }

    return 1;
  }

  // Translate response from HTTP to CoAP
  boost::scoped_ptr<CoapPDU> coapResponse(hm->toCoap());

  if (!coapResponse)
  {
    // Could not translate HTTP into CoAP
    return 1;
  }

  // Prepare appropriate response in CoAP
  coapResponse->setVersion(1);
  coapResponse->setMessageID(request->getMessageID());
  coapResponse->setToken(request->getTokenPointer(), request->getTokenLength());

  // Set type
  switch (request->getType())
  {
  case CoapPDU::COAP_CONFIRMABLE:
  case CoapPDU::COAP_NON_CONFIRMABLE:
    coapResponse->setType(CoapPDU::COAP_ACKNOWLEDGEMENT);
    break;
  case CoapPDU::COAP_ACKNOWLEDGEMENT:
  case CoapPDU::COAP_RESET:
    break;
  default:
    return 1;
    break;
  };

  // Send the packet
  ssize_t sent = sendto(sockfd, coapResponse->getPDUPointer(), coapResponse->getPDULength(), 0, (sockaddr*)recvFrom, addrLen);
  if (sent < 0)
  {
    //LM_V(("Error sending packet: %ld.",sent));
    perror(NULL);
    return 1;
  }
  else
  {
    //LM_V(("Sent: %ld",sent));
  }

  return 0;
}



/* ****************************************************************************
*
* serve -
*/
void Coap::serve()
{
  // Buffers for UDP and URIs
  char buffer[COAP_BUFFER_SIZE];
  char uriBuffer[COAP_URI_BUFFER_SIZE];
  int  recvURILen = 0;
  int  ret = 0;

  // Storage for handling receive address
  struct sockaddr_storage recvAddr;
  socklen_t               recvAddrLen = sizeof(struct sockaddr_storage);

  // Prepare binding address
  struct addrinfo *bindAddr = NULL;
  struct addrinfo hints;

  // Setting up bind address
  memset(&hints, 0x00, sizeof(struct addrinfo));
  hints.ai_flags      = 0;
  hints.ai_socktype   = SOCK_DGRAM;
  hints.ai_flags     |= AI_NUMERICSERV;
  hints.ai_family     = PF_INET; // ipv4, PF_INET6 for ipv6 or PF_UNSPEC to let OS decide

  int error = getaddrinfo(host, coapPortStr, &hints, &bindAddr);
  if (error)
  {
    //LM_V(("Could not start CoAP server: Error getting address info: %s.", gai_strerror(error)));
    return;
  }

  // Setting up the UDP socket
  int sd = socket(bindAddr->ai_family, bindAddr->ai_socktype, bindAddr->ai_protocol);

  // Binding socket
  if (bind(sd, bindAddr->ai_addr, bindAddr->ai_addrlen) != 0)
  {
    //LM_V(("Could not start CoAP server: Error binding socket"));
    perror(NULL);
    return;
  }

  while (1)
  {
    // zero out the buffer
    memset(buffer, 0, COAP_BUFFER_SIZE);

    // receive packet
    ret = recvfrom(sd, &buffer, COAP_BUFFER_SIZE, 0, (sockaddr*)&recvAddr, &recvAddrLen);
    if (ret == -1)
    {
      //LM_V(("Error receiving data"));
      return;
    }

    boost::scoped_ptr<CoapPDU> recvPDU(new CoapPDU((uint8_t*)buffer, COAP_BUFFER_SIZE, COAP_BUFFER_SIZE));

    // validate packet
    if (ret > COAP_BUFFER_SIZE)
    {
      //LM_V(("PDU too large to fit in pre-allocated buffer"));
      continue;
    }
    recvPDU->setPDULength(ret);
    if (recvPDU->validate() != 1)
    {
      //LM_V(("Malformed CoAP packet"));
      continue;
    }
    //LM_V(("Valid CoAP PDU received"));

    // Treat URI
    if (recvPDU->getURI(uriBuffer, COAP_URI_BUFFER_SIZE, &recvURILen) != 0)
    {
      //LM_V(("Error retrieving URI"));
      continue;
    }

    if (recvURILen == 0)
    {
      //LM_V(("There is no URI associated with this Coap PDU"));
    }
    else
    {
      // Invoke a callback thread
      boost::thread *workerThread = new boost::thread(boost::bind(&Coap::callback, this, recvPDU.get(), sd, &recvAddr));

      // Wait for thread to finnish (like using no threads at all) for now
      workerThread->join();

      continue;
    }

    // no URI, handle cases

    // code == 0, no payload, this is a ping request, send RST
    if ((recvPDU->getPDULength() == 0) && (recvPDU->getCode() == 0))
    {
      //LM_V(("CoAP ping request"));
    }

  }
}



/* ****************************************************************************
*
* run -
*/
int Coap::run(const char *_host, unsigned short _httpPort, unsigned short _coapPort)
{
  char* portString = new char[6];
  snprintf(portString, 6, "%hd", _coapPort);
  this->coapPortStr = new char[strlen(portString)];
  strcpy(this->coapPortStr, portString);

  this->httpPort = _httpPort;
  this->coapPort = _coapPort;
  this->host = new char[strlen(_host)];
  strcpy(this->host, _host);

  boost::thread *coapServerThread = new boost::thread(boost::bind(&Coap::serve, this));

  coapServerThread->get_id(); // to prevent 'warning: unused'

  return 0;
}

