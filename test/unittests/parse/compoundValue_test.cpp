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
#include "ngsi/ParseData.h"
#include "parse/CompoundValueNode.h"
#include "rest/ConnectionInfo.h"
#include "xmlParse/xmlRequest.h"
#include "jsonParse/jsonRequest.h"

#include "unittest.h"



/* ****************************************************************************
*
* updateNoCompoundValue -
*/
TEST(compoundValue, updateNoCompoundValue)
{
  ParseData         reqData;
  const char*       inFileXml   = "ngsi10.updateContextRequest.updateNoCompoundValue.valid.xml";
  const char*       inFileJson  = "ngsi10.updateContextRequest.updateNoCompoundValue.valid.json";
  ConnectionInfo    ci("/ngsi10/updateContext", "POST", "1.1");
  ContextAttribute* caP;
  std::string       result;

  utInit();

  EXPECT_EQ("OK", testDataFromFile(testBuf, sizeof(testBuf), inFileXml)) << "Error getting test data from '" << inFileXml << "'";
  result = xmlTreat(testBuf, &ci, &reqData, UpdateContext, "updateContextRequest", NULL);
  EXPECT_STREQ("OK", result.c_str());
  caP = reqData.upcr.res.contextElementVector.get(0)->contextAttributeVector.get(0);
  EXPECT_EQ("1", caP->value);

  EXPECT_EQ("OK", testDataFromFile(testBuf, sizeof(testBuf), inFileJson)) << "Error getting test data from '" << inFileJson << "'";
  ci.inFormat = JSON;
  result = jsonTreat(testBuf, &ci, &reqData, UpdateContext, "updateContextRequest", NULL);
  EXPECT_STREQ("OK", result.c_str());
  caP = reqData.upcr.res.contextElementVector.get(0)->contextAttributeVector.get(0);
  EXPECT_EQ("1", caP->value);
  

  utExit();
}



/* ****************************************************************************
*
* updateUnknownPath -
*/
TEST(compoundValue, updateUnknownPath)
{
  ParseData       reqData;
  const char*     inFileXml   = "ngsi10.updateContextRequest.updateUnknownPath.invalid.xml";
  const char*     inFileJson  = "ngsi10.updateContextRequest.updateUnknownPath.invalid.json";
  const char*     outFileXml  = "ngsi10.updateContextResponse.updateUnknownPath.valid.xml";
  const char*     outFileJson = "ngsi10.updateContextResponse.updateUnknownPath.valid.json";
  ConnectionInfo  ciXml("/ngsi10/updateContext", "POST", "1.1");
  ConnectionInfo  ciJson("/ngsi10/updateContext", "POST", "1.1");
  std::string     result;
  utInit();

  EXPECT_EQ("OK", testDataFromFile(testBuf, sizeof(testBuf), inFileXml)) << "Error getting test data from '" << inFileXml << "'";
  EXPECT_EQ("OK", testDataFromFile(expectedBuf, sizeof(expectedBuf), outFileXml)) << "Error getting test data from '" << outFileXml << "'";
  result = xmlTreat(testBuf, &ciXml, &reqData, UpdateContext, "updateContextRequest", NULL);
  EXPECT_STREQ(expectedBuf, result.c_str());

  EXPECT_EQ("OK", testDataFromFile(testBuf, sizeof(testBuf), inFileJson)) << "Error getting test data from '" << inFileJson << "'";
  EXPECT_EQ("OK", testDataFromFile(expectedBuf, sizeof(expectedBuf), outFileJson)) << "Error getting test data from '" << outFileJson << "'";
  ciJson.inFormat  = JSON;
  ciJson.outFormat = JSON;
  result = jsonTreat(testBuf, &ciJson, &reqData, UpdateContext, "updateContextRequest", NULL);
  EXPECT_STREQ(expectedBuf, result.c_str());

  utExit();
}



/* ****************************************************************************
*
* updateOneString - 
*/
TEST(compoundValue, updateOneString)
{
  ParseData                  reqData;
  const char*                inFile  = "ngsi10.updateContextRequest.updateOneString.valid.xml";
  ConnectionInfo             ci("/ngsi10/updateContext", "POST", "1.1");
  ContextAttribute*          caP;
  orion::CompoundValueNode*  cvnRootP;
  orion::CompoundValueNode*  childP;

  utInit();

  EXPECT_EQ("OK", testDataFromFile(testBuf, sizeof(testBuf), inFile)) << "Error getting test data from '" << inFile << "'";

  std::string result = xmlTreat(testBuf, &ci, &reqData, UpdateContext, "updateContextRequest", NULL);

  EXPECT_STREQ("OK", result.c_str());

  caP = reqData.upcr.res.contextElementVector.get(0)->contextAttributeVector.get(0);

  EXPECT_TRUE(caP != NULL);
  EXPECT_TRUE(caP->compoundValueP != NULL);

  // Get root of compound value
  cvnRootP = caP->compoundValueP;
  
  // The root pointer of the root must be the root itself
  EXPECT_EQ(cvnRootP, cvnRootP->rootP);

  // The root should be a struct in this test case
  EXPECT_EQ(orion::CompoundValueNode::Struct, cvnRootP->type);

  // The root should have exactly one child
  EXPECT_EQ(1, cvnRootP->childV.size());

  EXPECT_EQ(0, cvnRootP->level);
  EXPECT_EQ(0, cvnRootP->siblingNo);
  EXPECT_EQ("/", cvnRootP->path);
  EXPECT_EQ("/updateContextRequest/contextElementList/contextElement/contextAttributeList/contextAttribute/contextValue", cvnRootP->root);
  
  
  // The child
  childP = cvnRootP->childV[0];

  EXPECT_EQ("s1",                            childP->name);
  EXPECT_EQ(orion::CompoundValueNode::Leaf,  childP->type);
  EXPECT_EQ("STRING",                        childP->value);
  EXPECT_EQ(0,                               childP->childV.size());

  EXPECT_EQ(cvnRootP,                        childP->container);
  EXPECT_EQ(cvnRootP,                        childP->rootP);

  EXPECT_EQ("/s1",                           childP->path);
  EXPECT_EQ(1,                               childP->level);
  EXPECT_EQ(0,                               childP->siblingNo);

  utExit();
}



/* ****************************************************************************
*
* updateOneStringJson - 
*/
TEST(compoundValue, updateOneStringJson)
{
  ParseData                  reqData;
  const char*                inFile  = "ngsi10.updateContextRequest.updateOneString.valid.json";
  ConnectionInfo             ci("/ngsi10/updateContext", "POST", "1.1");
  ContextAttribute*          caP;
  orion::CompoundValueNode*  cvnRootP;
  orion::CompoundValueNode*  childP;

  utInit();

  EXPECT_EQ("OK", testDataFromFile(testBuf, sizeof(testBuf), inFile)) << "Error getting test data from '" << inFile << "'";

  ci.inFormat  = JSON;
  ci.outFormat = JSON;

  std::string result = jsonTreat(testBuf, &ci, &reqData, UpdateContext, "updateContextRequest", NULL);

  EXPECT_STREQ("OK", result.c_str());

  caP = reqData.upcr.res.contextElementVector.get(0)->contextAttributeVector.get(0);

  EXPECT_TRUE(caP != NULL);
  EXPECT_TRUE(caP->compoundValueP != NULL);

  // Get root of compound value
  cvnRootP = caP->compoundValueP;
  
  // The root pointer of the root must be the root itself
  EXPECT_EQ(cvnRootP, cvnRootP->rootP);

  // The root should be a struct in this test case
  EXPECT_EQ(orion::CompoundValueNode::Struct, cvnRootP->type);

  // The root should have exactly one child
  EXPECT_EQ(1, cvnRootP->childV.size());

  EXPECT_EQ(0, cvnRootP->level);
  EXPECT_EQ(0, cvnRootP->siblingNo);
  EXPECT_EQ("/", cvnRootP->path);
  EXPECT_EQ("/contextElements/contextElement/attributes/attribute/value/", cvnRootP->root);
  
  
  // The child
  childP = cvnRootP->childV[0];

  EXPECT_EQ("s1",                            childP->name);
  EXPECT_EQ(orion::CompoundValueNode::Leaf,  childP->type);
  EXPECT_EQ("STRING",                        childP->value);
  EXPECT_EQ(0,                               childP->childV.size());

  EXPECT_EQ(cvnRootP,                        childP->container);
  EXPECT_EQ(cvnRootP,                        childP->rootP);

  EXPECT_EQ("/s1",                           childP->path);
  EXPECT_EQ(1,                               childP->level);
  EXPECT_EQ(0,                               childP->siblingNo);

  utExit();
}



