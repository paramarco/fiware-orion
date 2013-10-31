#ifndef NGSI_CONTAINER
#define NGSI_CONTAINER

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

namespace cb
{

namespace ngsi
{

/* ****************************************************************************
*
* NgsiContainer - 
*/
typedef enum NgsiContainer
{
  NgsiNoContainer,

  Ngsi9DiscoverContextAvailabilityRequestContainer,
  Ngsi9DiscoverContextAvailabilityResponseContainer,
  Ngsi9NotifyContextAvailabilityRequestContainer,
  Ngsi9RegisterContextRequestContainer,
  Ngsi9RegisterContextResponseContainer,
  Ngsi9SubscribeContextAvailabilityRequestContainer,
  Ngsi9SubscribeContextAvailabilityResponseContainer,
  Ngsi9UnsubscribeContextAvailabilityRequestContainer,
  Ngsi9UnsubscribeContextAvailabilityResponseContainer,
  Ngsi9UpdateContextAvailabilitySubscriptionRequestContainer,
  Ngsi9UpdateContextAvailabilitySubscriptionResponseContainer,

  Ngsi10NotifyContextRequestContainer,
  Ngsi10QueryContextRequestContainer,
  Ngsi10QueryContextResponseContainer,
  Ngsi10SubscribeContextRequestContainer,
  Ngsi10SubscribeContextResponseContainer,
  Ngsi10UnsubscribeContextRequestContainer,
  Ngsi10UnsubscribeContextResponseContainer,
  Ngsi10UpdateContextRequestContainer,
  Ngsi10UpdateContextResponseContainer,
  Ngsi10UpdateContextSubscriptionRequestContainer,
  Ngsi10UpdateContextSubscriptionResponseContainer,

  NgsiAssociationContainer,
  NgsiAttributeAssociationContainer,
  NgsiAttributeAssociationListContainer,
  NgsiAttributeDomainNameContainer,
  NgsiAttributeExpressionContainer,
  NgsiAttributeListContainer,
  NgsiConditionValueListContainer,
  NgsiContextAttributeContainer,
  NgsiContextAttributeVectorContainer,
  NgsiContextElementContainer,
  NgsiContextElementResponseContainer,
  NgsiContextElementResponseVectorContainer,
  NgsiContextElementVectorContainer,
  NgsiContextRegistrationContainer,
  NgsiContextRegistrationAttributeContainer,
  NgsiContextRegistrationAttributeVectorContainer,
  NgsiContextRegistrationResponseContainer,
  NgsiContextRegistrationResponseVectorContainer,
  NgsiContextRegistrationVectorContainer,
  NgsiDurationContainer,

  NgsiEntityAssociationContainer,
  NgsiEntityIdContainer,
  NgsiEntityIdVectorContainer,
  NgsiErrorCodeContainer,
  NgsiMetadataContainer,
  NgsiMetadataVectorContainer,
  NgsiNotifyConditionContainer,
  NgsiNotifyConditionVectorContainer,
  NgsiOriginatorContainer,
  NgsiProvidingApplicationContainer,
  NgsiReferenceContainer,
  NgsiRegistrationIdContainer,
  NgsiRestrictionContainer,
  NgsiRestrictionStringContainer,
  NgsiScopeContainer,
  NgsiScopeVectorContainer,
  NgsiStatusCodeContainer,
  NgsiSubscribeErrorContainer,
  NgsiSubscribeResponseContainer,
  NgsiSubscriptionIdContainer,
  NgsiThrottlingContainer,
  NgsiUpdateActionTypeContainer
} NgsiContainer;

}
}

#endif
