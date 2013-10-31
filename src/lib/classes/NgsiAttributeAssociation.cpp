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

#include "globals.h"
#include "NgsiAttributeAssociation.h"
#include "NgsiService.h"


namespace cb
{

namespace ngsi
{

/* ****************************************************************************
*
* NgsiAttributeAssociation - 
*/
void NgsiAttributeAssociation::init(NgsiContainer container, bool isRequest, NgsiServiceType serviceType, bool mandatory, cb::common::Format format, std::string tag, std::string indent)
{
  _format      = format;
  _container   = container;
  _isRequest   = isRequest;
  _serviceType = serviceType;
  _mandatory   = mandatory;
  _tag         = tag;
  _indent      = indent;


  _source.init(container, isRequest, serviceType, true, format, "source", indent + TABULATOR);
  _target.init(container, isRequest, serviceType, true, format, "target", indent + TABULATOR);
}



/* ****************************************************************************
*
* render - 
*/
std::string NgsiAttributeAssociation::render(void)
{
  std::string out = "";

  out += tagStart();
  out += _source.render();
  out += _target.render();
  out += tagEnd();

  return out;
}



/* ****************************************************************************
*
* check - 
*/
std::string NgsiAttributeAssociation::check(void)
{
  return "OK";
}



/* ****************************************************************************
*
* present - 
*/
void NgsiAttributeAssociation::present(void)
{
   PRESENT_START();
  _source.present();
  _target.present();
  PRESENT_END()
}



/* ****************************************************************************
*
* release - 
*/
void NgsiAttributeAssociation::release(void)
{
  _source.release();
  _target.release();
}

}
}
