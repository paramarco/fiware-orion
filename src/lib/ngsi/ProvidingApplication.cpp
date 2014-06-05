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

#include "logMsg/logMsg.h"
#include "logMsg/traceLevels.h"

#include "common/globals.h"
#include "common/tag.h"
#include "ngsi/Request.h"
#include "ngsi/ProvidingApplication.h"



/* ****************************************************************************
*
* ProvidingApplication::check - 
*/
std::string ProvidingApplication::check(RequestType requestType, Format format, const std::string& indent, const std::string& predetectedError, int counter)
{
  if (isEmpty())
     return "no providing application";

  return "OK";
}



/* ****************************************************************************
*
* ProvidingApplication::isEmpty - 
*/
bool ProvidingApplication::isEmpty(void)
{
   return (string == "")? true : false;
}



/* ****************************************************************************
*
* ProvidingApplication::set - 
*/
void ProvidingApplication::set(const std::string& value)
{
  string = value;
}



/* ****************************************************************************
*
* ProvidingApplication::get - 
*/
std::string ProvidingApplication::get(void)
{
  return string;
}



/* ****************************************************************************
*
* ProvidingApplication::present - 
*/
void ProvidingApplication::present(const std::string& indent)
{
  if (string != "")
    PRINTF("%sProvidingApplication: %s\n", indent.c_str(), string.c_str());
  else
    PRINTF("%sNo ProvidingApplication\n", indent.c_str());
}



/* ****************************************************************************
*
* ProvidingApplication::render - 
*/
std::string ProvidingApplication::render(Format format, const std::string& indent, bool comma)
{
  if (string == "")
    return "";

  return valueTag(indent, "providingApplication", string, format, comma);
}



/* ****************************************************************************
*
* ProvidingApplication::c_str - 
*/
const char* ProvidingApplication::c_str(void)
{
   return string.c_str();
}



/* ****************************************************************************
*
* release - 
*/
void ProvidingApplication::release(void)
{
   /* This method is included for the sake of homogeneity */
}
