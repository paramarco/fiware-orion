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
#include "testInit.h"

#include "logMsg/logMsg.h"
#include "logMsg/traceLevels.h"

#include "common/globals.h"
#include "mongoBackend/MongoGlobal.h"
#include "mongoBackend/mongoUnsubscribeContext.h"
#include "ngsi10/UnsubscribeContextRequest.h"
#include "ngsi10/UnsubscribeContextResponse.h"

#include "mongo/client/dbclient.h"

#include "commonMocks.h"

using ::testing::_;
using ::testing::Throw;
using ::testing::Return;

/* ****************************************************************************
*
* First set of test is related with updating thinks
*
* - subscriptionNotFound
* - unsubscribe
*
* Simulating fails in MongoDB connection.
*
* - MongoDbFindOneFail
* - MongoDbRemoveFail
*
*/

#define NO_CODE 0


/* ****************************************************************************
*
* prepareDatabase -
*/
static void prepareDatabase(void) {

    /* Set database */
    setupDatabase();

    DBClientConnection* connection = getMongoConnection();

    BSONObj sub1 = BSON("_id" << OID("51307b66f481db11bf860001") <<
                        "expiration" << 10000000 <<
                        "lastNotification" << 15000000 <<
                        "reference" << "http://notify1.me" <<
                        "entities" << BSON_ARRAY(BSON("id" << "E1" << "type" << "T1" << "isPattern" << "false")) <<
                        "attrs" << BSONArray() <<
                        "conditions" << BSON_ARRAY(BSON(
                                                       "type" << "ONCHANGE" <<
                                                       "value" << BSON_ARRAY("AX1" << "AY1")
                                                       ) <<
                                                   BSON(
                                                       "type" << "ONTIMEINTERVAL" <<
                                                       "value" << 100
                                                       ))
                        );

    BSONObj sub2 = BSON("_id" << OID("51307b66f481db11bf860002") <<
                        "expiration" << 20000000 <<
                        "lastNotification" << 25000000 <<
                        "reference" << "http://notify2.me" <<
                        "entities" << BSON_ARRAY(BSON("id" << "E1" << "type" << "T1" << "isPattern" << "false")) <<
                        "attrs" << BSON_ARRAY("A1" << "A2") <<
                        "conditions" << BSON_ARRAY(BSON(
                                                       "type" << "ONCHANGE" <<
                                                       "value" << BSON_ARRAY("AX2" << "AY2")
                                                       ) <<
                                                   BSON(
                                                       "type" << "ONTIMEINTERVAL" <<
                                                       "value" << 200
                                                       ))
                        );

    connection->insert(SUBSCRIBECONTEXT_COLL, sub1);
    connection->insert(SUBSCRIBECONTEXT_COLL, sub2);

}

/* ****************************************************************************
*
* subscriptionNotFound -
*/
TEST(mongoUnsubscribeContext, subscriptionNotFound)
{
    HttpStatusCode             ms;
    UnsubscribeContextRequest  req;
    UnsubscribeContextResponse res;

    /* Prepare mock */
    NotifierMock* notifierMock = new NotifierMock();
    EXPECT_CALL(*notifierMock, destroyOntimeIntervalThreads(_))
            .Times(0);
    EXPECT_CALL(*notifierMock, sendNotifyContextRequest(_,_,_))
            .Times(0);
    EXPECT_CALL(*notifierMock, createIntervalThread(_,_,_))
            .Times(0);
    setNotifier(notifierMock);

    /* Forge the request (from "inside" to "outside") */
    req.subscriptionId.set("51307b66f481db11bf869999");

    /* Prepare database */
    prepareDatabase();

    /* Invoke the function in mongoBackend library */
    ms = mongoUnsubscribeContext(&req, &res);

    /* Check response is as expected */
    EXPECT_EQ(SccOk, ms);
    EXPECT_EQ("51307b66f481db11bf869999", res.subscriptionId.get());
    EXPECT_EQ(SccContextElementNotFound, res.statusCode.code);
    EXPECT_EQ("No context element found", res.statusCode.reasonPhrase);
    EXPECT_EQ("subscriptionId: '51307b66f481db11bf869999'", res.statusCode.details);

    /* Check database (untouched) */
    DBClientConnection* connection = getMongoConnection();
    ASSERT_EQ(2, connection->count(SUBSCRIBECONTEXT_COLL, BSONObj()));

    /* Release connection */
    mongoDisconnect();

    /* Release mock */
    delete notifierMock;
}

/* ****************************************************************************
*
* unsubscribe -
*/
TEST(mongoUnsubscribeContext, unsubscribe)
{
    HttpStatusCode             ms;
    UnsubscribeContextRequest  req;
    UnsubscribeContextResponse res;

    /* Prepare mock */
    NotifierMock* notifierMock = new NotifierMock();
    EXPECT_CALL(*notifierMock, destroyOntimeIntervalThreads("51307b66f481db11bf860001"))
            .Times(1);
    EXPECT_CALL(*notifierMock, sendNotifyContextRequest(_,_,_))
            .Times(0);
    EXPECT_CALL(*notifierMock, createIntervalThread(_,_,_))
            .Times(0);
    setNotifier(notifierMock);

    /* Forge the request (from "inside" to "outside") */
    req.subscriptionId.set("51307b66f481db11bf860001");

    /* Prepare database */
    prepareDatabase();

    /* Invoke the function in mongoBackend library */
    ms = mongoUnsubscribeContext(&req, &res);

    /* Check response is as expected */
    EXPECT_EQ(SccOk, ms);
    EXPECT_EQ("51307b66f481db11bf860001", res.subscriptionId.get());
    EXPECT_EQ(SccOk, res.statusCode.code);
    EXPECT_EQ("OK", res.statusCode.reasonPhrase);
    EXPECT_EQ(0, res.statusCode.details.size());

    /* Check database (one document, but not the deleted one) */
    DBClientConnection* connection = getMongoConnection();
    ASSERT_EQ(1, connection->count(SUBSCRIBECONTEXT_COLL, BSONObj()));
    BSONObj sub = connection->findOne(SUBSCRIBECONTEXT_COLL, BSON("_id" << OID("51307b66f481db11bf860002")));
    EXPECT_EQ("51307b66f481db11bf860002", sub.getField("_id").OID().str());

    /* Release connection */
    mongoDisconnect();

    /* Release mock */
    delete notifierMock;
}


