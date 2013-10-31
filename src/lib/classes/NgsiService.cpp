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
#include "NgsiService.h"



namespace cb
{

namespace ngsi
{



/* ****************************************************************************
*
* NgsiService::init - 
*/
void NgsiService::init(NgsiContainer container, bool isRequest, NgsiServiceType serviceType, bool mandatory, cb::common::Format format, std::string tag, std::string indent)
{
  NgsiBase::init(container, isRequest, serviceType, mandatory, format, tag, indent);
}


/* ****************************************************************************
*
* format - 
*/
void NgsiService::format(std::string format)
{
  if ((format == "JSON") || (format == "Json") || (format == "json"))
    _format = cb::common::JSON;
  else if ((format == "TEXT") || (format == "Text") || (format == "text"))
    _format = cb::common::TEXT;
  else if ((format == "cb::common::XML") || (format == "Xml") || (format == "xml"))
    _format = cb::common::XML;

  // Default format is XML
  _format = cb::common::XML;
}



/* ****************************************************************************
*
* format - 
*/
void NgsiService::format(cb::common::Format format)
{
  _format = format;
}



/* ****************************************************************************
*
* format - 
*/
cb::common::Format NgsiService::format(void)
{
  return _format;
}


/* ****************************************************************************
*
* inFormat - 
*/
void NgsiService::inFormat(std::string format)
{
  if ((format == "JSON") || (format == "Json") || (format == "json"))
    _inFormat = cb::common::JSON;
  else if ((format == "TEXT") || (format == "Text") || (format == "text"))
    _inFormat = cb::common::TEXT;
  else if ((format == "cb::common::XML") || (format == "Xml") || (format == "xml"))
    _inFormat = cb::common::XML;

  // Default format is XML
  _inFormat = cb::common::XML;
}



/* ****************************************************************************
*
* inFormat - 
*/
void NgsiService::inFormat(cb::common::Format format)
{
  _inFormat = format;
}



/* ****************************************************************************
*
* inFormat - 
*/
cb::common::Format NgsiService::inFormat(void)
{
  return _inFormat;
}


/* ****************************************************************************
*
* outFormat - 
*/
void NgsiService::outFormat(std::string format)
{
  if ((format == "JSON") || (format == "Json") || (format == "json"))
    _outFormat = cb::common::JSON;
  else if ((format == "TEXT") || (format == "Text") || (format == "text"))
    _outFormat = cb::common::TEXT;
  else if ((format == "XML") || (format == "Xml") || (format == "xml"))
    _outFormat = cb::common::XML;

  // Default format is XML
  _outFormat = cb::common::XML;
}



/* ****************************************************************************
*
* outFormat - 
*/
void NgsiService::outFormat(cb::common::Format format)
{
  _outFormat = format;
}



/* ****************************************************************************
*
* outFormat - 
*/
cb::common::Format NgsiService::outFormat(void)
{
  return _outFormat;
}

}
}