/* ****************************************************************************
*
* updateTwoStrings - 
*/
TEST(compoundValue, updateTwoStrings)
{
  ParseData                  reqData;
  const char*                inFile  = "ngsi10.updateContextRequest.updateTwoStrings.valid.xml";
  ConnectionInfo             ci("/ngsi10/updateContext", "POST", "1.1");
  ContextAttribute*          caP;
  orion::CompoundValueNode*  cvnRootP;
  orion::CompoundValueNode*  childP;

  utInit();

  EXPECT_EQ("OK", testDataFromFile(testBuf, sizeof(testBuf), inFile)) << "Error getting test data from '" << inFile << "'";

  std::string result = xmlTreat(testBuf, &ci, &reqData, UpdateContext, "updateContextRequest", NULL);

  EXPECT_STREQ("OK", result.c_str());

  caP = reqData.upcr.res.contextElementVector.get(0)->contextAttributeVector.get(0);
  EXPECT_TRUE(caP != NULL);
  EXPECT_TRUE(caP->compoundValueP != NULL);

  // Get root of compound value
  cvnRootP = caP->compoundValueP;
  
  // The root pointer of the root must be the root itself
  EXPECT_EQ(cvnRootP, cvnRootP->rootP);

  // The root should be a struct in this test case
  EXPECT_EQ(orion::CompoundValueNode::Struct, cvnRootP->type);

  // The root should have exactly two children
  EXPECT_EQ(2, cvnRootP->childV.size());

  EXPECT_EQ(0, cvnRootP->level);
  EXPECT_EQ(0, cvnRootP->siblingNo);
  EXPECT_EQ("/", cvnRootP->path);
  EXPECT_EQ("/updateContextRequest/contextElementList/contextElement/contextAttributeList/contextAttribute/contextValue", cvnRootP->root);

  // child 1
  childP = cvnRootP->childV[0];

  EXPECT_EQ("s1",                            childP->name);
  EXPECT_EQ(orion::CompoundValueNode::Leaf,  childP->type);
  EXPECT_EQ("STRING",                        childP->value);
  EXPECT_EQ(0,                               childP->childV.size());

  EXPECT_EQ(cvnRootP,                        childP->container);
  EXPECT_EQ(cvnRootP,                        childP->rootP);

  EXPECT_EQ("/s1",                           childP->path);
  EXPECT_EQ(1,                               childP->level);
  EXPECT_EQ(0,                               childP->siblingNo);

  // child 2
  childP = cvnRootP->childV[1];

  EXPECT_EQ("s2",                            childP->name);
  EXPECT_EQ(orion::CompoundValueNode::Leaf,  childP->type);
  EXPECT_EQ("STRING",                        childP->value);
  EXPECT_EQ(0,                               childP->childV.size());

  EXPECT_EQ(cvnRootP,                        childP->container);
  EXPECT_EQ(cvnRootP,                        childP->rootP);

  EXPECT_EQ("/s2",                           childP->path);
  EXPECT_EQ(1,                               childP->level);
  EXPECT_EQ(1,                               childP->siblingNo);

  utExit();
}



/* ****************************************************************************
*
* updateTwoStringsJson - 
*/
TEST(compoundValue, updateTwoStringsJson)
{
  ParseData                  reqData;
  const char*                inFile  = "ngsi10.updateContextRequest.updateTwoStrings.valid.json";
  ConnectionInfo             ci("/ngsi10/updateContext", "POST", "1.1");
  ContextAttribute*          caP;
  orion::CompoundValueNode*  cvnRootP;
  orion::CompoundValueNode*  childP;

  utInit();

  EXPECT_EQ("OK", testDataFromFile(testBuf, sizeof(testBuf), inFile)) << "Error getting test data from '" << inFile << "'";

  ci.inFormat  = JSON;
  ci.outFormat = JSON;
  std::string result = jsonTreat(testBuf, &ci, &reqData, UpdateContext, "updateContextRequest", NULL);

  EXPECT_STREQ("OK", result.c_str());

  caP = reqData.upcr.res.contextElementVector.get(0)->contextAttributeVector.get(0);
  EXPECT_TRUE(caP != NULL);
  EXPECT_TRUE(caP->compoundValueP != NULL);

  // Get root of compound value
  cvnRootP = caP->compoundValueP;
  
  // The root pointer of the root must be the root itself
  EXPECT_EQ(cvnRootP, cvnRootP->rootP);

  // The root should be a struct in this test case
  EXPECT_EQ(orion::CompoundValueNode::Struct, cvnRootP->type);

  // The root should have exactly two children
  EXPECT_EQ(2, cvnRootP->childV.size());

  EXPECT_EQ(0, cvnRootP->level);
  EXPECT_EQ(0, cvnRootP->siblingNo);
  EXPECT_EQ("/", cvnRootP->path);
  EXPECT_EQ("/contextElements/contextElement/attributes/attribute/value/", cvnRootP->root);

  // child 1
  childP = cvnRootP->childV[0];

  EXPECT_EQ("s1",                            childP->name);
  EXPECT_EQ(orion::CompoundValueNode::Leaf,  childP->type);
  EXPECT_EQ("STRING",                        childP->value);
  EXPECT_EQ(0,                               childP->childV.size());

  EXPECT_EQ(cvnRootP,                        childP->container);
  EXPECT_EQ(cvnRootP,                        childP->rootP);

  EXPECT_EQ("/s1",                           childP->path);
  EXPECT_EQ(1,                               childP->level);
  EXPECT_EQ(0,                               childP->siblingNo);

  // child 2
  childP = cvnRootP->childV[1];

  EXPECT_EQ("s2",                            childP->name);
  EXPECT_EQ(orion::CompoundValueNode::Leaf,  childP->type);
  EXPECT_EQ("STRING",                        childP->value);
  EXPECT_EQ(0,                               childP->childV.size());

  EXPECT_EQ(cvnRootP,                        childP->container);
  EXPECT_EQ(cvnRootP,                        childP->rootP);

  EXPECT_EQ("/s2",                           childP->path);
  EXPECT_EQ(1,                               childP->level);
  EXPECT_EQ(1,                               childP->siblingNo);

  utExit();
}



/* ****************************************************************************
*
* updateTwoItemsSameNameInStruct - 
*/
TEST(compoundValue, updateTwoItemsSameNameInStruct)
{
  ParseData       reqData;
  const char*     inFile  = "ngsi10.updateContextRequest.updateTwoItemsSameNameInStruct.valid.xml";
  const char*     outFile = "ngsi10.updateContextResponse.updateTwoItemsSameNameInStruct.valid.xml";
  ConnectionInfo  ci("/ngsi10/updateContext", "POST", "1.1");

  utInit();

  EXPECT_EQ("OK", testDataFromFile(testBuf, sizeof(testBuf), inFile)) << "Error getting test data from '" << inFile << "'";
  EXPECT_EQ("OK", testDataFromFile(expectedBuf, sizeof(expectedBuf), outFile)) << "Error getting test data from '" << outFile << "'";

  std::string result = xmlTreat(testBuf, &ci, &reqData, UpdateContext, "updateContextRequest", NULL);

  EXPECT_STREQ(expectedBuf, result.c_str());

  utExit();
}



/* ****************************************************************************
*
* updateTwoItemsSameNameInStructJson - 
*/
TEST(compoundValue, updateTwoItemsSameNameInStructJson)
{
  ParseData       reqData;
  const char*     inFile  = "ngsi10.updateContextRequest.updateTwoItemsSameNameInStruct.valid.json";
  const char*     outFile = "ngsi10.updateContextResponse.updateTwoItemsSameNameInStruct.valid.json";
  ConnectionInfo  ci("/ngsi10/updateContext", "POST", "1.1");

  utInit();

  EXPECT_EQ("OK", testDataFromFile(testBuf, sizeof(testBuf), inFile)) << "Error getting test data from '" << inFile << "'";
  EXPECT_EQ("OK", testDataFromFile(expectedBuf, sizeof(expectedBuf), outFile)) << "Error getting test data from '" << outFile << "'";

  ci.inFormat  = JSON;
  ci.outFormat = JSON;
  std::string result = jsonTreat(testBuf, &ci, &reqData, UpdateContext, "updateContextRequest", NULL);

  EXPECT_STREQ(expectedBuf, result.c_str());

  utExit();
}



/* ****************************************************************************
*
* updateContextValueVectorOneItem - 
*/
TEST(compoundValue, updateContextValueVectorOneItem)
{
  ParseData                  reqData;
  const char*                inFile  = "ngsi10.updateContextRequest.updateContextValueVectorOneItem.valid.xml";
  ConnectionInfo             ci("/ngsi10/updateContext", "POST", "1.1");
  ContextAttribute*          caP;
  orion::CompoundValueNode*  cvnRootP;
  orion::CompoundValueNode*  childP;

  utInit();

  EXPECT_EQ("OK", testDataFromFile(testBuf, sizeof(testBuf), inFile)) << "Error getting test data from '" << inFile << "'";

  std::string result = xmlTreat(testBuf, &ci, &reqData, UpdateContext, "updateContextRequest", NULL);

  EXPECT_STREQ("OK", result.c_str());

  caP = reqData.upcr.res.contextElementVector.get(0)->contextAttributeVector.get(0);
  
  EXPECT_TRUE(caP != NULL);
  EXPECT_TRUE(caP->compoundValueP != NULL);

  // Get root of compound value
  cvnRootP = caP->compoundValueP;
  
  // The root pointer of the root must be the root itself
  EXPECT_EQ(cvnRootP, cvnRootP->rootP);

  // The root should be a 'vector' in this test case
  EXPECT_EQ(orion::CompoundValueNode::Vector, cvnRootP->type);

  // The root should have exactly one child
  EXPECT_EQ(1, cvnRootP->childV.size());

  EXPECT_EQ(0, cvnRootP->level);
  EXPECT_EQ(0, cvnRootP->siblingNo);
  EXPECT_EQ("/", cvnRootP->path);
  EXPECT_EQ("/updateContextRequest/contextElementList/contextElement/contextAttributeList/contextAttribute/contextValue", cvnRootP->root);
  
  
  // The child
  childP = cvnRootP->childV[0];

  EXPECT_EQ("vecitem",                       childP->name);
  EXPECT_EQ(orion::CompoundValueNode::Leaf,  childP->type);
  EXPECT_EQ("1",                             childP->value);
  EXPECT_EQ(0,                               childP->childV.size());

  EXPECT_EQ(cvnRootP,                        childP->container);
  EXPECT_EQ(cvnRootP,                        childP->rootP);

  EXPECT_EQ("/vecitem",                      childP->path);
  EXPECT_EQ(1,                               childP->level);
  EXPECT_EQ(0,                               childP->siblingNo);

  utExit();
}



