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

#include "common/Format.h"
#include "common/globals.h"
#include "common/tag.h"
#include "ngsi/Request.h"
#include "ngsi/StatusCode.h"
#include "ngsi9/UpdateContextAvailabilitySubscriptionRequest.h"
#include "ngsi9/UpdateContextAvailabilitySubscriptionResponse.h"



/* ****************************************************************************
*
* Constructor - 
*/
UpdateContextAvailabilitySubscriptionRequest::UpdateContextAvailabilitySubscriptionRequest()
{
   restrictions = 0;
}



/* ****************************************************************************
*
* UpdateContextAvailabilitySubscriptionRequest::render - 
*/
std::string UpdateContextAvailabilitySubscriptionRequest::render(RequestType requestType, Format format, std::string indent)
{
  std::string out = "";
  std::string tag = "updateContextAvailabilitySubscriptionRequest";

  out += startTag(indent, tag, format);
  out += entityIdVector.render(format, indent + "  ");
  out += attributeList.render(format, indent + "  ");
  out += duration.render(format, indent + "  ");
  out += restriction.render(format, indent + "  ");
  out += subscriptionId.render(format, indent + "  ");
  out += endTag(indent, tag, format);

  return out;
}



/* ****************************************************************************
*
* UpdateContextAvailabilitySubscriptionRequest::present - 
*/
void UpdateContextAvailabilitySubscriptionRequest::present(std::string indent)
{
   entityIdVector.present(indent + "  ");
   attributeList.present(indent + "  ");
   duration.present(indent + "  ");
   restriction.present(indent + "  ");
   subscriptionId.present(indent + "  ");
}



/* ****************************************************************************
*
* UpdateContextAvailabilitySubscriptionRequest::check - 
*/
std::string UpdateContextAvailabilitySubscriptionRequest::check(RequestType requestType, Format format, std::string indent, std::string predetectedError, int counter)
{
  std::string                                    res;
  UpdateContextAvailabilitySubscriptionResponse  response;

  response.subscriptionId = subscriptionId;

  if (predetectedError != "")
  {
    response.errorCode.code         = SccBadRequest;
    response.errorCode.reasonPhrase = predetectedError;
  }
  else if (((res = entityIdVector.check(UpdateContextAvailabilitySubscription, format, indent, predetectedError, counter))      != "OK") ||
           ((res = attributeList.check(UpdateContextAvailabilitySubscription, format, indent, predetectedError, counter))       != "OK") ||
           ((res = duration.check(UpdateContextAvailabilitySubscription, format, indent, predetectedError, counter))            != "OK") ||
           ((res = restriction.check(UpdateContextAvailabilitySubscription, format, indent, predetectedError, counter))         != "OK") ||
           ((res = subscriptionId.check(UpdateContextAvailabilitySubscription, format, indent, predetectedError, counter))      != "OK"))
  {
    response.errorCode.code         = SccBadRequest;
    response.errorCode.reasonPhrase = res;
  }
  else
    return "OK";

  return response.render(UpdateContextAvailabilitySubscription, format, indent, counter);
}



/* ****************************************************************************
*
* UpdateContextAvailabilitySubscriptionRequest::release - 
*/
void UpdateContextAvailabilitySubscriptionRequest::release(void)
{
  entityIdVector.release();
  attributeList.release();
  duration.release();
  restriction.release();
  subscriptionId.release();
}
