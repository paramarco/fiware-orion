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
#include "gtest/gtest.h"

#include "serviceRoutines/badVerbGetDeleteOnly.h"
#include "serviceRoutines/statisticsTreat.h"
#include "rest/RestService.h"



/* ****************************************************************************
*
* rs - 
*/
static RestService rs[] = 
{
  { "GET",    StatisticsRequest, 1, { "statistics" }, "", statisticsTreat      },
  { "DELETE", StatisticsRequest, 1, { "statistics" }, "", statisticsTreat      },
  { "*",      StatisticsRequest, 1, { "statistics" }, "", badVerbGetDeleteOnly },

  { "",       InvalidRequest,    0, {              }, "", NULL                 }
};



/* ****************************************************************************
*
* ok - 
*/
TEST(badVerbGetDeleteOnly, ok)
{
  ConnectionInfo  ci("/statistics",  "PUT", "1.1");
  std::string     expected = ""; // Bad verb gives no payload, only HTTP headers
  std::string     out;

  out = restService(&ci, rs);

  EXPECT_EQ(expected, out);
  EXPECT_EQ("Allow",       ci.httpHeader[0]);
  EXPECT_EQ("GET, DELETE", ci.httpHeaderValue[0]);
}
