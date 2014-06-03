source ../harnessFunctions.sh

echo "1: ***********************************"
url="/NGSI10/notifyContext"
payload='{
  "subscriptionId": "012345678901234567890123",
  "originator": "http://localhost/test",
  "contextResponses": [
    {
      "contextElement": {
        "type": "Room",
        "isPattern": "false",
        "id": "ConferenceRoom",
        "attributeDomainName" : "ADN",
        "attributes": [
          {
            "name": "temperature",
            "type": "Room",
            "value": "10",
            "metadatas": [
              {
                "name": "ID",
                "type": "string",
                "value": "1110"
              },
              {
                "name": "id",
                "type": "integer",
                "value": "12"
              }
            ]
          },
          {
            "name": "temperature",
            "type": "Room",
            "value": "10",
            "metadatas": [
              {
                "name": "ID",
                "type": "string",
                "value": "1110"
              },
              {
                "name": "id",
                "type": "integer",
                "value": "55"
              }
            ]
          }
        ],
        "metadatas": [
          {
            "name": "m1",
            "type": "t1",
            "value": "v1"
          },
          {
            "name": "m2",
            "type": "t2",
            "value": "v2"
          }
        ]
      },
      "statusCode": {
        "code": "200",
        "reasonPhrase": "Ok",
        "details": "a"
      }
    }
  ]
}'
curlJson ${url} "${payload}" "-X GET"

