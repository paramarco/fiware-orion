#ifndef NGSI_STRING_H
#define NGSI_STRING_H

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

#include "NgsiBase.h"

namespace cb
{

namespace ngsi
{


/* ****************************************************************************
*
* NgsiString - 
*/
class NgsiString: public NgsiBase
{
protected:
  std::string    _value;

public:
  // overridden virtual methods
  std::string    render(void);
  std::string    check(void);
  void           present(void);
  void           release(void);

  // new methods
  virtual void   value(std::string value);
  std::string    value(void);
  bool           isEmpty(void);
  int            length(void);

  bool operator==(std::string s) { return (s == _value); }
};

}
}

#endif
