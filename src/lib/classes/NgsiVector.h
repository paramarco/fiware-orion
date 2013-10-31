#ifndef NGSI_VECTOR_H
#define NGSI_VECTOR_H

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
extern void exit(int);

#include "logMsg/logMsg.h"
#include "contextBrokerII/traceLevels.h"

#include "globals.h"
#include "Present.h"
#include "NgsiBase.h"

using namespace std;

namespace cb
{

namespace ngsi
{


/* ****************************************************************************
*
* NgsiVector - 
*/
template <class T> class NgsiVector: public NgsiBase
{
private:
  vector<T>     vec;
  std::string   _itemTag;

public:
  void          init(NgsiContainer container, bool isRequest, NgsiServiceType serviceType, bool mandatory, cb::common::Format format, std::string tag, std::string indent);
  void          init(NgsiContainer container, bool isRequest, NgsiServiceType serviceType, bool mandatory, cb::common::Format format, std::string tag, std::string itemTag, std::string indent);
  std::string   render(void);
  std::string   check(void);
  void          release(void);
  void          present(void);
  void          itemTag(std::string itemTagX);
  
  void          push_back(T item);
  unsigned int  size(void);
  T             get(int ix);
};



/* ****************************************************************************
*
* init - 
*/
template <class T> void NgsiVector<T>::init(NgsiContainer container, bool isRequest, NgsiServiceType serviceType, bool mandatory, cb::common::Format format, std::string tag, std::string itemTagX, std::string indent)
{
  NgsiBase::init(container, isRequest, serviceType, mandatory, format, tag, indent);
  itemTag(itemTagX);
}



/* ****************************************************************************
*
* init - 
*/
template <class T> void NgsiVector<T>::init(NgsiContainer container, bool isRequest, NgsiServiceType serviceType, bool mandatory, cb::common::Format format, std::string tag, std::string indent)
{
  printf("Please use the itemTag version of the method init for a vector!  (%s)\n", _tag.c_str());
  exit(1);
}



/* ****************************************************************************
*
* render - 
*/
template <class T> std::string NgsiVector<T>::render(void)
{
  std::string out = "";

  out += tagStart();

  for (unsigned int ix = 0; ix < vec.size(); ++ix)
     out += vec[ix]->render();

  out += tagEnd();

  return out;
}



/* ****************************************************************************
*
* check - 
*/
template <class T> std::string NgsiVector<T>::check(void)
{
  LM_T(LmtCheck, ("IN (vector with tag '%s')", _tag.c_str()));

  for (unsigned int ix = 0; ix < vec.size(); ++ix)
  {
    std::string s;

    if ((s = vec[ix]->check()) != "OK")
      return s;
  }

  return "OK";
}



/* ****************************************************************************
*
* present - 
*/
template <class T> void NgsiVector<T>::present(void)
{
  for (unsigned int ix = 0; ix < vec.size(); ++ix)
  {
    vec[ix]->present();
  }
}



/* ****************************************************************************
*
* release - 
*/
template <class T> void NgsiVector<T>::release(void)
{
}



/* ****************************************************************************
*
* push_back - 
*/
template <class T> void NgsiVector<T>::push_back(T item)
{
   // printf("'%s' pushing back vector item with tag '%s'\n", _tag.c_str(), _itemTag.c_str());

  item->init(_container, _isRequest, _serviceType, false, _format, _itemTag, _indent + TABULATOR);
  vec.push_back(item);
}



/* ****************************************************************************
*
* size - 
*/
template <class T> unsigned int NgsiVector<T>::size(void)
{
  return vec.size();
}



/* ****************************************************************************
*
* get - 
*/
template <class T> T NgsiVector<T>::get(int ix)
{
   return vec[ix];
}



/* ****************************************************************************
*
* itemTag - 
*/
template <class T> void NgsiVector<T>::itemTag(std::string itemTagX)
{
  _itemTag = itemTagX;
}

}
}

#endif
