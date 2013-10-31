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

#include "HttpStatusCode.h"



/* ****************************************************************************
*
* httpStatusCodeToString - 
*/
std::string httpStatusCodeToString(HttpStatusCode code)
{
  switch (code)
  {
  case SccNone:                   return  "0";
  case SccOk:                     return  "200";
  case SccBadRequest:             return  "400";
  case SccForbidden:              return  "403";
  case SccContextElementNotFound: return  "404";
  case SccBadVerb:                return  "405";
  case SccNotAcceptable:          return  "406";
  case SccRequestEntityTooLarge:  return  "413";
  case SccUnsupportedMediaType:   return  "415";
  case SccSubscriptionIdNotFound: return  "470";
  case SccMissingParameter:       return  "471";
  case SccInvalidParameter:       return  "472";
  case SccErrorInMetadata:        return  "473";
  case SccEntityIdReNotAllowed:   return  "480";
  case SccEntityTypeRequired:     return  "481";
  case SccAttributeListRequired:  return  "482";
  case SccReceiverInternalError:  return  "500";
  case SccNotImplemented:         return  "501";
  }

  return "200";
}
