#ifndef NGSI_BASE_H
#define NGSI_BASE_H

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
#include "NgsiContainer.h"
#include "NgsiServiceType.h"


namespace cb
{

namespace ngsi
{


/* ****************************************************************************
*
* 'forward' classes - 
*/
class NgsiService;



/* ****************************************************************************
*
* NgsiBase - 
*/
class NgsiBase
{
protected:
  NgsiContainer        _container;    // To what 'container' do I belong?
  bool                 _isRequest;    // FALSE means it is part of a response
  NgsiServiceType      _serviceType;  // 'Service' Type (6 service types for NGSI-9 and 6 for NGSI-10)
  bool                 _mandatory;    // Self-explanatory - depends on 'container'
  bool                 _used;         // if (mandatory == true && used == false) ERROR
  std::string          _tag;          // used by the method 'render' 
  std::string          _indent;       // either here or input to many methods
  cb::common::Format   _format;       // XML/JSON/Test/

public:
  NgsiService*         _top;          // pointer to 'Top' - '_container' might be non-needed

  NgsiBase();

  // 'get' methods
  NgsiContainer        container(void);
  bool                 isRequest(void);
  NgsiServiceType      serviceType(void);
  bool                 mandatory(void);
  bool                 used(void);
  std::string          tag(void);
  std::string          indent(void);
  cb::common::Format   format(void);

  // 'set' methods
  void                 container(NgsiContainer container);
  void                 isRequest(bool isRequest);
  void                 serviceType(NgsiServiceType serviceType);
  void                 mandatory(bool mandatory);
  void                 used(bool used);
  void                 tag(std::string tag);
  void                 indent(std::string indent);
  void                 format(cb::common::Format format);

  virtual void         init(NgsiContainer container, bool isRequest, NgsiServiceType serviceType, bool mandatory, cb::common::Format format, std::string tag, std::string indent);
  virtual std::string  render(void)  = 0;
  virtual std::string  check(void)   = 0;
  virtual void         present(void) = 0;
  virtual void         release(void) = 0;

  std::string          tagStart();
  std::string          tagEnd();
};

}
}

#endif
