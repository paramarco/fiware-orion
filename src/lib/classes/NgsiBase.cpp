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

#include "Format.h"
#include "NgsiBase.h"



namespace cb
{

namespace ngsi
{

/* ****************************************************************************
*
* Constructor - 
*/
NgsiBase::NgsiBase()
{
  _container    = NgsiNoContainer;
  _isRequest    = false;
  _serviceType  = NgsiNoService;
  _mandatory    = true;
  _used         = false;
  _tag          = "TAG";
  _indent       = "";
}



/* ****************************************************************************
*
* container - 
*/
NgsiContainer NgsiBase::container(void)
{
   return _container;
}



/* ****************************************************************************
*
* isRequest - 
*/
bool NgsiBase::isRequest(void)
{
   return _isRequest;
}



/* ****************************************************************************
*
* serviceType - 
*/
NgsiServiceType NgsiBase::serviceType(void)
{
   return _serviceType;
}



/* ****************************************************************************
*
* mandatory - 
*/
bool NgsiBase::mandatory(void)
{
  return _mandatory;
}



/* ****************************************************************************
*
* used - 
*/
bool NgsiBase::used(void)
{
  return _used;
}



/* ****************************************************************************
*
* tag - 
*/
std::string NgsiBase::tag(void)
{
  return _tag;
}



/* ****************************************************************************
*
* indent - 
*/
std::string NgsiBase::indent(void)
{
  return _indent;
}



/* ****************************************************************************
*
* format - 
*/
cb::common::Format NgsiBase::format(void)
{
  return _format;
}



/* ****************************************************************************
*
* container - 
*/
void NgsiBase::container(NgsiContainer container)
{
  _container = container;
}



/* ****************************************************************************
*
* isRequest - 
*/
void NgsiBase::isRequest(bool isRequest)
{
  _isRequest = isRequest;
}



/* ****************************************************************************
*
* serviceType - 
*/
void NgsiBase::serviceType(NgsiServiceType serviceType)
{
  _serviceType = serviceType;
}



/* ****************************************************************************
*
* mandatory - 
*/
void NgsiBase::mandatory(bool mandatory)
{
  _mandatory = mandatory;
}



/* ****************************************************************************
*
* used - 
*/
void NgsiBase::used(bool used)
{
  _used = used;
}



/* ****************************************************************************
*
* tag - 
*/
void NgsiBase::tag(std::string tag)
{
  _tag = tag;
}



/* ****************************************************************************
*
* indent - 
*/
void NgsiBase::indent(std::string indent)
{
   _indent = indent;
}



/* ****************************************************************************
*
* format - 
*/
void NgsiBase::format(cb::common::Format format)
{
   _format = format;
}



/* ****************************************************************************
*
* init - 
*/
void NgsiBase::init(NgsiContainer containerX, bool isRequestX, NgsiServiceType serviceTypeX, bool mandatoryX, cb::common::Format formatX, std::string tagX, std::string indentX)
{
  container(containerX);
  isRequest(isRequestX);
  serviceType(serviceTypeX);
  mandatory(mandatoryX);
  format(formatX);
  tag(tagX);
  indent(indentX);
}



/* ****************************************************************************
*
* tagStart - 
*/
std::string NgsiBase::tagStart(void)
{
  if (_format == cb::common::XML)
    return _indent + "<" + _tag + ">" + "\n";
  else if (_format == cb::common::JSON)
    return _indent + "\"" + _tag + "\" : {\n";
  else if (_format == cb::common::TEXT)
     return _indent + _tag + "\n";

  return _indent;
}



/* ****************************************************************************
*
* tagEnd - 
*/
std::string NgsiBase::tagEnd(void)
{
  if (_format == cb::common::XML)
    return _indent + "</" + _tag + ">" + "\n";
  else if (_format == cb::common::JSON)
    return _indent + "}\n";
  else if (_format == cb::common::TEXT)
    return "\n";
   
  return _indent + "TAG_END";
}

}
}
