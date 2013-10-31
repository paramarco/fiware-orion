#ifndef NGSI_REGISTER_CONTEXT_REQUEST_H
#define NGSI_REGISTER_CONTEXT_REQUEST_H

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

/* ****************************************************************************
*
* FILE                    NgsiRegisterContextRequest.h
*
* CREATION DATE           Jan 2013
*
* AUTHOR                  Ken Zangelin
*
*
*/
#include <string>

#include "Present.h"
#include "Format.h"
#include "NgsiRequest.h"
#include "NgsiVector.h"
#include "NgsiContextRegistration.h"
#include "NgsiTimeString.h"
#include "NgsiRegistrationId.h"
#include "NgsiServiceType.h"


namespace cb
{

namespace ngsi9
{

/* ****************************************************************************
*
* NgsiRegisterContextRequest - 
*/
   class NgsiRegisterContextRequest : public ngsi::NgsiRequest
{
public:
  ngsi::NgsiVector<ngsi::NgsiContextRegistration*>  _contextRegistrationVector;  // Mandatory
  ngsi::NgsiTimeString                              _duration;                   // Optional
  ngsi::NgsiRegistrationId                          _registrationId;             // Optional


  void         init(cb::ngsi::NgsiContainer container, bool isRequest, cb::ngsi::NgsiServiceType serviceType, bool mandatory, cb::common::Format format, std::string tag, std::string indent);
  std::string  render(void);
  std::string  check(void);
  void         present(void);
};

}
}

#endif
