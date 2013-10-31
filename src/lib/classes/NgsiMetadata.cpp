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
#include "contextBrokerII/traceLevels.h"

#include "globals.h"
#include "Present.h"
#include "NgsiContainer.h"
#include "NgsiServiceType.h"
#include "NgsiMetadata.h"
#include "restErrorMsg.h"



namespace cb
{

namespace ngsi
{

/* ****************************************************************************
*
* NgsiMetadata::NgsiMetadata - 
*/
NgsiMetadata::NgsiMetadata(std::string name, std::string type, std::string value)
{
  _name.value(name);
  _type.value(type);
  _value.value(value);
}



/* ****************************************************************************
*
* init - 
*/
void NgsiMetadata::init(NgsiContainer container, bool isRequest, NgsiServiceType serviceType, bool mandatory, cb::common::Format format, std::string tag, std::string indent)
{
  NgsiBase::init(container, isRequest, serviceType, mandatory, format, tag, indent);

  _name.init(container,   isRequest, serviceType, true, format, "name",  indent + TABULATOR);
  _type.init(container,   isRequest, serviceType, true, format, "type",  indent + TABULATOR);
  _value.init(container,  isRequest, serviceType, true, format, "value", indent + TABULATOR);
}



/* ****************************************************************************
*
* render - 
*/
std::string NgsiMetadata::render(void)
{
  std::string out = "";

  out += tagStart();
  out += _name.render();
  out += _type.render();
  out += _value.render();
  out += tagEnd();

  return out;
}



/* ****************************************************************************
*
* check - 
*/
std::string NgsiMetadata::check(void)
{
  LM_T(LmtCheck, ("IN"));

  if (_name == "")
     return rest::errorMsg(_serviceType, _format, SccOk, "empty name", "no name for metadata");
  if (_value == "")
     return rest::errorMsg(_serviceType, _format, SccOk, "empty value", "no value for metadata");

  return "OK";
}



/* ****************************************************************************
*
* check - 
*/
void NgsiMetadata::present(void)
{
  _name.present();
  _type.present();
  _value.present();
}



/* ****************************************************************************
*
* release - 
*/
void NgsiMetadata::release(void)
{
}

}
}
