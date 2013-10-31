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

#include "logMsg/logMsg.h"

#include "globals.h"
#include "Format.h"
#include "HttpStatusCode.h"
#include "NgsiServiceType.h"
#include "restErrorMsg.h"

namespace cb
{

namespace rest
{



/* ****************************************************************************
*
* errorMsgXml - 
*/
static std::string errorMsgXml(std::string tag, HttpStatusCode code, std::string reasonPhrase, std::string details)
{
  std::string out = "";

  out += "<" + tag + ">\n";
  out += TABULATOR + "<code>"         + httpStatusCodeToString(code) + "</code>\n";
  out += TABULATOR + "<reasonPhrase>" +  reasonPhrase                + "</reasonPhrase>\n";
  out += TABULATOR + "<details>"      +  details                     + "</details>\n";
  out += "</" + tag + ">\n";

  return out;
}



/* ****************************************************************************
*
* errorMsgJson - 
*/
static std::string errorMsgJson(std::string tag, HttpStatusCode code, std::string reasonPhrase, std::string details)
{
  std::string out = "";

  out += "\"" + tag + "\":\n";
  out += "{\n";
  out += TABULATOR + "\"code\" : \""         + httpStatusCodeToString(code) + "\",\n";
  out += TABULATOR + "\"reasonPhrase\" : \"" + reasonPhrase + "\",\n";
  out += TABULATOR + "\"details\" : \""      + details + "\",\n";
  out += "}\n";

  return out;
}



/* ****************************************************************************
*
* errorMsgText - 
*/
static std::string errorMsgText(std::string tag, HttpStatusCode code, std::string reasonPhrase, std::string details)
{
   return "plain text error";
}



/* ****************************************************************************
*
* errorMsgHtml - 
*/
static std::string errorMsgHtml(std::string tag, HttpStatusCode code, std::string reasonPhrase, std::string details)
{
   return "HTML error";
}



/* ****************************************************************************
*
* errorMsg - 
*/
std::string errorMsg(cb::ngsi::NgsiServiceType serviceType, cb::common::Format format, HttpStatusCode code, std::string reasonPhrase, std::string details)
{
  std::string tag = ngsiServiceTypeToTag(serviceType);

  if (format == cb::common::XML)
    return errorMsgXml(tag, code, reasonPhrase, details);
  else if (format ==cb::common::JSON)
    return errorMsgJson(tag, code, reasonPhrase, details);
  else if (format ==cb::common::TEXT)
    return errorMsgText(tag, code, reasonPhrase, details);
  else if (format ==cb::common::HTML)
    return errorMsgHtml(tag, code, reasonPhrase, details);

  return "cb::rest::errorMsg: bad format";
}

}
}
