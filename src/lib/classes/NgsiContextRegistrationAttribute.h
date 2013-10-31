#ifndef NGSI_CONTEXT_REGISTRATION_ATTRIBUTE_H
#define NGSI_CONTEXT_REGISTRATION_ATTRIBUTE_H

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
#include "NgsiBase.h"
#include "NgsiString.h"
#include "NgsiVector.h"
#include "NgsiMetadata.h"

namespace cb
{

namespace ngsi
{


/* ****************************************************************************
*
* NgsiContextRegistrationAttribute - 
*/
class NgsiContextRegistrationAttribute: public NgsiBase
{
public:
  NgsiString                                    _name;
  NgsiString                                    _type;
  NgsiString                                    _isDomain;
  NgsiVector<NgsiMetadata*>                     _contextMetadataVector;

public:
  NgsiContextRegistrationAttribute(std::string name, std::string type, std::string isDomain)
  {
     _name.value(name);
     _type.value(type);
     _isDomain.value(isDomain);
  }

  void init(NgsiContainer container, bool isRequest, NgsiServiceType serviceType, bool mandatory, cb::common::Format format, std::string tag, std::string indent)
  {
     NgsiBase::init(container,              isRequest, serviceType, mandatory, format, tag,                           indent);

     _name.init(container,                  isRequest, serviceType, true,      format, "name",                        indent + TABULATOR);
     _type.init(container,                  isRequest, serviceType, true,      format, "type",                        indent + TABULATOR);
     _isDomain.init(container,              isRequest, serviceType, false,     format, "isDomain",                    indent + TABULATOR);
     _contextMetadataVector.init(container, isRequest, serviceType, false,     format, "metadata", "contextMetadata", indent + TABULATOR);
  }

  std::string render()
  {
    std::string out = "";

    out += tagStart();
    out += _name.render();
    out += _type.render();
    out += _isDomain.render();
    out += _contextMetadataVector.render();
    out += tagEnd();

    return out;
  }

  std::string check(void)
  {
    return "OK";
  }

  void present(void)
  {
    _name.present();
    _type.present();
    _isDomain.present();
    _contextMetadataVector.present();
  }

  void release(void)
  {
    _name.release();
    _type.release();
    _isDomain.release();
    _contextMetadataVector.release();
  }
};

}
}

#endif