/* ****************************************************************************
*
* MongoDbFindOneFail -
*
*/
TEST(mongoUnsubscribeContext, MongoDbFindOneFail)
{
    HttpStatusCode             ms;
    UnsubscribeContextRequest  req;
    UnsubscribeContextResponse res;

    /* Prepare mocks */
    const DBException e = DBException("boom!!", 33);
    DBClientConnectionMock* connectionMock = new DBClientConnectionMock();
    ON_CALL(*connectionMock, findOne("unittest.csubs",_,_,_))
            .WillByDefault(Throw(e));

    NotifierMock* notifierMock = new NotifierMock();
    EXPECT_CALL(*notifierMock, destroyOntimeIntervalThreads(_))
            .Times(0);
    EXPECT_CALL(*notifierMock, sendNotifyContextRequest(_,_,_))
            .Times(0);
    EXPECT_CALL(*notifierMock, createIntervalThread(_,_,_))
            .Times(0);
    setNotifier(notifierMock);

    /* Forge the request (from "inside" to "outside") */
    req.subscriptionId.set("51307b66f481db11bf860001");

    /* Prepare database */
    prepareDatabase();
    mongoConnect(connectionMock);

    /* Invoke the function in mongoBackend library */
    ms = mongoUnsubscribeContext(&req, &res);

    /* Check response is as expected */
    EXPECT_EQ(SccOk, ms);
    EXPECT_EQ("51307b66f481db11bf860001", res.subscriptionId.get());
    EXPECT_EQ(SccReceiverInternalError, res.statusCode.code);
    EXPECT_EQ("Internal Server Error", res.statusCode.reasonPhrase);
    EXPECT_EQ("collection: unittest.csubs "
              "- findOne() _id: 51307b66f481db11bf860001 "
              "- exception: boom!!", res.statusCode.details);

    /* Check database (untouched) */
    mongoDisconnect();

    // Sleeping a little to "give mongod time to process its input".
    // Without this sleep, this tests fails around 10% of the times (in Ubuntu 13.04)
    usleep(1000);
    mongoConnect("localhost");
    DBClientConnection*  connection = getMongoConnection();
    int                  count      = connection->count(SUBSCRIBECONTEXT_COLL, BSONObj());

    ASSERT_EQ(2, count);

    /* Release mocks */
    delete notifierMock;

}

/* ****************************************************************************
*
* MongoDbRemoveFail -
*
*/
TEST(mongoUnsubscribeContext, MongoDbRemoveFail)
{
    HttpStatusCode             ms;
    UnsubscribeContextRequest  req;
    UnsubscribeContextResponse res;

    /* Prepare mocks */
    const DBException e = DBException("boom!!", 33);
    BSONObj fakeSub = BSON("_id" << OID("51307b66f481db11bf860001") <<
                                       "expiration" << 10000000 <<
                                       "lastNotification" << 15000000 <<
                                       "reference" << "http://notify1.me" <<
                                       "entities" << BSON_ARRAY(BSON("id" << "E1" << "type" << "T1" << "isPattern" << "false")) <<
                                       "attrs" << BSONArray() <<
                                       "conditions" << BSONArray());
    DBClientConnectionMock* connectionMock = new DBClientConnectionMock();
    ON_CALL(*connectionMock, findOne("unittest.csubs",_,_,_))
            .WillByDefault(Return(fakeSub));
    ON_CALL(*connectionMock, remove("unittest.csubs",_,_))
            .WillByDefault(Throw(e));

    NotifierMock* notifierMock = new NotifierMock();
    EXPECT_CALL(*notifierMock, destroyOntimeIntervalThreads(_))
            .Times(0);
    EXPECT_CALL(*notifierMock, sendNotifyContextRequest(_,_,_))
            .Times(0);
    EXPECT_CALL(*notifierMock, createIntervalThread(_,_,_))
            .Times(0);
    setNotifier(notifierMock);

    /* Forge the request (from "inside" to "outside") */
    req.subscriptionId.set("51307b66f481db11bf860001");

    /* Prepare database */
    prepareDatabase();
    mongoConnect(connectionMock);

    /* Invoke the function in mongoBackend library */
    ms = mongoUnsubscribeContext(&req, &res);

    /* Check response is as expected */
    EXPECT_EQ(SccOk, ms);
    EXPECT_EQ("51307b66f481db11bf860001", res.subscriptionId.get());
    EXPECT_EQ(SccReceiverInternalError, res.statusCode.code);
    EXPECT_EQ("Internal Server Error", res.statusCode.reasonPhrase);
    EXPECT_EQ("collection: unittest.csubs "
              "- remove() _id: 51307b66f481db11bf860001 "
              "- exception: boom!!", res.statusCode.details);

    /* Check database (untouched) */
    mongoDisconnect();

    // Sleeping a little to "give mongod time to process its input".
    usleep(1000);
    mongoConnect("localhost");
    DBClientConnection* connection = getMongoConnection();
    ASSERT_EQ(2, connection->count(SUBSCRIBECONTEXT_COLL, BSONObj()));

    /* Release mocks */
    delete notifierMock;

}

