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
* Author: Fermin Galan Marquez
*/
#include <string>

#include "common/globals.h"

#include "logMsg/traceLevels.h"
#include "logMsg/logMsg.h"

#include "mongoBackend/MongoGlobal.h"
#include "mongoBackend/mongoSubscribeContextAvailability.h"
#include "ngsi9/SubscribeContextAvailabilityRequest.h"
#include "ngsi9/SubscribeContextAvailabilityResponse.h"

#include "common/Format.h"
#include "common/sem.h"

/* ****************************************************************************
*
* mongoSubscribeContextAvailability - 
*/
HttpStatusCode mongoSubscribeContextAvailability(SubscribeContextAvailabilityRequest* requestP, SubscribeContextAvailabilityResponse* responseP, Format inFormat)
{
    /* Take semaphore. The LM_S* family of macros combines semaphore release with return */
    semTake();

    LM_T(LmtMongo, ("Subscribe Context Availability Request"));

    DBClientConnection* connection = getMongoConnection();

    /* If expiration is not present, then use a default one */
    if (requestP->duration.isEmpty()) {
        requestP->duration.set(DEFAULT_DURATION);
    }

    /* Calculate expiration (using the current time and the duration field in the request) */
    int expiration = getCurrentTime() + requestP->duration.parse();
    LM_T(LmtMongo, ("Subscription expiration: %d", expiration));

    /* Create the mongoDB subscription document */
    BSONObjBuilder sub;
    OID oid;
    oid.init();
    sub.append("_id", oid);
    sub.append(CASUB_EXPIRATION, expiration);
    sub.append(CASUB_REFERENCE, requestP->reference.get());

    /* Build entities array */
    BSONArrayBuilder entities;
    for (unsigned int ix = 0; ix < requestP->entityIdVector.size(); ++ix) {
        EntityId* en = requestP->entityIdVector.get(ix);
        if (en->type == "") {
            entities.append(BSON(CASUB_ENTITY_ID << en->id <<
                                 CASUB_ENTITY_ISPATTERN << en->isPattern));
        }
        else {
            entities.append(BSON(CASUB_ENTITY_ID << en->id <<
                                 CASUB_ENTITY_TYPE << en->type <<
                                 CASUB_ENTITY_ISPATTERN << en->isPattern));
        }

    }
    sub.append(CASUB_ENTITIES, entities.arr());

    /* Build attributes array */
    BSONArrayBuilder attrs;
    for (unsigned int ix = 0; ix < requestP->attributeList.size(); ++ix) {
        attrs.append(requestP->attributeList.get(ix));
    }
    sub.append(CASUB_ATTRS, attrs.arr());

    /* Adding format to use in notifications */
    sub.append(CASUB_FORMAT, std::string(formatToString(inFormat)));

    /* Insert document in database */
    BSONObj subDoc = sub.obj();
    try {
        LM_T(LmtMongo, ("insert() in '%s' collection: '%s'", getSubscribeContextAvailabilityCollectionName(), subDoc.toString().c_str()));
        connection->insert(getSubscribeContextAvailabilityCollectionName(), subDoc);
    }
    catch( const DBException &e ) {
        responseP->errorCode.fill(
            SccReceiverInternalError,
            "Database Error",
            std::string("collection: ") + getSubscribeContextAvailabilityCollectionName() +
                " - insert(): " + subDoc.toString() +
                " - exception: " + e.what()
        );

        LM_SRE(SccOk,("Database error '%s'", responseP->errorCode.reasonPhrase.c_str()));
    }

    /* Send notifications for matching context registrations */
    processAvailabilitySubscription(requestP->entityIdVector, requestP->attributeList, oid.str(), requestP->reference.get(), inFormat);

    /* Fill the response element */
    responseP->duration = requestP->duration;
    responseP->subscriptionId.set(oid.str());

    LM_SR(SccOk);
}
