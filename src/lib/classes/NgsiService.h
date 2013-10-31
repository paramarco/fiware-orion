#ifndef NGSI_SERVICE
#define NGSI_SERVICE

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

#include "Format.h"
#include "Present.h"
#include "NgsiBase.h"



namespace cb
{

namespace ngsi
{

/* ****************************************************************************
*
* NgsiService - 
*/
class NgsiService: public NgsiBase
{
public:
  cb::common::Format  _format;
  cb::common::Format  _inFormat;
  cb::common::Format  _outFormat;

  void                format(std::string format);
  void                format(cb::common::Format format);
  cb::common::Format  format(void);

  void                inFormat(std::string format);
  void                inFormat(cb::common::Format format);
  cb::common::Format  inFormat(void);

  void                outFormat(std::string format);
  void                outFormat(cb::common::Format format);
  cb::common::Format  outFormat(void);


  void    init(NgsiContainer container, bool isRequest, NgsiServiceType serviceType, bool mandatory, cb::common::Format format, std::string tag, std::string indent);
  virtual std::string  render(void)  { return "Not Implemented"; };
  virtual std::string  check(void)   { return "OK"; }
  virtual void         present(void) { printf("In NgsiService::present\n"); };
  virtual void         release(void) { };
};

}
}

#endif
