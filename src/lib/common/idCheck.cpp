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
#include "logMsg/traceLevels.h"

#include "common/idCheck.h"



/* ****************************************************************************
*
* idCheck - 
*/
std::string idCheck(const std::string& s)
{
  if (s.length() != 24)
  {
    return "bad length (24 chars expected)";
  }

  char* cP = (char*) s.c_str();

  while (*cP != 0)
  {
    if ((*cP >= '0') && (*cP <= '9'))
    {
    }
    else if ((*cP >= 'a') && (*cP <= 'f'))
    {
    }
    else if ((*cP >= 'A') && (*cP <= 'F'))
    {
    }
    else
    {
      LM_W(("Bad Input (invalid char '%c' in ID string)", *cP));
      return "invalid char in ID string";
    }

    ++cP;
  }

  return "OK";
}
