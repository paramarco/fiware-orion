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

#include "Format.h"
#include "Present.h"
#include "NgsiContainer.h"
#include "NgsiServiceType.h"
#include "NgsiContextRegistration.h"

namespace cb
{

namespace ngsi
{

/* ****************************************************************************
*
* init - 
*/
void NgsiContextRegistration::init(NgsiContainer container, bool isRequest, NgsiServiceType serviceType, bool mandatory, cb::common::Format format, std::string tag, std::string indent)
{
  NgsiBase::init(container, isRequest, serviceType, mandatory, format, tag, indent);

  _entityIdVector.init(container,                     isRequest, serviceType, false, format, "entityIdList",                     "entityId",                     indent + TABULATOR);
  _contextRegistrationAttributeVector.init(container, isRequest, serviceType, false, format, "contextRegistrationAttributeList", "contextRegistrationAttribute", indent + TABULATOR);
  _registrationMetadataVector.init(container,         isRequest, serviceType, false, format, "registrationMetadata",             "metadata",                     indent + TABULATOR);
  _providingApplication.init(container,               isRequest, serviceType, true,  format, "providingApplication",             indent + TABULATOR);
}



/* ****************************************************************************
*
* render - 
*/
std::string NgsiContextRegistration::render(void)
{
  std::string out = "";

  out += tagStart();
  out += _entityIdVector.render();
  out += _contextRegistrationAttributeVector.render();
  out += _registrationMetadataVector.render();
  out += _providingApplication.render();
  out += tagEnd();

  return out;
}



/* ****************************************************************************
*
* check - 
*/
std::string NgsiContextRegistration::check(void)
{
  std::string s;

  LM_T(LmtCheck, ("IN"));

  if       ((s = _entityIdVector.check())                     != "OK")  return s;
  else if  ((s = _contextRegistrationAttributeVector.check()) != "OK")  return s;
  else if  ((s = _registrationMetadataVector.check())         != "OK")  return s;
  else if  ((s = _providingApplication.check())               != "OK")  return s;
  else
    return "OK";
}



/* ****************************************************************************
*
* present - 
*/
void NgsiContextRegistration::present(void)
{
  _entityIdVector.present();
  _contextRegistrationAttributeVector.present();
  _registrationMetadataVector.present();
  _providingApplication.present();
}



/* ****************************************************************************
*
* release - 
*/
void NgsiContextRegistration::release(void)
{
  _entityIdVector.release();
  _contextRegistrationAttributeVector.release();
  _registrationMetadataVector.release();
  _providingApplication.release();
}

}
}