/* ****************************************************************************
*
* updateContextValueVectorOneItemJson - 
*/
TEST(compoundValue, updateContextValueVectorOneItemJson)
{
  ParseData                  reqData;
  const char*                inFile  = "ngsi10.updateContextRequest.updateContextValueVectorOneItem.valid.json";
  ConnectionInfo             ci("/ngsi10/updateContext", "POST", "1.1");
  ContextAttribute*          caP;
  orion::CompoundValueNode*  cvnRootP;
  orion::CompoundValueNode*  childP;

  utInit();

  EXPECT_EQ("OK", testDataFromFile(testBuf, sizeof(testBuf), inFile)) << "Error getting test data from '" << inFile << "'";
  ci.inFormat  = JSON;
  ci.outFormat = JSON;
  std::string result = jsonTreat(testBuf, &ci, &reqData, UpdateContext, "updateContextRequest", NULL);
  EXPECT_STREQ("OK", result.c_str());

  caP = reqData.upcr.res.contextElementVector.get(0)->contextAttributeVector.get(0);
  
  EXPECT_TRUE(caP != NULL);
  EXPECT_TRUE(caP->compoundValueP != NULL);

  // Get root of compound value
  cvnRootP = caP->compoundValueP;
  
  // The root pointer of the root must be the root itself
  EXPECT_EQ(cvnRootP, cvnRootP->rootP);

  // The root should be a 'vector' in this test case
  EXPECT_EQ(orion::CompoundValueNode::Vector, cvnRootP->type);

  // The root should have exactly one child
  EXPECT_EQ(1, cvnRootP->childV.size());

  EXPECT_EQ(0, cvnRootP->level);
  EXPECT_EQ(0, cvnRootP->siblingNo);
  EXPECT_EQ("/", cvnRootP->path);
  EXPECT_EQ("/contextElements/contextElement/attributes/attribute/value/", cvnRootP->root);
  
  
  // The child
  childP = cvnRootP->childV[0];

  EXPECT_EQ("item",                          childP->name);
  EXPECT_EQ(orion::CompoundValueNode::Leaf,  childP->type);
  EXPECT_EQ("1",                             childP->value);
  EXPECT_EQ(0,                               childP->childV.size());

  EXPECT_EQ(cvnRootP,                        childP->container);
  EXPECT_EQ(cvnRootP,                        childP->rootP);

  EXPECT_EQ("/item",                         childP->path);
  EXPECT_EQ(1,                               childP->level);
  EXPECT_EQ(0,                               childP->siblingNo);

  utExit();
}



/* ****************************************************************************
*
* updateContextValueVectorFiveItems - 
*/
TEST(compoundValue, updateContextValueVectorFiveItems)
{
  ParseData                  reqData;
  const char*                inFile  = "ngsi10.updateContextRequest.updateContextValueVectorFiveItems.valid.xml";
  ConnectionInfo             ci("/ngsi10/updateContext", "POST", "1.1");
  ContextAttribute*          caP;
  orion::CompoundValueNode*  cvnRootP;
  orion::CompoundValueNode*  childP;

  utInit();

  EXPECT_EQ("OK", testDataFromFile(testBuf, sizeof(testBuf), inFile)) << "Error getting test data from '" << inFile << "'";

  std::string result = xmlTreat(testBuf, &ci, &reqData, UpdateContext, "updateContextRequest", NULL);

  EXPECT_STREQ("OK", result.c_str());

  caP = reqData.upcr.res.contextElementVector.get(0)->contextAttributeVector.get(0);
  
  EXPECT_TRUE(caP != NULL);
  EXPECT_TRUE(caP->compoundValueP != NULL);

  // Get root of compound value
  cvnRootP = caP->compoundValueP;
  
  // The root pointer of the root must be the root itself
  EXPECT_EQ(cvnRootP, cvnRootP->rootP);

  // The root should be a 'vector' in this test case
  EXPECT_EQ(orion::CompoundValueNode::Vector, cvnRootP->type);

  // The root should have five children
  EXPECT_EQ(5, cvnRootP->childV.size());

  EXPECT_EQ(0, cvnRootP->level);
  EXPECT_EQ(0, cvnRootP->siblingNo);
  EXPECT_EQ("/", cvnRootP->path);
  EXPECT_EQ("/updateContextRequest/contextElementList/contextElement/contextAttributeList/contextAttribute/contextValue", cvnRootP->root);
  
  
  // Child 1-5
  std::string value[] = { "1", "2", "3", "4", "5" };
  for (unsigned int childIx = 0; childIx < 5; ++childIx)
  {
    childP = cvnRootP->childV[childIx];

    EXPECT_EQ("vecitem",                       childP->name);
    EXPECT_EQ(orion::CompoundValueNode::Leaf,  childP->type);
    EXPECT_EQ(value[childIx],                  childP->value);
    EXPECT_EQ(0,                               childP->childV.size());

    EXPECT_EQ(cvnRootP,                        childP->container);
    EXPECT_EQ(cvnRootP,                        childP->rootP);

    EXPECT_EQ("/vecitem",                      childP->path);
    EXPECT_EQ(1,                               childP->level);
    EXPECT_EQ(childIx,                         childP->siblingNo);
  }

  utExit();
}



/* ****************************************************************************
*
* updateContextValueVectorFiveItemsJson - 
*/
TEST(compoundValue, updateContextValueVectorFiveItemsJson)
{
  ParseData                  reqData;
  const char*                inFile  = "ngsi10.updateContextRequest.updateContextValueVectorFiveItems.valid.json";
  ConnectionInfo             ci("/ngsi10/updateContext", "POST", "1.1");
  ContextAttribute*          caP;
  orion::CompoundValueNode*  cvnRootP;
  orion::CompoundValueNode*  childP;

  utInit();

  EXPECT_EQ("OK", testDataFromFile(testBuf, sizeof(testBuf), inFile)) << "Error getting test data from '" << inFile << "'";
  ci.inFormat  = JSON;
  ci.outFormat = JSON;
  std::string result = jsonTreat(testBuf, &ci, &reqData, UpdateContext, "updateContextRequest", NULL);
  EXPECT_STREQ("OK", result.c_str());

  caP = reqData.upcr.res.contextElementVector.get(0)->contextAttributeVector.get(0);
  
  EXPECT_TRUE(caP != NULL);
  EXPECT_TRUE(caP->compoundValueP != NULL);

  // Get root of compound value
  cvnRootP = caP->compoundValueP;
  
  // The root pointer of the root must be the root itself
  EXPECT_EQ(cvnRootP, cvnRootP->rootP);

  // The root should be a 'vector' in this test case
  EXPECT_EQ(orion::CompoundValueNode::Vector, cvnRootP->type);

  // The root should have five children
  EXPECT_EQ(5, cvnRootP->childV.size());

  EXPECT_EQ(0, cvnRootP->level);
  EXPECT_EQ(0, cvnRootP->siblingNo);
  EXPECT_EQ("/", cvnRootP->path);
  EXPECT_EQ("/contextElements/contextElement/attributes/attribute/value/", cvnRootP->root);
  
  
  // Child 1-5
  std::string value[] = { "1", "2", "3", "4", "5" };
  for (unsigned int childIx = 0; childIx < 5; ++childIx)
  {
    childP = cvnRootP->childV[childIx];

    EXPECT_EQ("item",                          childP->name);
    EXPECT_EQ(orion::CompoundValueNode::Leaf,  childP->type);
    EXPECT_EQ(value[childIx],                  childP->value);
    EXPECT_EQ(0,                               childP->childV.size());

    EXPECT_EQ(cvnRootP,                        childP->container);
    EXPECT_EQ(cvnRootP,                        childP->rootP);

    EXPECT_EQ("/item",                         childP->path);
    EXPECT_EQ(1,                               childP->level);
    EXPECT_EQ(childIx,                         childP->siblingNo);
  }

  utExit();
}



/* ****************************************************************************
*
* updateContextValueVectorFiveItemsPlusBadOne - 
*/
TEST(compoundValue, updateContextValueVectorFiveItemsPlusBadOne)
{
  ParseData       reqData;
  const char*     inFile  = "ngsi10.updateContextRequest.updateContextValueVectorFiveItemsPlusBadOne.valid.xml";
  const char*     outFile = "ngsi10.updateContextResponse.updateContextValueVectorFiveItemsPlusBadOne.valid.xml";
  ConnectionInfo  ci("/ngsi10/updateContext", "POST", "1.1");

  utInit();

  EXPECT_EQ("OK", testDataFromFile(testBuf, sizeof(testBuf), inFile)) << "Error getting test data from '" << inFile << "'";
  EXPECT_EQ("OK", testDataFromFile(expectedBuf, sizeof(expectedBuf), outFile)) << "Error getting test data from '" << outFile << "'";

  std::string result = xmlTreat(testBuf, &ci, &reqData, UpdateContext, "updateContextRequest", NULL);

  EXPECT_STREQ(expectedBuf, result.c_str());

  utExit();
}



