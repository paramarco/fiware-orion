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
#include "contextBrokerII/traceLevels.h"

#include "NgsiContainer.h"
#include "Format.h"
#include "Present.h"
#include "NgsiRegisterContextRequest.h"



namespace cb
{

namespace ngsi9
{

/* ****************************************************************************
*
* NgsiRegisterContextRequest::init- 
*/
   void NgsiRegisterContextRequest::init(cb::ngsi::NgsiContainer container, bool isRequest, cb::ngsi::NgsiServiceType serviceType, bool mandatory, cb::common::Format format, std::string tag, std::string indent)
{
  NgsiBase::init(container,                  isRequest, serviceType, mandatory, format, "registerContextRequest",                         indent);

  _contextRegistrationVector.init(container, isRequest, serviceType, true,      format, "contextRegistrationList", "contextRegistration", indent + TABULATOR);
  _duration.init(container,                  isRequest, serviceType, false,     format, "duration",                                       indent + TABULATOR);
  _registrationId.init(container,            isRequest, serviceType, false,     format, "registrationId",                                 indent + TABULATOR);
}



/* ****************************************************************************
*
* NgsiRegisterContextRequest::present - 
*/
void NgsiRegisterContextRequest::present(void)
{
  PRESENT_START();
  _contextRegistrationVector.present();
  _duration.present();
  _registrationId.present();
  PRESENT_END();
}



/* ****************************************************************************
*
* NgsiRegisterContextRequest::render - 
*/
std::string NgsiRegisterContextRequest::render(void)
{
  std::string out = "";

  out += tagStart();
  out += _contextRegistrationVector.render();
  out += _duration.render();
  out += _registrationId.render();
  out += tagEnd();

  return out;
}



/* ****************************************************************************
*
* NgsiRegisterContextRequest::check - 
*/
std::string NgsiRegisterContextRequest::check(void)
{
  std::string s;

  LM_T(LmtCheck, ("IN"));

  if       ((s = _contextRegistrationVector.check()) != "OK")  { LM_W(("1: s == '%s'", s.c_str())); return s; }
  else if  ((s = _duration.check())                  != "OK")  { LM_W(("2")); return s; }
  else if  ((s = _registrationId.check())            != "OK")  { LM_W(("3")); return s; }
  else
  {
    LM_W(("OK"));
    return "OK";
  }
}

}
}
