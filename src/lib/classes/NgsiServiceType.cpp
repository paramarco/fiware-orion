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

#include "NgsiServiceType.h"

namespace cb
{

namespace ngsi
{



/* ****************************************************************************
*
* ngsiServiceTypeToTag - 
*/
std::string ngsiServiceTypeToTag(NgsiServiceType serviceType)
{
  switch (serviceType)
  {
  case NgsiNoService:                                return "No Service";

  case Ngsi9DiscoverContextAvailability:             return "DiscoverContextAvailability";
  case Ngsi9NotifyContextAvailability:               return "NotifyContextAvailability";
  case Ngsi9RegisterContext:                         return "RegisterContext";
  case Ngsi9SubscribeContextAvailability:            return "SubscribeContextAvailability";
  case Ngsi9UnsubscribeContextAvailability:          return "UnsubscribeContextAvailability";
  case Ngsi9UpdateContextAvailabilitySubscription:   return "UpdateContextAvailabilitySubscription";

  case Ngsi10NotifyContext:                          return "NotifyContext";
  case Ngsi10QueryContext:                           return "QueryContext";
  case Ngsi10SubscribeContext:                       return "SubscribeContext";
  case Ngsi10UnsubscribeContext:                     return "UnsubscribeContext";
  case Ngsi10UpdateContext:                          return "UpdateContext";
  case Ngsi10UpdateContextSubscription:              return "UpdateContextSubscription";
  }

  return "Bad Service Type";
}

}
}