/* ****************************************************************************
*
* updateTwoStructs - 
*/
TEST(compoundValue, updateTwoStructs)
{
  ParseData                  reqData;
  const char*                inFile        = "ngsi10.updateContextRequest.updateTwoStructs.valid.xml";
  const char*                renderedFile  = "ngsi.contextAttribute.updateTwoStructsRendered.valid.xml";
  ConnectionInfo             ci("/ngsi10/updateContext", "POST", "1.1");
  ContextAttribute*          caP;
  std::string                rendered;

  utInit();

  EXPECT_EQ("OK", testDataFromFile(testBuf, sizeof(testBuf), inFile)) << "Error getting test data from '" << inFile << "'";
  std::string result = xmlTreat(testBuf, &ci, &reqData, UpdateContext, "updateContextRequest", NULL);
  EXPECT_STREQ("OK", result.c_str());

  caP = reqData.upcr.res.contextElementVector.get(0)->contextAttributeVector.get(0);
  

  EXPECT_TRUE(caP != NULL);
  EXPECT_TRUE(caP->compoundValueP != NULL);

  rendered = caP->render(XML, "");
  EXPECT_EQ("OK", testDataFromFile(expectedBuf, sizeof(expectedBuf), renderedFile)) << "Error getting test data from '" << renderedFile << "'";
  EXPECT_STREQ(expectedBuf, rendered.c_str());

  orion::CompoundValueNode*  cvnRootP;
  orion::CompoundValueNode*  structP;
  orion::CompoundValueNode*  childP;

  // Get root of compound value
  cvnRootP = caP->compoundValueP;
  
  // The root pointer of the root must be the root itself
  EXPECT_EQ(cvnRootP, cvnRootP->rootP);

  // The root should be a 'struct' in this test case
  EXPECT_EQ(orion::CompoundValueNode::Struct, cvnRootP->type);

  // The root should have two children
  EXPECT_EQ(2, cvnRootP->childV.size());

  EXPECT_EQ(0, cvnRootP->level);
  EXPECT_EQ(0, cvnRootP->siblingNo);
  EXPECT_EQ("/", cvnRootP->path);
  EXPECT_EQ("/updateContextRequest/contextElementList/contextElement/contextAttributeList/contextAttribute/contextValue", cvnRootP->root);
  

  // Now, child struct 1
  structP = cvnRootP->childV[0];

  EXPECT_EQ("struct1",                         structP->name);
  EXPECT_EQ(orion::CompoundValueNode::Struct,  structP->type);
  EXPECT_EQ(2,                                 structP->childV.size());

  EXPECT_EQ(cvnRootP,                          structP->container);
  EXPECT_EQ(cvnRootP,                          structP->rootP);

  EXPECT_EQ("/struct1",                        structP->path);
  EXPECT_EQ(1,                                 structP->level);
  EXPECT_EQ(0,                                 structP->siblingNo);


  // Child 1 of struct1
  childP = structP->childV[0];

  EXPECT_EQ("s1-1",                            childP->name);
  EXPECT_EQ(orion::CompoundValueNode::Leaf,    childP->type);
  EXPECT_EQ("1-1",                             childP->value);
  EXPECT_EQ(0,                                 childP->childV.size());

  EXPECT_EQ(structP,                           childP->container);
  EXPECT_EQ(cvnRootP,                          childP->rootP);

  EXPECT_EQ("/struct1/s1-1",                   childP->path);
  EXPECT_EQ(2,                                 childP->level);
  EXPECT_EQ(0,                                 childP->siblingNo);


  // Child 2 of struct1
  childP = cvnRootP->childV[0]->childV[1];

  EXPECT_EQ("s1-2",                            childP->name);
  EXPECT_EQ(orion::CompoundValueNode::Leaf,    childP->type);
  EXPECT_EQ("1-2",                             childP->value);
  EXPECT_EQ(0,                                 childP->childV.size());

  EXPECT_EQ(structP,                           childP->container);
  EXPECT_EQ(cvnRootP,                          childP->rootP);

  EXPECT_EQ("/struct1/s1-2",                   childP->path);
  EXPECT_EQ(2,                                 childP->level);
  EXPECT_EQ(1,                                 childP->siblingNo);



  // child struct 2
  structP = cvnRootP->childV[1];

  EXPECT_EQ("struct2",                         structP->name);
  EXPECT_EQ(orion::CompoundValueNode::Struct,  structP->type);
  EXPECT_EQ(2,                                 structP->childV.size());

  EXPECT_EQ(cvnRootP,                          structP->container);
  EXPECT_EQ(cvnRootP,                          structP->rootP);

  EXPECT_EQ("/struct2",                        structP->path);
  EXPECT_EQ(1,                                 structP->level);
  EXPECT_EQ(1,                                 structP->siblingNo);

  // Child 1 of struct2
  childP = cvnRootP->childV[1]->childV[0];

  EXPECT_EQ("s2-1",                            childP->name);
  EXPECT_EQ(orion::CompoundValueNode::Leaf,    childP->type);
  EXPECT_EQ("2-1",                             childP->value);
  EXPECT_EQ(0,                                 childP->childV.size());

  EXPECT_EQ(structP,                           childP->container);
  EXPECT_EQ(cvnRootP,                          childP->rootP);

  EXPECT_EQ("/struct2/s2-1",                   childP->path);
  EXPECT_EQ(2,                                 childP->level);
  EXPECT_EQ(0,                                 childP->siblingNo);


  // Child 2 of struct2
  childP = cvnRootP->childV[1]->childV[1];

  EXPECT_EQ("s2-2",                            childP->name);
  EXPECT_EQ(orion::CompoundValueNode::Leaf,    childP->type);
  EXPECT_EQ("2-2",                             childP->value);
  EXPECT_EQ(0,                                 childP->childV.size());

  EXPECT_EQ(structP,                           childP->container);
  EXPECT_EQ(cvnRootP,                          childP->rootP);

  EXPECT_EQ("/struct2/s2-2",                   childP->path);
  EXPECT_EQ(2,                                 childP->level);
  EXPECT_EQ(1,                                 childP->siblingNo);

  utExit();
}



/* ****************************************************************************
*
* updateTwoStructsJson - 
*/
TEST(compoundValue, updateTwoStructsJson)
{
  ParseData                  reqData;
  const char*                inFile        = "ngsi10.updateContextRequest.updateTwoStructs.valid.json";
  const char*                renderedFile  = "ngsi.contextAttribute.updateTwoStructsRendered.valid.json";
  ConnectionInfo             ci("/ngsi10/updateContext", "POST", "1.1");
  ContextAttribute*          caP;
  std::string                rendered;

  utInit();

  EXPECT_EQ("OK", testDataFromFile(testBuf, sizeof(testBuf), inFile)) << "Error getting test data from '" << inFile << "'";
  ci.inFormat  = JSON;
  ci.outFormat = JSON;
  std::string result = jsonTreat(testBuf, &ci, &reqData, UpdateContext, "updateContextRequest", NULL);
  EXPECT_STREQ("OK", result.c_str());

  caP = reqData.upcr.res.contextElementVector.get(0)->contextAttributeVector.get(0);
  

  EXPECT_TRUE(caP != NULL);
  EXPECT_TRUE(caP->compoundValueP != NULL);

  rendered = caP->render(JSON, "");
  EXPECT_EQ("OK", testDataFromFile(expectedBuf, sizeof(expectedBuf), renderedFile)) << "Error getting test data from '" << renderedFile << "'";
  EXPECT_STREQ(expectedBuf, rendered.c_str());

  orion::CompoundValueNode*  cvnRootP;
  orion::CompoundValueNode*  structP;
  orion::CompoundValueNode*  childP;

  // Get root of compound value
  cvnRootP = caP->compoundValueP;
  
  // The root pointer of the root must be the root itself
  EXPECT_EQ(cvnRootP, cvnRootP->rootP);

  // The root should be a 'struct' in this test case
  EXPECT_EQ(orion::CompoundValueNode::Struct, cvnRootP->type);

  // The root should have two children
  EXPECT_EQ(2, cvnRootP->childV.size());

  EXPECT_EQ(0, cvnRootP->level);
  EXPECT_EQ(0, cvnRootP->siblingNo);
  EXPECT_EQ("/", cvnRootP->path);
  EXPECT_EQ("/contextElements/contextElement/attributes/attribute/value/", cvnRootP->root);
  

  // Now, child struct 1
  structP = cvnRootP->childV[0];

  EXPECT_EQ("struct1",                         structP->name);
  EXPECT_EQ(orion::CompoundValueNode::Struct,  structP->type);
  EXPECT_EQ(2,                                 structP->childV.size());

  EXPECT_EQ(cvnRootP,                          structP->container);
  EXPECT_EQ(cvnRootP,                          structP->rootP);

  EXPECT_EQ("/struct1",                        structP->path);
  EXPECT_EQ(1,                                 structP->level);
  EXPECT_EQ(0,                                 structP->siblingNo);

  // Child 1 of struct1
  childP = structP->childV[0];

  EXPECT_EQ("s1-1",                            childP->name);
  EXPECT_EQ(orion::CompoundValueNode::Leaf,    childP->type);
  EXPECT_EQ("1-1",                             childP->value);
  EXPECT_EQ(0,                                 childP->childV.size());

  EXPECT_EQ(structP,                           childP->container);
  EXPECT_EQ(cvnRootP,                          childP->rootP);

  EXPECT_EQ("/struct1/s1-1",                   childP->path);
  EXPECT_EQ(2,                                 childP->level);
  EXPECT_EQ(0,                                 childP->siblingNo);


  // Child 2 of struct1
  childP = cvnRootP->childV[0]->childV[1];

  EXPECT_EQ("s1-2",                            childP->name);
  EXPECT_EQ(orion::CompoundValueNode::Leaf,    childP->type);
  EXPECT_EQ("1-2",                             childP->value);
  EXPECT_EQ(0,                                 childP->childV.size());

  EXPECT_EQ(structP,                           childP->container);
  EXPECT_EQ(cvnRootP,                          childP->rootP);

  EXPECT_EQ("/struct1/s1-2",                   childP->path);
  EXPECT_EQ(2,                                 childP->level);
  EXPECT_EQ(1,                                 childP->siblingNo);



  // child struct 2
  structP = cvnRootP->childV[1];

  EXPECT_EQ("struct2",                         structP->name);
  EXPECT_EQ(orion::CompoundValueNode::Struct,  structP->type);
  EXPECT_EQ(2,                                 structP->childV.size());

  EXPECT_EQ(cvnRootP,                          structP->container);
  EXPECT_EQ(cvnRootP,                          structP->rootP);

  EXPECT_EQ("/struct2",                        structP->path);
  EXPECT_EQ(1,                                 structP->level);
  EXPECT_EQ(1,                                 structP->siblingNo);

  // Child 1 of struct2
  childP = cvnRootP->childV[1]->childV[0];

  EXPECT_EQ("s2-1",                            childP->name);
  EXPECT_EQ(orion::CompoundValueNode::Leaf,    childP->type);
  EXPECT_EQ("2-1",                             childP->value);
  EXPECT_EQ(0,                                 childP->childV.size());

  EXPECT_EQ(structP,                           childP->container);
  EXPECT_EQ(cvnRootP,                          childP->rootP);

  EXPECT_EQ("/struct2/s2-1",                   childP->path);
  EXPECT_EQ(2,                                 childP->level);
  EXPECT_EQ(0,                                 childP->siblingNo);


  // Child 2 of struct2
  childP = cvnRootP->childV[1]->childV[1];

  EXPECT_EQ("s2-2",                            childP->name);
  EXPECT_EQ(orion::CompoundValueNode::Leaf,    childP->type);
  EXPECT_EQ("2-2",                             childP->value);
  EXPECT_EQ(0,                                 childP->childV.size());

  EXPECT_EQ(structP,                           childP->container);
  EXPECT_EQ(cvnRootP,                          childP->rootP);

  EXPECT_EQ("/struct2/s2-2",                   childP->path);
  EXPECT_EQ(2,                                 childP->level);
  EXPECT_EQ(1,                                 childP->siblingNo);

  utExit();
}



