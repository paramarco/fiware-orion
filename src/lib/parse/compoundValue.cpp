/*
*
* Copyright 2014 Telefonica Investigacion y Desarrollo, S.A.U
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
#include "common/globals.h"

#include "ngsi/ParseData.h"
#include "parse/CompoundValueNode.h"
#include "parse/compoundValue.h"
#include "rest/ConnectionInfo.h"


namespace orion
{

/* ****************************************************************************
*
* compoundValueStart - 
*
* As commented in xmlParse.h
* This function is called when the first compound node is encountered, so not
* only must the root be created, but also the first node of the compound tree
* must be taken care of. This is done by calling compoundValueMiddle.
*/
void compoundValueStart
(
    ConnectionInfo*                 ciP,
    const std::string&              path,
    const std::string&              name,
    const std::string&              value,
    const std::string&              rest,
    orion::CompoundValueNode::Type  type,
    bool                            fatherIsVector
)
{
  ciP->inCompoundValue = true;

  ciP->compoundValueP = new orion::CompoundValueNode(orion::CompoundValueNode::Object);
  LM_T(LmtCompoundValueContainer, ("Set current container to '%s' (%s)", ciP->compoundValueP->path.c_str(), ciP->compoundValueP->name.c_str()));
  ciP->compoundValueRoot = ciP->compoundValueP;

  if (fatherIsVector)
    ciP->compoundValueP->type = orion::CompoundValueNode::Vector;

  // In the XML parsing routines, in all context attributes that can accept Compound values,
  // a pointer to the one where we are right now is saved in ParseData.
  // 
  // If this pointer is not set, it is a fatal error and the broker dies, because of the 
  // following LM_X, that does an exit
  // It is better to exit here and clearly see the error, than to continue and get strange
  // outputs that will be difficult to trace back to here.
  if (ciP->parseDataP->lastContextAttribute == NULL)
    orionExitFunction(1, "No pointer to last ContextAttribute");

  if (ciP->parseDataP->lastContextAttribute->typeFromXmlAttribute == "vector")
    ciP->compoundValueP->type = orion::CompoundValueNode::Vector;

  ciP->compoundValueVector.push_back(ciP->compoundValueP);
  LM_T(LmtCompoundValueAdd, ("Created new toplevel element"));
  compoundValueMiddle(ciP, rest, name, value, type);
}



/* ****************************************************************************
*
* compoundValueMiddle - 
*
* containerType: vector/object/string
*/
void compoundValueMiddle(ConnectionInfo* ciP, const std::string& relPath, const std::string& name, const std::string& value, orion::CompoundValueNode::Type type)
{
  LM_T(LmtCompoundValue, ("Compound MIDDLE %s: %s: NAME: '%s', VALUE: '%s'", relPath.c_str(), CompoundValueNode::typeName(type), name.c_str(), value.c_str()));
  if ((type == orion::CompoundValueNode::Vector) || (type == orion::CompoundValueNode::Object))
  {
    // If we enter a vector or an object, the container must change (so that we add to this container from now on).
    // ciP->compoundValueP points to the current compound container
    ciP->compoundValueP = ciP->compoundValueP->add(type, name);
    LM_T(LmtCompoundValueContainer, ("Set current container to '%s' (%s)", ciP->compoundValueP->path.c_str(), ciP->compoundValueP->name.c_str()));
  }
  else
  {
    ciP->compoundValueP->add(type, name, value);
  }
} 



/* ****************************************************************************
*
* compoundValueEnd - 
*/
void compoundValueEnd(ConnectionInfo* ciP, ParseData* parseDataP)
{
  LM_T(LmtCompoundValue, ("Compound END"));

  // Finish the compound value - error check included
  std::string status = ciP->compoundValueRoot->finish();

  // Any problems in 'finish'?
  // If so, mark as erroneous
  if (status != "OK")
  {
    ciP->httpStatusCode = SccBadRequest;
    ciP->answer = std::string("compound value error: ") + status;
    LM_W(("Bad Input (%s)", ciP->answer.c_str()));
  }

  // Give the root pointer of this Compound to the active ContextAttribute
  // lastContextAttribute is set in the XML parsing routiunes, to point at the
  // latest contextAttribute, i.e. the attribute whose 'contextValue' is the
  // owner of this compound value tree.
  LM_T(LmtCompoundValue, ("Set compoundValueP (%p) for attribute at %p", ciP->compoundValueRoot, parseDataP->lastContextAttribute));
  parseDataP->lastContextAttribute->compoundValueP = ciP->compoundValueRoot;

  // Reset the Compound stuff in ConnectionInfo
  ciP->compoundValueRoot = NULL;
  ciP->compoundValueP    = NULL;
  LM_T(LmtCompoundValueContainer, ("Set current container to NULL"));
  ciP->inCompoundValue   = false;
} 

}
