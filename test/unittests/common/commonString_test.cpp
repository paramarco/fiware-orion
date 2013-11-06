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
* Author: Fermin Galan
*/
#include "gtest/gtest.h"

#include "logMsg/logMsg.h"
#include "logMsg/traceLevels.h"
#include "common/string.h"


/* ****************************************************************************
*
* stringSplit - 
*/
TEST(commonString, stringSplit)
{
   std::string               s1 = "";
   std::string               s2 = ":::::1:2::4";
   int                       components;
   std::vector<std::string>  out;

   components = stringSplit(s1, ':', out);
   EXPECT_EQ(0, components);
   EXPECT_EQ(0, out.size());
   
   components = stringSplit(s2, ':', out);
   EXPECT_EQ(4, components);
   EXPECT_EQ(4, out.size());
}



/* ****************************************************************************
*
* parseUrl - 
*/
TEST(commonString, parseUrl)
{
   bool         r;
   std::string  url1 = "";
   std::string  url2 = "http:/Bad";
   std::string  url3 = "http:/XXX";
   std::string  url4 = "http://";
   std::string  url5 = "http://XXX:12:34";
   std::string  url6 = "http://XXX:1234/path";
   std::string  url7 = "http://XXX/path";
   std::string  host;
   int          port;
   std::string  path;

   r = parseUrl(url1, host, port, path);
   EXPECT_FALSE(r);
   r = parseUrl(url2, host, port, path);
   EXPECT_FALSE(r);
   r = parseUrl(url3, host, port, path);
   EXPECT_FALSE(r);
   r = parseUrl(url4, host, port, path);
   EXPECT_FALSE(r);
   r = parseUrl(url5, host, port, path);
   EXPECT_FALSE(r);

   r = parseUrl(url6, host, port, path);
   EXPECT_TRUE(r);
   EXPECT_STREQ("XXX", host.c_str());
   EXPECT_EQ(1234, port);
   EXPECT_STREQ("/path", path.c_str());

   r = parseUrl(url7, host, port, path);
   EXPECT_TRUE(r);
   EXPECT_STREQ("XXX", host.c_str());
   EXPECT_EQ(80, port); // DEFAULT_HTTP_PORT == 80
   EXPECT_STREQ("/path", path.c_str());
}



/* ****************************************************************************
*
* parseFullUrl -
*/
TEST(string, parseFullUrl)
{
    std::string url = "http://host:8080/my/path";

    std::string host;
    int port;
    std::string path;

    bool result = parseUrl(url, host, port, path);

    EXPECT_TRUE(result) << "wrong result (shall be true)";
    EXPECT_EQ("host", host) << "wrong host";
    EXPECT_EQ(8080, port) << "wrong port";
    EXPECT_EQ("/my/path", path) << "wrong path";

}

/* ****************************************************************************
*
* parseUrlShortPath -
*/
TEST(string, parseUrlShortPath)
{
    std::string url = "http://host:8080/my";

    std::string host;
    int port;
    std::string path;

    bool result = parseUrl(url, host, port, path);

    EXPECT_TRUE(result) << "wrong result (shall be true)";
    EXPECT_EQ("host", host) << "wrong host";
    EXPECT_EQ(8080, port) << "wrong port";
    EXPECT_EQ("/my", path) << "wrong path";
}

/* ****************************************************************************
*
* parseUrlWithSlashPath -
*/
TEST(string, parseUrlWithSlashPath)
{
    std::string url = "http://host:8080/";

    std::string host;
    int port;
    std::string path;

    bool result = parseUrl(url, host, port, path);

    EXPECT_TRUE(result) << "wrong result (shall be true)";
    EXPECT_EQ("host", host) << "wrong host";
    EXPECT_EQ(8080, port) << "wrong port";
    EXPECT_EQ("/", path) << "wrong path";
}


/* ****************************************************************************
*
* parseUrlWithout -
*/
TEST(string, parseUrlWithoutPath)
{
    std::string url = "http://host:8080";

    std::string host;
    int port;
    std::string path;

    bool result = parseUrl(url, host, port, path);

    EXPECT_TRUE(result) << "wrong result (shall be true)";
    EXPECT_EQ("host", host) << "wrong host";
    EXPECT_EQ(8080, port) << "wrong port";
    EXPECT_EQ("/", path) << "wrong path";
}

/* ****************************************************************************
*
* parseUrlWithoutPort -
*/
TEST(string, parseUrlWithoutPort)
{
    std::string url = "http://host/my/path";

    std::string host;
    int port;
    std::string path;

    bool result = parseUrl(url, host, port, path);

    EXPECT_TRUE(result) << "wrong result (shall be true)";
    EXPECT_EQ("host", host) << "wrong host";
    EXPECT_EQ(80, port) << "wrong port";
    EXPECT_EQ("/my/path", path) << "wrong path";
}

/* ****************************************************************************
*
* parseUrlWithoutPortAndPath -
*/
TEST(string, parseUrlWithoutPortAndPath)
{
    std::string url = "http://host";

    std::string host;
    int port;
    std::string path;

    bool result = parseUrl(url, host, port, path);

    EXPECT_TRUE(result) << "wrong result (shall be true)";
    EXPECT_EQ("host", host) << "wrong host";
    EXPECT_EQ(80, port) << "wrong port";
    EXPECT_EQ("/", path) << "wrong path";
}

/* ****************************************************************************
*
* parseMalformedUrl1 -
*/
TEST(string, parseMalformedUrl1)
{
    std::string url = "http://";

    std::string host;
    int port;
    std::string path;

    bool result = parseUrl(url, host, port, path);

    EXPECT_FALSE(result) << "wrong result (shall be false)";

}

/* ****************************************************************************
*
* parseMalformedUrl2 -
*/
TEST(string, parseMalformedUrl2)
{
    std::string url = "http://20:host:8080/my/path";

    std::string host;
    int port;
    std::string path;

    bool result = parseUrl(url, host, port, path);

    EXPECT_FALSE(result) << "wrong result (shall be false)";
}

/* ****************************************************************************
*
* parseEmptyUrl -
*/
TEST(string, parseEmptyUrl)
{
    std::string url = "";

    std::string host;
    int port;
    std::string path;

    bool result = parseUrl(url, host, port, path);

    EXPECT_FALSE(result) << "wrong result (shall be false)";
}

/* ****************************************************************************
*
* parsedUptime -
*/
TEST(string, parsedUptime)
{
  std::string uptime;

  // 3 days, 4 hours, 5 min and 6 seconds
  uptime = parsedUptime(3 * (24 * 3600) + 4 * 3600 + 5 * 60 + 6);
  EXPECT_EQ(uptime, "3 d, 4 h, 5 m, 6 s");
}