/* ****************************************************************************
*
* sixLevels - 
*/
TEST(compoundValue, sixLevels)
{
  ParseData                  reqData;
  const char*                inFile        = "ngsi10.updateContextRequest.sixLevels.valid.xml";
  const char*                renderedFile  = "ngsi.contextAttribute.updateSixLevelsRendered.valid.xml";
  ConnectionInfo             ci("/ngsi10/updateContext", "POST", "1.1");
  ContextAttribute*          caP;
  std::string                rendered;

  utInit();

  EXPECT_EQ("OK", testDataFromFile(testBuf, sizeof(testBuf), inFile)) << "Error getting test data from '" << inFile << "'";
  std::string result = xmlTreat(testBuf, &ci, &reqData, UpdateContext, "updateContextRequest", NULL);
  EXPECT_STREQ("OK", result.c_str());

  caP = reqData.upcr.res.contextElementVector.get(0)->contextAttributeVector.get(0);

  EXPECT_TRUE(caP != NULL);
  EXPECT_TRUE(caP->compoundValueP != NULL);

  rendered = caP->render(XML, "");
  EXPECT_EQ("OK", testDataFromFile(expectedBuf, sizeof(expectedBuf), renderedFile)) << "Error getting test data from '" << renderedFile << "'";
  EXPECT_STREQ(expectedBuf, rendered.c_str());

  orion::CompoundValueNode*  cvnRootP;
  orion::CompoundValueNode*  level1;
  orion::CompoundValueNode*  level2;
  orion::CompoundValueNode*  level3;
  orion::CompoundValueNode*  childP;
  orion::CompoundValueNode*  vitemP;
  orion::CompoundValueNode*  structP;

  // Get root of compound value
  cvnRootP = caP->compoundValueP;
  
  // The root pointer of the root must be the root itself
  EXPECT_EQ(cvnRootP, cvnRootP->rootP);

  // The root should be a 'struct' in this test case
  EXPECT_EQ(orion::CompoundValueNode::Struct, cvnRootP->type);

  // The root should have one child
  EXPECT_EQ(1, cvnRootP->childV.size());

  EXPECT_EQ(0, cvnRootP->level);
  EXPECT_EQ(0, cvnRootP->siblingNo);
  EXPECT_EQ("/", cvnRootP->path);
  EXPECT_EQ("/updateContextRequest/contextElementList/contextElement/contextAttributeList/contextAttribute/contextValue", cvnRootP->root);
  

  // Now, child 1: level1
  level1 = cvnRootP->childV[0];

  EXPECT_EQ("level1",                          level1->name);
  EXPECT_EQ(orion::CompoundValueNode::Struct,  level1->type);
  EXPECT_EQ("",                                level1->value);
  EXPECT_EQ(2,                                 level1->childV.size());

  EXPECT_EQ(cvnRootP,                          level1->container);
  EXPECT_EQ(cvnRootP,                          level1->rootP);

  EXPECT_EQ("/level1",                         level1->path);
  EXPECT_EQ(1,                                 level1->level);
  EXPECT_EQ(0,                                 level1->siblingNo);

  // /level1/level == 1
  childP = level1->childV[0];

  EXPECT_EQ("level",                           childP->name);
  EXPECT_EQ(orion::CompoundValueNode::Leaf,    childP->type);
  EXPECT_EQ("2",                               childP->value);
  EXPECT_EQ(0,                                 childP->childV.size());

  EXPECT_EQ(level1,                            childP->container);
  EXPECT_EQ(cvnRootP,                          childP->rootP);

  EXPECT_EQ("/level1/level",                   childP->path);
  EXPECT_EQ(2,                                 childP->level);
  EXPECT_EQ(0,                                 childP->siblingNo);


  // /level1/level2 == Struct
  level2 = level1->childV[1];

  EXPECT_EQ("level2",                          level2->name);
  EXPECT_EQ(orion::CompoundValueNode::Struct,  level2->type);
  EXPECT_EQ(2,                                 level2->childV.size());

  EXPECT_EQ(level1,                            level2->container);
  EXPECT_EQ(cvnRootP,                          level2->rootP);

  EXPECT_EQ("/level1/level2",                  level2->path);
  EXPECT_EQ(2,                                 level2->level);
  EXPECT_EQ(1,                                 level2->siblingNo);


  // /level1/level2/level == 2
  childP = level2->childV[0];

  EXPECT_EQ("level",                           childP->name);
  EXPECT_EQ(orion::CompoundValueNode::Leaf,    childP->type);
  EXPECT_EQ("3",                               childP->value);
  EXPECT_EQ(0,                                 childP->childV.size());

  EXPECT_EQ(level2,                            childP->container);
  EXPECT_EQ(cvnRootP,                          childP->rootP);

  EXPECT_EQ("/level1/level2/level",            childP->path);
  EXPECT_EQ(3,                                 childP->level);
  EXPECT_EQ(0,                                 childP->siblingNo);

  // /level1/level2/level3 == Vector
  level3 = level2->childV[1];

  EXPECT_EQ("level3",                          level3->name);
  EXPECT_EQ(orion::CompoundValueNode::Vector,  level3->type);
  EXPECT_EQ(2,                                 level3->childV.size());

  EXPECT_EQ(level2,                            level3->container);
  EXPECT_EQ(cvnRootP,                          level3->rootP);

  EXPECT_EQ("/level1/level2/level3",           level3->path);
  EXPECT_EQ(3,                                 level3->level);
  EXPECT_EQ(1,                                 level3->siblingNo);

  // /level1/level2/level3/level4item[0]
  vitemP = level3->childV[0];

  EXPECT_EQ("level4item",                       vitemP->name);
  EXPECT_EQ(orion::CompoundValueNode::Struct,   vitemP->type);
  EXPECT_EQ(2,                                  vitemP->childV.size());

  EXPECT_EQ(level3,                             vitemP->container);
  EXPECT_EQ(cvnRootP,                           vitemP->rootP);

  EXPECT_EQ("/level1/level2/level3/level4item", vitemP->path);
  EXPECT_EQ(4,                                  vitemP->level);
  EXPECT_EQ(0,                                  vitemP->siblingNo);
  
  // /level1/level2/level3/level4item[0]/level
  childP = vitemP->childV[0];

  EXPECT_EQ("level",                                  childP->name);
  EXPECT_EQ(orion::CompoundValueNode::Leaf,           childP->type);
  EXPECT_EQ("5",                                      childP->value);
  EXPECT_EQ(0,                                        childP->childV.size());

  EXPECT_EQ(vitemP,                                   childP->container);
  EXPECT_EQ(cvnRootP,                                 childP->rootP);

  EXPECT_EQ("/level1/level2/level3/level4item/level", childP->path);
  EXPECT_EQ(5,                                        childP->level);
  EXPECT_EQ(0,                                        childP->siblingNo);

  // /level1/level2/level3/level4item[0]/struct1
  structP = vitemP->childV[1];

  EXPECT_EQ("struct1",                                  structP->name);
  EXPECT_EQ(orion::CompoundValueNode::Struct,           structP->type);
  EXPECT_EQ(3,                                          structP->childV.size());

  EXPECT_EQ(vitemP,                                     structP->container);
  EXPECT_EQ(cvnRootP,                                   structP->rootP);

  EXPECT_EQ("/level1/level2/level3/level4item/struct1", structP->path);
  EXPECT_EQ(5,                                          structP->level);
  EXPECT_EQ(1,                                          structP->siblingNo);

  // /level1/level2/level3/level4item[0]/struct1/level
  childP = structP->childV[0];

  EXPECT_EQ("level",                                          childP->name);
  EXPECT_EQ(orion::CompoundValueNode::Leaf,                   childP->type);
  EXPECT_EQ("6",                                              childP->value);
  EXPECT_EQ(0,                                                childP->childV.size());

  EXPECT_EQ(structP,                                          childP->container);
  EXPECT_EQ(cvnRootP,                                         childP->rootP);

  EXPECT_EQ("/level1/level2/level3/level4item/struct1/level", childP->path);
  EXPECT_EQ(6,                                                childP->level);
  EXPECT_EQ(0,                                                childP->siblingNo);

  // /level1/level2/level3/level4item[0]/struct1/s1-1
  childP = structP->childV[1];

  EXPECT_EQ("s1-1" ,                                          childP->name);
  EXPECT_EQ(orion::CompoundValueNode::Leaf,                   childP->type);
  EXPECT_EQ("1-1",                                            childP->value);
  EXPECT_EQ(0,                                                childP->childV.size());

  EXPECT_EQ(structP,                                          childP->container);
  EXPECT_EQ(cvnRootP,                                         childP->rootP);

  EXPECT_EQ("/level1/level2/level3/level4item/struct1/s1-1",  childP->path);
  EXPECT_EQ(6,                                                childP->level);
  EXPECT_EQ(1,                                                childP->siblingNo);

  // /level1/level2/level3/level4item[0]/struct1/s1-2
  childP = structP->childV[2];

  EXPECT_EQ("s1-2" ,                                          childP->name);
  EXPECT_EQ(orion::CompoundValueNode::Leaf,                   childP->type);
  EXPECT_EQ("1-2",                                            childP->value);
  EXPECT_EQ(0,                                                childP->childV.size());

  EXPECT_EQ(structP,                                          childP->container);
  EXPECT_EQ(cvnRootP,                                         childP->rootP);

  EXPECT_EQ("/level1/level2/level3/level4item/struct1/s1-2",  childP->path);
  EXPECT_EQ(6,                                                childP->level);
  EXPECT_EQ(2,                                                childP->siblingNo);


  // /level1/level2/level3/level4item[1]
  vitemP = level3->childV[1];

  EXPECT_EQ("level4item",                       vitemP->name);
  EXPECT_EQ(orion::CompoundValueNode::Struct,   vitemP->type);
  EXPECT_EQ(2,                                  vitemP->childV.size());

  EXPECT_EQ(level3,                             vitemP->container);
  EXPECT_EQ(cvnRootP,                           vitemP->rootP);

  EXPECT_EQ("/level1/level2/level3/level4item", vitemP->path);
  EXPECT_EQ(4,                                  vitemP->level);
  EXPECT_EQ(1,                                  vitemP->siblingNo);
  
  // /level1/level2/level3/level4item[1]/level
  childP = vitemP->childV[0];

  EXPECT_EQ("level",                                  childP->name);
  EXPECT_EQ(orion::CompoundValueNode::Leaf,           childP->type);
  EXPECT_EQ("5",                                      childP->value);
  EXPECT_EQ(0,                                        childP->childV.size());

  EXPECT_EQ(vitemP,                                   childP->container);
  EXPECT_EQ(cvnRootP,                                 childP->rootP);

  EXPECT_EQ("/level1/level2/level3/level4item/level", childP->path);
  EXPECT_EQ(5,                                        childP->level);
  EXPECT_EQ(0,                                        childP->siblingNo);

  // /level1/level2/level3/level4item[1]/struct2
  structP = vitemP->childV[1];

  EXPECT_EQ("struct2",                                  structP->name);
  EXPECT_EQ(orion::CompoundValueNode::Struct,           structP->type);
  EXPECT_EQ(3,                                          structP->childV.size());

  EXPECT_EQ(vitemP,                                     structP->container);
  EXPECT_EQ(cvnRootP,                                   structP->rootP);

  EXPECT_EQ("/level1/level2/level3/level4item/struct2", structP->path);
  EXPECT_EQ(5,                                          structP->level);
  EXPECT_EQ(1,                                          structP->siblingNo);

  // /level1/level2/level3/level4item[1]/struct2/level
  childP = structP->childV[0];

  EXPECT_EQ("level",                                          childP->name);
  EXPECT_EQ(orion::CompoundValueNode::Leaf,                   childP->type);
  EXPECT_EQ("6",                                              childP->value);
  EXPECT_EQ(0,                                                childP->childV.size());

  EXPECT_EQ(structP,                                          childP->container);
  EXPECT_EQ(cvnRootP,                                         childP->rootP);

  EXPECT_EQ("/level1/level2/level3/level4item/struct2/level", childP->path);
  EXPECT_EQ(6,                                                childP->level);
  EXPECT_EQ(0,                                                childP->siblingNo);

  // /level1/level2/level3/level4item[1]/struct2/s2-1
  childP = structP->childV[1];

  EXPECT_EQ("s2-1" ,                                          childP->name);
  EXPECT_EQ(orion::CompoundValueNode::Leaf,                   childP->type);
  EXPECT_EQ("2-1",                                            childP->value);
  EXPECT_EQ(0,                                                childP->childV.size());

  EXPECT_EQ(structP,                                          childP->container);
  EXPECT_EQ(cvnRootP,                                         childP->rootP);

  EXPECT_EQ("/level1/level2/level3/level4item/struct2/s2-1",  childP->path);
  EXPECT_EQ(6,                                                childP->level);
  EXPECT_EQ(1,                                                childP->siblingNo);

  // /level1/level2/level3/level4item[1]/struct2/s2-2
  childP = structP->childV[2];

  EXPECT_EQ("s2-2" ,                                          childP->name);
  EXPECT_EQ(orion::CompoundValueNode::Leaf,                   childP->type);
  EXPECT_EQ("2-2",                                            childP->value);
  EXPECT_EQ(0,                                                childP->childV.size());

  EXPECT_EQ(structP,                                          childP->container);
  EXPECT_EQ(cvnRootP,                                         childP->rootP);

  EXPECT_EQ("/level1/level2/level3/level4item/struct2/s2-2",  childP->path);
  EXPECT_EQ(6,                                                childP->level);
  EXPECT_EQ(2,                                                childP->siblingNo);

  utExit();
}


