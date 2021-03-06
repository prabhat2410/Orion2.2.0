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
* iot_support at tid dot es
*
* Author: Ken Zangelin
*/
#include <stdio.h>
#include <string>
#include <vector>

#include "logMsg/logMsg.h"
#include "logMsg/traceLevels.h"

#include "common/globals.h"
#include "common/tag.h"
#include "alarmMgr/alarmMgr.h"

#include "convenience/RegisterProviderRequest.h"
#include "ngsi/StatusCode.h"
#include "ngsi/MetadataVector.h"
#include "ngsi/Duration.h"
#include "ngsi/ProvidingApplication.h"
#include "ngsi/RegistrationId.h"
#include "ngsi9/DiscoverContextAvailabilityResponse.h"



/* ****************************************************************************
*
* Constructor - 
*/
RegisterProviderRequest::RegisterProviderRequest()
{
}



/* ****************************************************************************
*
* RegisterProviderRequest::toJsonV1 -
*/
std::string RegisterProviderRequest::toJsonV1(void)
{
  std::string  out                            = "";
  bool         providingApplicationRendered   = providingApplication.get() != "";
  bool         registrationIdRendered         = registrationId.get() != "";
  bool         commaAfterProvidingApplication = registrationIdRendered;
  bool         commaAfterDuration             = commaAfterProvidingApplication || providingApplicationRendered;

  out += startTag();
  out += duration.toJsonV1(commaAfterDuration);
  out += providingApplication.toJsonV1(commaAfterProvidingApplication);
  out += registrationId.toJsonV1(RegisterContext, false);
  out += endTag(false);

  return out;
}



/* ****************************************************************************
*
* RegisterProviderRequest::check - 
*/
std::string RegisterProviderRequest::check
(
  ApiVersion          apiVersion,
  RequestType         requestType,
  const std::string&  predetectedError
)
{
  DiscoverContextAvailabilityResponse  response;
  std::string                          res;

  if (predetectedError != "")
  {
    response.errorCode.fill(SccBadRequest, predetectedError);
  }
  else if (((res = duration.check())                 != "OK") ||
           ((res = providingApplication.check())     != "OK") ||
           ((res = registrationId.check())           != "OK"))
  {
    response.errorCode.fill(SccBadRequest, res);
  }
  else
  {
    return "OK";
  }

  std::string details = std::string("RegisterProviderRequest Error: '") + res + "'";
  alarmMgr.badInput(clientIp, details);

  return response.toJsonV1();
}
