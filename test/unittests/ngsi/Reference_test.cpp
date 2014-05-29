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
#include "logMsg/logMsg.h"
#include "logMsg/traceLevels.h"

#include "ngsi/Reference.h"

#include "unittest.h"



/* ****************************************************************************
*
* check - should Reference::check always return "OK"?
*/
TEST(Reference, check)
{
  Reference    reference;
  std::string  checked;
  std::string  expected = "OK";
  
  utInit();

  checked = reference.check(RegisterContext, XML, "", "", 0);
  EXPECT_STREQ("OK", checked.c_str());

  utExit();
}



/* ****************************************************************************
*
* isEmptySetAndGet - 
*/
TEST(Reference, isEmptySetAndGet)
{
  Reference  reference;

  utInit();

  reference.set("REF");
  EXPECT_STREQ("REF", reference.get().c_str());
  EXPECT_FALSE(reference.isEmpty());

  reference.set("");
  EXPECT_TRUE(reference.isEmpty());

  utExit();
}



/* ****************************************************************************
*
* present - no output expected, just exercising the code
*/
TEST(Reference, present)
{
  Reference   reference;

  utInit();

  reference.set("");
  reference.present("");

  reference.set("STR");
  reference.present("");

  utExit();
}



/* ****************************************************************************
*
* render - 
*/
TEST(Reference, render)
{
  Reference    reference;
  std::string  out;
  const char*  outfile1 = "ngsi.reference.render.middle.xml";
  const char*  outfile2 = "ngsi.reference.render.middle.json";

  utInit();

  reference .set("");
  out = reference.render(XML, "", false);
  EXPECT_STREQ("", out.c_str());

  reference .set("REF");
  out = reference.render(XML, "", false);
  EXPECT_EQ("OK", testDataFromFile(expectedBuf, sizeof(expectedBuf), outfile1)) << "Error getting test data from '" << outfile1 << "'";
  EXPECT_STREQ(expectedBuf, out.c_str());

  out = reference.render(JSON, "", false);
  EXPECT_EQ("OK", testDataFromFile(expectedBuf, sizeof(expectedBuf), outfile2)) << "Error getting test data from '" << outfile2 << "'";
  EXPECT_STREQ(expectedBuf, out.c_str());

  utExit();
}



/* ****************************************************************************
*
* c_str - 
*/
TEST(Reference, c_str)
{
  Reference   reference;

  utInit();

  reference.set("STR");
  EXPECT_STREQ("STR", reference.c_str());

  utExit();
}
