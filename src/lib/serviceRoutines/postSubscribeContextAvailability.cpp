/*
*
* Copyright 2013 Telefonica Investigacion y Desarrollo, S.A.U
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
* Author: Ken Zangelin
*/
#include <string>
#include <vector>

#include "mongoBackend/mongoSubscribeContextAvailability.h"
#include "ngsi/ParseData.h"
#include "ngsi9/SubscribeContextAvailabilityResponse.h"
#include "rest/ConnectionInfo.h"
#include "serviceRoutines/postSubscribeContextAvailability.h"



/* ****************************************************************************
*
* postSubscribeContextAvailability - 
*/
std::string postSubscribeContextAvailability(ConnectionInfo* ciP, int components, std::vector<std::string>& compV, ParseData* parseDataP)
{
  SubscribeContextAvailabilityResponse  scar;
  std::string                           answer;

  // FIXME P6: at the moment, we assume that notifications are sent in the same format that the one
  // used to do the subscription, so we are passing ciP->inFormat. This is just an heuristic, the client could want
  // for example to use XML in the subscription message but wants notifications in JSON. We need a more
  // flexible approach, to be implemented
  ciP->httpStatusCode = mongoSubscribeContextAvailability(&parseDataP->scar.res, &scar, ciP->inFormat, ciP->tenant);
  answer = scar.render(SubscribeContextAvailability, ciP->outFormat, "");

  return answer;
}