#if 0
/* ****************************************************************************
*
* sixLevelsJson - 
*/
TEST(compoundValue, sixLevelsJson)
{
  ParseData                  reqData;
  const char*                inFile        = "ngsi10.updateContextRequest.sixLevels.valid.json";
  const char*                renderedFile  = "ngsi.contextAttribute.updateSixLevelsRendered.valid.json";
  ConnectionInfo             ci("/ngsi10/updateContext", "POST", "1.1");
  ContextAttribute*          caP;
  std::string                rendered;

  utInit();

  EXPECT_EQ("OK", testDataFromFile(testBuf, sizeof(testBuf), inFile)) << "Error getting test data from '" << inFile << "'";
  ci.inFormat  = JSON;
  ci.outFormat = JSON;
  std::string result = jsonTreat(testBuf, &ci, &reqData, UpdateContext, "updateContextRequest", NULL);
  EXPECT_STREQ("OK", result.c_str());

  caP = reqData.upcr.res.contextElementVector.get(0)->contextAttributeVector.get(0);

  EXPECT_TRUE(caP != NULL);
  EXPECT_TRUE(caP->compoundValueP != NULL);

  rendered = caP->render(JSON, "");
  EXPECT_EQ("OK", testDataFromFile(expectedBuf, sizeof(expectedBuf), renderedFile)) << "Error getting test data from '" << renderedFile << "'";
  EXPECT_STREQ(expectedBuf, rendered.c_str());

  orion::CompoundValueNode*  cvnRootP;
  orion::CompoundValueNode*  level1;
  orion::CompoundValueNode*  level2;
  orion::CompoundValueNode*  level3;
  orion::CompoundValueNode*  childP;
  orion::CompoundValueNode*  vitemP;
  orion::CompoundValueNode*  structP;

  // Get root of compound value
  cvnRootP = caP->compoundValueP;
  
  // The root pointer of the root must be the root itself
  EXPECT_EQ(cvnRootP, cvnRootP->rootP);

  // The root should be a 'struct' in this test case
  EXPECT_EQ(orion::CompoundValueNode::Struct, cvnRootP->type);

  // The root should have one child
  EXPECT_EQ(1, cvnRootP->childV.size());

  EXPECT_EQ(0, cvnRootP->level);
  EXPECT_EQ(0, cvnRootP->siblingNo);
  EXPECT_EQ("/", cvnRootP->path);
  EXPECT_EQ("/updateContextRequest/contextElementList/contextElement/contextAttributeList/contextAttribute/contextValue", cvnRootP->root);
  

  // Now, child 1: level1
  level1 = cvnRootP->childV[0];

  EXPECT_EQ("level1",                          level1->name);
  EXPECT_EQ(orion::CompoundValueNode::Struct,  level1->type);
  EXPECT_EQ("",                                level1->value);
  EXPECT_EQ(2,                                 level1->childV.size());

  EXPECT_EQ(cvnRootP,                          level1->container);
  EXPECT_EQ(cvnRootP,                          level1->rootP);

  EXPECT_EQ("/level1",                         level1->path);
  EXPECT_EQ(1,                                 level1->level);
  EXPECT_EQ(0,                                 level1->siblingNo);

  // /level1/level == 1
  childP = level1->childV[0];

  EXPECT_EQ("level",                           childP->name);
  EXPECT_EQ(orion::CompoundValueNode::Leaf,    childP->type);
  EXPECT_EQ("2",                               childP->value);
  EXPECT_EQ(0,                                 childP->childV.size());

  EXPECT_EQ(level1,                            childP->container);
  EXPECT_EQ(cvnRootP,                          childP->rootP);

  EXPECT_EQ("/level1/level",                   childP->path);
  EXPECT_EQ(2,                                 childP->level);
  EXPECT_EQ(0,                                 childP->siblingNo);


  // /level1/level2 == Struct
  level2 = level1->childV[1];

  EXPECT_EQ("level2",                          level2->name);
  EXPECT_EQ(orion::CompoundValueNode::Struct,  level2->type);
  EXPECT_EQ(2,                                 level2->childV.size());

  EXPECT_EQ(level1,                            level2->container);
  EXPECT_EQ(cvnRootP,                          level2->rootP);

  EXPECT_EQ("/level1/level2",                  level2->path);
  EXPECT_EQ(2,                                 level2->level);
  EXPECT_EQ(1,                                 level2->siblingNo);


  // /level1/level2/level == 2
  childP = level2->childV[0];

  EXPECT_EQ("level",                           childP->name);
  EXPECT_EQ(orion::CompoundValueNode::Leaf,    childP->type);
  EXPECT_EQ("3",                               childP->value);
  EXPECT_EQ(0,                                 childP->childV.size());

  EXPECT_EQ(level2,                            childP->container);
  EXPECT_EQ(cvnRootP,                          childP->rootP);

  EXPECT_EQ("/level1/level2/level",            childP->path);
  EXPECT_EQ(3,                                 childP->level);
  EXPECT_EQ(0,                                 childP->siblingNo);

  // /level1/level2/level3 == Vector
  level3 = level2->childV[1];

  EXPECT_EQ("level3",                          level3->name);
  EXPECT_EQ(orion::CompoundValueNode::Vector,  level3->type);
  EXPECT_EQ(2,                                 level3->childV.size());

  EXPECT_EQ(level2,                            level3->container);
  EXPECT_EQ(cvnRootP,                          level3->rootP);

  EXPECT_EQ("/level1/level2/level3",           level3->path);
  EXPECT_EQ(3,                                 level3->level);
  EXPECT_EQ(1,                                 level3->siblingNo);

  // /level1/level2/level3/level4item[0]
  vitemP = level3->childV[0];

  EXPECT_EQ("level4item",                       vitemP->name);
  EXPECT_EQ(orion::CompoundValueNode::Struct,   vitemP->type);
  EXPECT_EQ(2,                                  vitemP->childV.size());

  EXPECT_EQ(level3,                             vitemP->container);
  EXPECT_EQ(cvnRootP,                           vitemP->rootP);

  EXPECT_EQ("/level1/level2/level3/level4item", vitemP->path);
  EXPECT_EQ(4,                                  vitemP->level);
  EXPECT_EQ(0,                                  vitemP->siblingNo);
  
  // /level1/level2/level3/level4item[0]/level
  childP = vitemP->childV[0];

  EXPECT_EQ("level",                                  childP->name);
  EXPECT_EQ(orion::CompoundValueNode::Leaf,           childP->type);
  EXPECT_EQ("5",                                      childP->value);
  EXPECT_EQ(0,                                        childP->childV.size());

  EXPECT_EQ(vitemP,                                   childP->container);
  EXPECT_EQ(cvnRootP,                                 childP->rootP);

  EXPECT_EQ("/level1/level2/level3/level4item/level", childP->path);
  EXPECT_EQ(5,                                        childP->level);
  EXPECT_EQ(0,                                        childP->siblingNo);

  // /level1/level2/level3/level4item[0]/struct1
  structP = vitemP->childV[1];

  EXPECT_EQ("struct1",                                  structP->name);
  EXPECT_EQ(orion::CompoundValueNode::Struct,           structP->type);
  EXPECT_EQ(3,                                          structP->childV.size());

  EXPECT_EQ(vitemP,                                     structP->container);
  EXPECT_EQ(cvnRootP,                                   structP->rootP);

  EXPECT_EQ("/level1/level2/level3/level4item/struct1", structP->path);
  EXPECT_EQ(5,                                          structP->level);
  EXPECT_EQ(1,                                          structP->siblingNo);

  // /level1/level2/level3/level4item[0]/struct1/level
  childP = structP->childV[0];

  EXPECT_EQ("level",                                          childP->name);
  EXPECT_EQ(orion::CompoundValueNode::Leaf,                   childP->type);
  EXPECT_EQ("6",                                              childP->value);
  EXPECT_EQ(0,                                                childP->childV.size());

  EXPECT_EQ(structP,                                          childP->container);
  EXPECT_EQ(cvnRootP,                                         childP->rootP);

  EXPECT_EQ("/level1/level2/level3/level4item/struct1/level", childP->path);
  EXPECT_EQ(6,                                                childP->level);
  EXPECT_EQ(0,                                                childP->siblingNo);

  // /level1/level2/level3/level4item[0]/struct1/s1-1
  childP = structP->childV[1];

  EXPECT_EQ("s1-1" ,                                          childP->name);
  EXPECT_EQ(orion::CompoundValueNode::Leaf,                   childP->type);
  EXPECT_EQ("1-1",                                            childP->value);
  EXPECT_EQ(0,                                                childP->childV.size());

  EXPECT_EQ(structP,                                          childP->container);
  EXPECT_EQ(cvnRootP,                                         childP->rootP);

  EXPECT_EQ("/level1/level2/level3/level4item/struct1/s1-1",  childP->path);
  EXPECT_EQ(6,                                                childP->level);
  EXPECT_EQ(1,                                                childP->siblingNo);

  // /level1/level2/level3/level4item[0]/struct1/s1-2
  childP = structP->childV[2];

  EXPECT_EQ("s1-2" ,                                          childP->name);
  EXPECT_EQ(orion::CompoundValueNode::Leaf,                   childP->type);
  EXPECT_EQ("1-2",                                            childP->value);
  EXPECT_EQ(0,                                                childP->childV.size());

  EXPECT_EQ(structP,                                          childP->container);
  EXPECT_EQ(cvnRootP,                                         childP->rootP);

  EXPECT_EQ("/level1/level2/level3/level4item/struct1/s1-2",  childP->path);
  EXPECT_EQ(6,                                                childP->level);
  EXPECT_EQ(2,                                                childP->siblingNo);


  // /level1/level2/level3/level4item[1]
  vitemP = level3->childV[1];

  EXPECT_EQ("level4item",                       vitemP->name);
  EXPECT_EQ(orion::CompoundValueNode::Struct,   vitemP->type);
  EXPECT_EQ(2,                                  vitemP->childV.size());

  EXPECT_EQ(level3,                             vitemP->container);
  EXPECT_EQ(cvnRootP,                           vitemP->rootP);

  EXPECT_EQ("/level1/level2/level3/level4item", vitemP->path);
  EXPECT_EQ(4,                                  vitemP->level);
  EXPECT_EQ(1,                                  vitemP->siblingNo);
  
  // /level1/level2/level3/level4item[1]/level
  childP = vitemP->childV[0];

  EXPECT_EQ("level",                                  childP->name);
  EXPECT_EQ(orion::CompoundValueNode::Leaf,           childP->type);
  EXPECT_EQ("5",                                      childP->value);
  EXPECT_EQ(0,                                        childP->childV.size());

  EXPECT_EQ(vitemP,                                   childP->container);
  EXPECT_EQ(cvnRootP,                                 childP->rootP);

  EXPECT_EQ("/level1/level2/level3/level4item/level", childP->path);
  EXPECT_EQ(5,                                        childP->level);
  EXPECT_EQ(0,                                        childP->siblingNo);

  // /level1/level2/level3/level4item[1]/struct2
  structP = vitemP->childV[1];

  EXPECT_EQ("struct2",                                  structP->name);
  EXPECT_EQ(orion::CompoundValueNode::Struct,           structP->type);
  EXPECT_EQ(3,                                          structP->childV.size());

  EXPECT_EQ(vitemP,                                     structP->container);
  EXPECT_EQ(cvnRootP,                                   structP->rootP);

  EXPECT_EQ("/level1/level2/level3/level4item/struct2", structP->path);
  EXPECT_EQ(5,                                          structP->level);
  EXPECT_EQ(1,                                          structP->siblingNo);

  // /level1/level2/level3/level4item[1]/struct2/level
  childP = structP->childV[0];

  EXPECT_EQ("level",                                          childP->name);
  EXPECT_EQ(orion::CompoundValueNode::Leaf,                   childP->type);
  EXPECT_EQ("6",                                              childP->value);
  EXPECT_EQ(0,                                                childP->childV.size());

  EXPECT_EQ(structP,                                          childP->container);
  EXPECT_EQ(cvnRootP,                                         childP->rootP);

  EXPECT_EQ("/level1/level2/level3/level4item/struct2/level", childP->path);
  EXPECT_EQ(6,                                                childP->level);
  EXPECT_EQ(0,                                                childP->siblingNo);

  // /level1/level2/level3/level4item[1]/struct2/s2-1
  childP = structP->childV[1];

  EXPECT_EQ("s2-1" ,                                          childP->name);
  EXPECT_EQ(orion::CompoundValueNode::Leaf,                   childP->type);
  EXPECT_EQ("2-1",                                            childP->value);
  EXPECT_EQ(0,                                                childP->childV.size());

  EXPECT_EQ(structP,                                          childP->container);
  EXPECT_EQ(cvnRootP,                                         childP->rootP);

  EXPECT_EQ("/level1/level2/level3/level4item/struct2/s2-1",  childP->path);
  EXPECT_EQ(6,                                                childP->level);
  EXPECT_EQ(1,                                                childP->siblingNo);

  // /level1/level2/level3/level4item[1]/struct2/s2-2
  childP = structP->childV[2];

  EXPECT_EQ("s2-2" ,                                          childP->name);
  EXPECT_EQ(orion::CompoundValueNode::Leaf,                   childP->type);
  EXPECT_EQ("2-2",                                            childP->value);
  EXPECT_EQ(0,                                                childP->childV.size());

  EXPECT_EQ(structP,                                          childP->container);
  EXPECT_EQ(cvnRootP,                                         childP->rootP);

  EXPECT_EQ("/level1/level2/level3/level4item/struct2/s2-2",  childP->path);
  EXPECT_EQ(6,                                                childP->level);
  EXPECT_EQ(2,                                                childP->siblingNo);

  utExit();
}
#endif



