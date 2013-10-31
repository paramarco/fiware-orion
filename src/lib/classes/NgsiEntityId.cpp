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

#include "globals.h"
#include "Present.h"
#include "NgsiEntityId.h"
#include "restErrorMsg.h"



namespace cb
{

namespace ngsi
{

/* ****************************************************************************
*
* NgsiEntityId::NgsiEntityId - 
*/
NgsiEntityId::NgsiEntityId(std::string id, std::string type, std::string isPattern)
{
  _id.value(id);
  _type.value(type);
  _isPattern.value(isPattern);
}



/* ****************************************************************************
*
* init - 
*/
void NgsiEntityId::init(NgsiContainer container, bool isRequest, NgsiServiceType serviceType, bool mandatory, cb::common::Format format, std::string tag, std::string indent)
{
  NgsiBase::init(container,  isRequest, serviceType, mandatory, format, tag,         indent);

  _id.init(container,        isRequest, serviceType, true,      format, "id",        indent + TABULATOR);
  _type.init(container,      isRequest, serviceType, true,      format, "type",      indent + TABULATOR);
  _isPattern.init(container, isRequest, serviceType, false,     format, "isPattern", indent + TABULATOR);
}



/* ****************************************************************************
*
* render - 
*/
std::string NgsiEntityId::render(void)
{
  std::string out = tagStart();

  out += _id.render();
  out += _type.render();
  out += _isPattern.render(); 
  out += tagEnd();
  
  return out;
}



/* ****************************************************************************
*
* check - 
*/
std::string NgsiEntityId::check(void)
{
  if (_id == "")
  {
    LM_E(("No 'id' for EntityId"));
    return rest::errorMsg(_serviceType, _format, SccOk, "empty id", "no id string for entityId");
  }
  if (_type == "")
  {
    LM_E(("No 'type' for EntityId"));
    return rest::errorMsg(_serviceType, _format, SccOk, "empty type", "no type for entityId");
  }

  return "OK";
}



/* ****************************************************************************
*
* present - 
*/
void NgsiEntityId::present(void)
{
  PRESENT_START();
  _id.present();
  _type.present();
  _isPattern.present();
  PRESENT_END();
}



/* ****************************************************************************
*
* release - 
*/
void NgsiEntityId::release(void)
{
  _id.release();
  _type.release();
  _isPattern.release();
}

}
}
