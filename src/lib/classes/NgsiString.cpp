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

#include <stdio.h>
#include <string>

#include "logMsg/logMsg.h"
#include "contextBrokerII/traceLevels.h"

#include "NgsiString.h"
#include "NgsiService.h"

#include "xmlSimpleString.h"
#include "jsonSimpleString.h"
#include "textSimpleString.h"



namespace cb
{

namespace ngsi
{

/* ****************************************************************************
*
* render - 
*/
std::string NgsiString::render(void)
{
   if (format() == cb::common::XML)
    return xmlSimpleString(_indent, _tag, _value);
  else if (format() == cb::common::JSON)
    return jsonSimpleString(_indent, _tag, _value);

  return textSimpleString(_indent, _tag, _value);
}



/* ****************************************************************************
*
* check - 
*/
std::string NgsiString::check(void)
{
  LM_T(LmtCheck, ("IN"));

  if ((_mandatory == true) && (_used == false))
    return "Mandatory but not supplied";

  if (_used == false)
    return "OK";

  if ((_mandatory == true) && (_value == ""))
    return "empty string";

  return "OK";
}



/* ****************************************************************************
*
* present - 
*/
void NgsiString::present(void)
{
   printf("%s%s: '%s'\n", _indent.c_str(), _tag.c_str(), _value.c_str());
}



/* ****************************************************************************
*
* release - 
*/
void NgsiString::release(void)
{
}



/* ****************************************************************************
*
* value - 
*/
void NgsiString::value(std::string value)
{
  _value = value;
}



/* ****************************************************************************
*
* value - 
*/
std::string NgsiString::value(void)
{
  return _value;
}



/* ****************************************************************************
*
* isEmpty - 
*/
bool NgsiString::isEmpty(void)
{
  return (_value == "")? true : false;
}



/* ****************************************************************************
*
* length - 
*/
int NgsiString::length(void)
{
  return _value.length();
}

}
}