/* ****************************************************************************
*
* updateOneStringAndOneVectorInSeparateContextValues - 
*/
TEST(compoundValue, updateOneStringAndOneVectorInSeparateContextValues)
{
  ParseData                  reqData;
  const char*                inFile  = "ngsi10.updateContextRequest.updateOneStringAndOneVectorInSeparateContextValues.valid.xml";
  ConnectionInfo             ci("/ngsi10/updateContext", "POST", "1.1");
  ContextAttribute*          caP;
  orion::CompoundValueNode*  cvnRootP;
  orion::CompoundValueNode*  childP;

  utInit();

  EXPECT_EQ("OK", testDataFromFile(testBuf, sizeof(testBuf), inFile)) << "Error getting test data from '" << inFile << "'";

  std::string result = xmlTreat(testBuf, &ci, &reqData, UpdateContext, "updateContextRequest", NULL);

  EXPECT_STREQ("OK", result.c_str());

  caP = reqData.upcr.res.contextElementVector.get(0)->contextAttributeVector.get(0);

  EXPECT_TRUE(caP != NULL);
  EXPECT_TRUE(caP->compoundValueP != NULL);

  // Get root of compound value
  cvnRootP = caP->compoundValueP;
  
  // The root pointer of the root must be the root itself
  EXPECT_EQ(cvnRootP, cvnRootP->rootP);

  // The root should be a struct in this test case
  EXPECT_EQ(orion::CompoundValueNode::Struct, cvnRootP->type);

  // The root should have exactly one child
  EXPECT_EQ(1, cvnRootP->childV.size());

  EXPECT_EQ(0, cvnRootP->level);
  EXPECT_EQ(0, cvnRootP->siblingNo);
  EXPECT_EQ("/", cvnRootP->path);
  EXPECT_EQ("/updateContextRequest/contextElementList/contextElement/contextAttributeList/contextAttribute/contextValue", cvnRootP->root);
  
  
  // The child
  childP = cvnRootP->childV[0];

  EXPECT_EQ("s1",                            childP->name);
  EXPECT_EQ(orion::CompoundValueNode::Leaf,  childP->type);
  EXPECT_EQ("STRING",                        childP->value);
  EXPECT_EQ(0,                               childP->childV.size());

  EXPECT_EQ(cvnRootP,                        childP->container);
  EXPECT_EQ(cvnRootP,                        childP->rootP);

  EXPECT_EQ("/s1",                           childP->path);
  EXPECT_EQ(1,                               childP->level);
  EXPECT_EQ(0,                               childP->siblingNo);


  // 
  // Now the second ContextAttribute, that also has a Compound value
  //
  caP = reqData.upcr.res.contextElementVector.get(0)->contextAttributeVector.get(1);

  EXPECT_TRUE(caP != NULL);
  EXPECT_TRUE(caP->compoundValueP != NULL);

  // Get root of compound value
  cvnRootP = caP->compoundValueP;
  
  // The root pointer of the root must be the root itself
  EXPECT_EQ(cvnRootP, cvnRootP->rootP);

  // The root should be a vector in this test case
  EXPECT_EQ(orion::CompoundValueNode::Vector, cvnRootP->type);

  // The root should have four children
  EXPECT_EQ(4, cvnRootP->childV.size());

  EXPECT_EQ(0, cvnRootP->level);
  EXPECT_EQ(0, cvnRootP->siblingNo);
  EXPECT_EQ("/", cvnRootP->path);
  EXPECT_EQ("/updateContextRequest/contextElementList/contextElement/contextAttributeList/contextAttribute/contextValue", cvnRootP->root);
  
  
  // The children
  const char* value[] = { "I-0", "I-1", "I-2", "I-3" };
  for (unsigned int ix = 0; ix < 4; ++ix)
  {
    childP = cvnRootP->childV[ix];

    EXPECT_EQ("item",                          childP->name);
    EXPECT_EQ(orion::CompoundValueNode::Leaf,  childP->type);
    EXPECT_EQ(value[ix],                       childP->value);
    EXPECT_EQ(0,                               childP->childV.size());

    EXPECT_EQ(cvnRootP,                        childP->container);
    EXPECT_EQ(cvnRootP,                        childP->rootP);

    EXPECT_EQ("/item",                         childP->path);
    EXPECT_EQ(1,                               childP->level);
    EXPECT_EQ(ix,                              childP->siblingNo);
  }

  utExit();
}



