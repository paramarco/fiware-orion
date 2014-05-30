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
#include <vector>

#include "logMsg/logMsg.h"
#include "logMsg/traceLevels.h"

#include "common/string.h"
#include "common/globals.h"
#include "common/tag.h"
#include "common/compileInfo.h"

#include "ngsi/ParseData.h"
#include "rest/ConnectionInfo.h"
#include "serviceRoutines/versionTreat.h"



/* ****************************************************************************
*
* version - 
*/
static char versionString[30] = { 'a', 'l', 'p', 'h', 'a', 0 };



/* ****************************************************************************
*
* versionSet - 
*/
void versionSet(const char* version)
{
   strncpy(versionString, version, sizeof(versionString));
}

/* ****************************************************************************
*
* versionGet -
*/
char* versionGet() {
    return versionString;
}


/* ****************************************************************************
*
* versionTreat - 
*/
std::string versionTreat(ConnectionInfo* ciP, int components, std::vector<std::string>& compV, ParseData* parseDataP)
{
  std::string out     = "";
  std::string tag     = "orion";
  std::string indent  = "";

#ifdef UNIT_TEST
  std::string uptime = "0 d, 0 h, 0 m, 0 s";
#else
  std::string uptime = parsedUptime(getTimer()->getCurrentTime() - startTime);
#endif

  out += startTag(indent, tag, ciP->outFormat);
  out += valueTag(indent + "  ", "version",       versionString,   ciP->outFormat, true);
  out += valueTag(indent + "  ", "uptime",        uptime,          ciP->outFormat, true);
  out += valueTag(indent + "  ", "git_hash",      GIT_HASH,        ciP->outFormat, true);
  out += valueTag(indent + "  ", "compile_time",  COMPILE_TIME,    ciP->outFormat, true);
  out += valueTag(indent + "  ", "compiled_by",   COMPILED_BY,     ciP->outFormat, true);
  out += valueTag(indent + "  ", "compiled_in",   COMPILED_IN,     ciP->outFormat, false);
  out += endTag(indent, tag, ciP->outFormat);

  ciP->httpStatusCode = SccOk;
  return out;
}