/* ****************************************************************************
*
* updateOneStringAndOneVectorInSeparateContextValuesJson - 
*/
TEST(compoundValue, updateOneStringAndOneVectorInSeparateContextValuesJson)
{
  ParseData                  reqData;
  const char*                inFile  = "ngsi10.updateContextRequest.updateOneStringAndOneVectorInSeparateContextValues.valid.json";
  ConnectionInfo             ci("/ngsi10/updateContext", "POST", "1.1");
  ContextAttribute*          caP;
  orion::CompoundValueNode*  cvnRootP;
  orion::CompoundValueNode*  childP;

  utInit();

  EXPECT_EQ("OK", testDataFromFile(testBuf, sizeof(testBuf), inFile)) << "Error getting test data from '" << inFile << "'";
  ci.inFormat  = JSON;
  ci.outFormat = JSON;
  std::string result = jsonTreat(testBuf, &ci, &reqData, UpdateContext, "updateContextRequest", NULL);
  EXPECT_STREQ("OK", result.c_str());

  caP = reqData.upcr.res.contextElementVector.get(0)->contextAttributeVector.get(0);

  EXPECT_TRUE(caP != NULL);
  EXPECT_TRUE(caP->compoundValueP != NULL);

  // Get root of compound value
  cvnRootP = caP->compoundValueP;
  
  // The root pointer of the root must be the root itself
  EXPECT_EQ(cvnRootP, cvnRootP->rootP);

  // The root should be a struct in this test case
  EXPECT_EQ(orion::CompoundValueNode::Struct, cvnRootP->type);

  // The root should have exactly one child
  EXPECT_EQ(1, cvnRootP->childV.size());

  EXPECT_EQ(0, cvnRootP->level);
  EXPECT_EQ(0, cvnRootP->siblingNo);
  EXPECT_EQ("/", cvnRootP->path);
  EXPECT_EQ("/contextElements/contextElement/attributes/attribute/value/", cvnRootP->root);
  
  
  // The child
  childP = cvnRootP->childV[0];

  EXPECT_EQ("s1",                            childP->name);
  EXPECT_EQ(orion::CompoundValueNode::Leaf,  childP->type);
  EXPECT_EQ("STRING",                        childP->value);
  EXPECT_EQ(0,                               childP->childV.size());

  EXPECT_EQ(cvnRootP,                        childP->container);
  EXPECT_EQ(cvnRootP,                        childP->rootP);

  EXPECT_EQ("/s1",                           childP->path);
  EXPECT_EQ(1,                               childP->level);
  EXPECT_EQ(0,                               childP->siblingNo);


  // 
  // Now the second ContextAttribute, that also has a Compound value
  //
  caP = reqData.upcr.res.contextElementVector.get(0)->contextAttributeVector.get(1);

  EXPECT_TRUE(caP != NULL);
  EXPECT_TRUE(caP->compoundValueP != NULL);

  // Get root of compound value
  cvnRootP = caP->compoundValueP;
  
  // The root pointer of the root must be the root itself
  EXPECT_EQ(cvnRootP, cvnRootP->rootP);

  // The root should be a vector in this test case
  EXPECT_EQ(orion::CompoundValueNode::Vector, cvnRootP->type);

  // The root should have four children
  EXPECT_EQ(4, cvnRootP->childV.size());

  EXPECT_EQ(0, cvnRootP->level);
  EXPECT_EQ(0, cvnRootP->siblingNo);
  EXPECT_EQ("/", cvnRootP->path);
  EXPECT_EQ("/contextElements/contextElement/attributes/attribute/value/", cvnRootP->root);
  
  
  // The children
  const char* value[] = { "I-0", "I-1", "I-2", "I-3" };
  for (unsigned int ix = 0; ix < 4; ++ix)
  {
    childP = cvnRootP->childV[ix];

    EXPECT_EQ("item",                          childP->name);
    EXPECT_EQ(orion::CompoundValueNode::Leaf,  childP->type);
    EXPECT_EQ(value[ix],                       childP->value);
    EXPECT_EQ(0,                               childP->childV.size());

    EXPECT_EQ(cvnRootP,                        childP->container);
    EXPECT_EQ(cvnRootP,                        childP->rootP);

    EXPECT_EQ("/item",                         childP->path);
    EXPECT_EQ(1,                               childP->level);
    EXPECT_EQ(ix,                              childP->siblingNo);
  }

  utExit();
}



/* ****************************************************************************
*
* tenCompounds - 
*/
TEST(compoundValue, tenCompounds)
{
  ParseData                  reqData;
  const char*                inFile       = "ngsi10.updateContextRequest.tenCompounds.valid.xml";
  const char*                renderedFile = "ngsi10.updateContextRequest.tenCompoundsRendered.valid.xml";
  ConnectionInfo             ci("/ngsi10/updateContext", "POST", "1.1");
  UpdateContextRequest*      upcrP;
  std::string                rendered;
   
  utInit();

  EXPECT_EQ("OK", testDataFromFile(testBuf, sizeof(testBuf), inFile)) << "Error getting test data from '" << inFile << "'";
  std::string result = xmlTreat(testBuf, &ci, &reqData, UpdateContext, "updateContextRequest", NULL);
  EXPECT_STREQ("OK", result.c_str());

  upcrP = &reqData.upcr.res;
  rendered = upcrP->render(UpdateContext, XML, "");

  EXPECT_EQ("OK", testDataFromFile(expectedBuf, sizeof(expectedBuf), renderedFile)) << "Error getting test data from '" << renderedFile << "'";
  EXPECT_STREQ(expectedBuf, rendered.c_str());

  utExit();
}
