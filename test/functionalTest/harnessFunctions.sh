# Copyright 2013 Telefonica Investigacion y Desarrollo, S.A.U
#
# This file is part of Orion Context Broker.
#
# Orion Context Broker is free software: you can redistribute it and/or
# modify it under the terms of the GNU Affero General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
#
# Orion Context Broker is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero
# General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with Orion Context Broker. If not, see http://www.gnu.org/licenses/.
#
# For those usages not covered by this license please contact with
# fermin at tid dot es

# ------------------------------------------------------------------------------
#
# harnessInit - 
#

if [ "$CONTEXTBROKER_TESTENV_SOURCED" != "YES" ]
then
  echo
  echo '---------------------------------------------------'
  echo "Test Environment missing - please source testEnv.sh"
  echo '---------------------------------------------------'
  echo
  exit 1
fi

export CONTEXTBROKER_HARNESS_FUNCTIONS_SOURCED="YES"

# ------------------------------------------------------------------------------
#
# harnessExit - 
#
function harnessExit()
{
  rm -f headers.out
}



# ------------------------------------------------------------------------------
#
# dbInit - 
#
function dbInit()
{
  role=$1  
  db=$1

  if [ "$role" == "CB" ]
  then
    echo 'db.dropDatabase()' | mongo ${BROKER_DATABASE_NAME} --quiet
  elif [ "$role" == "CM" ]
  then
    echo 'db.dropDatabase()' | mongo ${BROKER_DATABASE2_NAME} --quiet
  else
    echo 'db.dropDatabase()' | mongo $db --quiet
  fi
}

# ------------------------------------------------------------------------------
#
# localBrokerStart
#
function localBrokerStart()
{
  role=$1
  traceLevels=$2
  ipVersion=$3

  shift
  shift
  shift

  extraParams=$*

  IPvOption=""

  if [ "$ipVersion" == "IPV4" ]
  then
    IPvOption="-ipv4" 
  elif [ "$ipVersion" == "IPV6" ]
  then
    IPvOption="-ipv6"
  fi

  if [ "$role" == "CB" ]
  then
    port=$BROKER_PORT
    CB_START_CMD="contextBroker -vvvvv -harakiri -port ${BROKER_PORT} -pidpath ${BROKER_PID_FILE}     -db ${BROKER_DATABASE_NAME} -t $traceLevels $IPvOption $extraParams"
  elif [ "$role" == "CM" ]
  then
    mkdir -p /tmp/configManager
    port=$CM_PORT
    CB_START_CMD="contextBroker -harakiri -port ${CM_PORT}     -pidpath ${BROKER_PID2_FILE} -db ${BROKER_DATABASE2_NAME} -t $traceLevels -fwdPort ${BROKER_PORT} -logDir /tmp/configManager -ngsi9 $extraParams"
  fi

  if [ "$VALGRIND" == "" ]; then
    $CB_START_CMD
    # Wait some time so that the contextBroker is able to do its initial steps (reset database, start HTTP server, etc.)
    sleep 1
  else
    valgrind $CB_START_CMD > /tmp/valgrind.out 2>&1 &
    # Waiting for valgrind to start (sleep 10)
    sleep 10s
  fi

  # Test to see whether we have a broker running on $port. If not raise an error
  running_broker=$(ps -fe | grep contextBroker | grep $port | wc -l)
  if [ $running_broker -ne 1 ]; then
    echo "Unable to start contextBroker"
    exit 1
  fi
}



# ------------------------------------------------------------------------------
#
# localBrokerStop
#
function localBrokerStop
{
  role=$1

  if [ "$role" == "CB" ]
  then
    port=$BROKER_PORT
  else
    port=CM_PORT
  fi

  # Test to see if we have a broker running on $port if so kill it!
  running_broker=$(ps -fe | grep contextBroker | grep $port | wc -l)
  if [ $running_broker -ne 0 ]; then
    kill $(ps -fe | grep contextBroker | grep $port | awk '{print $2}') 2> /dev/null
    # Wait some time so the broker can finish properly
    sleep 1
    running_broker=$(ps -fe | grep contextBroker | grep $port | wc -l)
    if [ $running_broker -ne 0 ]; then
      # If the broker refuses to stop politely, kill the process by brute force
      kill -9 $(ps -fe | grep contextBroker | grep $port | awk '{print $2}') 2> /dev/null
      sleep 1
      running_broker=$(ps -fe | grep contextBroker | grep $port | wc -l)
      if [ $running_broker -ne 0 ]; then
        echo "Existing contextBroker is immortal, can not be killed!"
        exit 1
      fi
    fi
  fi
}



# ------------------------------------------------------------------------------
#
# brokerStart
#
function brokerStart()
{
  role=$1
  traceLevels=$2
  ipVersion=$3

  shift
  shift
  shift

  extraParams=$*

  if [ "$role" == "" ]
  then
    echo "No role given as first parameter for brokerStart"
    return
  fi

  if [ "$traceLevels" == "" ]
  then
    traceLevels=0-255
  fi

  localBrokerStop $role
  localBrokerStart $role $traceLevels $ipVersion $extraParams
}



# ------------------------------------------------------------------------------
#
# brokerStop - 
#
function brokerStop
{
  role=$1

  if [ "$role" == "CB" ]
  then
    pidFile=$BROKER_PID_FILE
    port=$BROKER_PORT
  elif [ "$role" == "CM" ]
  then
    pidFile=$BROKER_PID2_FILE
    port=$CM_PORT
  fi

  if [ "$VALGRIND" == "" ]; then
    kill $(cat $pidFile) 2> /dev/null
    if [ -f /tmp/orion_${port}.pid ]
    then
      rm -f /tmp/orion_${port}.pid 2> /dev/null
    fi
  else
    curl localhost:${port}/exit/harakiri 2> /dev/null >> ${TEST_BASENAME}.valgrind.stop.out
    # Waiting for valgrind to terminate (sleep 10)
    sleep 10
  fi
}



# ------------------------------------------------------------------------------
#
# accumulatorStop - 
#
function accumulatorStop()
{
  port=$1

  # If port is missing, we use the default LISTENER_PORT
  if [ -z "$port" ]
  then
    port=${LISTENER_PORT}
  fi

  apid=$(curl localhost:$port/pid -s -S 2> /dev/null)
  if [ "$apid" != "" ]
  then
    kill $apid 2> /dev/null
  fi

  sleep 1

  running_app=$(ps -fe | grep accumulator-server | grep $port | wc -l)

  if [ $running_app -ne 0 ]
  then
    kill $(ps -fe | grep accumulator-server | grep $port | awk '{print $2}') 2> /dev/null
    # Wait some time so the accumulator can finish properly
    sleep 1
    running_app=$(ps -fe | grep accumulator-server | grep $port | wc -l)
    if [ $running_app -ne 0 ]
    then
      # If the accumulator refuses to stop politely, kill the process by brute force
      kill -9 $(ps -fe | grep accumulator-server | grep $port | awk '{print $2}') 2> /dev/null
      sleep 1
      running_app=$(ps -fe | grep accumulator-server | grep $port | wc -l)

      if [ $running_app -ne 0 ]
      then
        echo "Existing accumulator-server.py is immortal, can not be killed!"
        exit 1
      fi
    fi
  fi
}


# ------------------------------------------------------------------------------
#
# accumulatorStart - 
#
function accumulatorStart()
{
  bindIp=$1
  port=$2

  # If port is missing, we use the default LISTENER_PORT
  if [ -z "$port" ]
  then
    port=$LISTENER_PORT
  fi

  accumulatorStop $port

  accumulator-server.py $port /notify $bindIp 2> /dev/null &
  echo accumulator running as PID $$

  # Wait until accumulator has started or we have waited a given maximum time
  port_not_ok=1
  typeset -i time
  time=0

  until [ $port_not_ok -eq 0 ]
  do
   if [ "$time" -eq "$MAXIMUM_WAIT" ]
   then
      echo "Unable to start listening application after waiting ${MAXIMUM_WAIT}"
      exit 1
   fi 
   sleep 1

   time=$time+1
   nc -z localhost $port
   port_not_ok=$?
  done
}



# ------------------------------------------------------------------------------
#
# printXmlWithHeaders - 
#
function printXmlWithHeaders()
{
  text=$1

  cat headers.out

  if [ "$text" != "" ]
  then
    echo "$text" | xmllint --format -
  fi

  rm headers.out
}



# ------------------------------------------------------------------------------
#
# printJsonWithHeaders - 
#
function printJsonWithHeaders()
{
  text=$1

  cat headers.out

  if [ "$text" != "" ]
  then
    echo "${text}" | python -mjson.tool
  fi

  rm headers.out
}



# ------------------------------------------------------------------------------
#
# curlIt- 
#
# URL: You also have to specify host, port
# 
function curlIt()
{
  _outFormat="$1"
  _url="$2"
  _payload="$3"
  _contenttype="$4"
  _accept="$5"
  _extraoptions="$6"
  _httpTenant="$7"

  _params="-s -S --dump-header headers.out"

  if [ "$_extraoptions" == "" ] && [ "$_httpTenant" != "" ]
  then
    response=$(echo ${_payload} | (curl ${_url} ${_params} --header "${_contenttype}" --header "${_accept}" --header "Expect:" --header "$_httpTenant" -d @- ))
  else
    response=$(echo ${_payload} | (curl ${_url} ${_params} --header "${_contenttype}" --header "${_accept}" --header "Expect:" ${_extraoptions} -d @- ))
  fi

  if [ "$_outFormat" == "XML" ] || [ "$_outFormat" == "xml" ]
  then
    printXmlWithHeaders "${response}"
  elif [ "$_outFormat" == "JSON" ] || [ "$_outFormat" == "json" ]
  then
    printJsonWithHeaders "${response}"
  fi
}



# ------------------------------------------------------------------------------
#
# curlXml - 
#
# Helper function.
#
function curlXml()
{
  _url="$1"
  _payload="$2"
  _extraoptions="$3"
  
  curlIt "XML" "localhost:${BROKER_PORT}${_url}" "${_payload}" "Content-Type: application/xml" "Accept: application/xml" "$_extraoptions"
}

function curlXmlTenants()
{
  _url="$1"
  _payload="$2"
  _tenant="$3"
  
  curlIt "XML" "localhost:${BROKER_PORT}${_url}" "${_payload}" "Content-Type: application/xml" "Accept: application/xml" "" "$_tenant"
}


# ------------------------------------------------------------------------------
#
# curlJson - 
#
# Helper function.
#
function curlJson()
{
  _url="$1"
  _payload="$2"
  _extraoptions="$3"
  
  curlIt "JSON" "localhost:${BROKER_PORT}${_url}" "${_payload}" "Content-Type: application/json" "Accept: application/json" "${_extraoptions}"
}



# ------------------------------------------------------------------------------
#
# curlNoPayload - 
#
function curlNoPayload()
{
  _outFormat="$1"
  _url="$2"
  _extraoptions="$3"
  _contenttype="$4"
  _accept="$5"
   
  _params="-s -S --dump-header headers.out "
  
  _response=$(curl localhost:${BROKER_PORT}${_url} ${_params} ${_extraoptions} --header "${_contenttype}" --header "${_accept}")
    
  if [ "$_outFormat" == "XML" ] || [ "$_outFormat" == "xml" ]
  then
    printXmlWithHeaders "${_response}"
  elif [ "$_outFormat" == "JSON" ] || [ "$_outFormat" == "json" ]
  then
    printJsonWithHeaders "${_response}"
  fi
}



# ------------------------------------------------------------------------------
#
# curlXmlNoPayload - 
#
# Helper function.
#
function curlXmlNoPayload()
{
  _url="$1"
  _extraoptions="$2"
  
  curlNoPayload "XML" "$_url" "$_extraoptions" "Content-Type: application/xml" "Accept: application/xml"
}



# ------------------------------------------------------------------------------
#
# curlJsonNoPayload - 
#
# Helper function.
#
function curlJsonNoPayload()
{
  _url="$1"
  _extraoptions="$2"
  
  curlNoPayload "JSON" "$_url" "$_extraoptions" "Content-Type: application/json" "Accept: application/json"
}



# ------------------------------------------------------------------------------
#
# mongoCmd - 
#
# This functions is needed due to some problems with jenkins that seems to avoid
# the usage of 'mongo --quiet ...' directly. Thus, we need to use mongo without
# --quiet, but we need to get rid of some preamble lines about mongo version and
# connection information and a final 'bye' line
#
function mongoCmd()
{

  db=$1
  cmd=$2
  echo $cmd | mongo $db | tail -n 2 | head -n 1
}

# ------------------------------------------------------------------------------
#
# dbInsertEntity -
#
# Insert a crafted entity which and "inflated" attribute. The database is pased
# as first argument and the id of the entity as second. The size of the "inflated"
# attribute is the third argument x 10.
#
function dbInsertEntity()
{
  db=$1
  entId=$2
  size=$3

  # This is a JavaScript code that generates a string variable which length is the
  # one in the argument x 10
  jsCode="s = \"\"; for (var i = 0; i < $size ; i++) { s += \"0123456789\"; }"

  ent="entity = \"$entId\""

  doc='doc = {
    "_id": {
        "id": entity,
        "type": "T"
    },
    "attrs": [
        {
            "name": "A",
            "type": "TA",
            "value": s,
            "creDate" : 1389376081,
            "modDate" : 1389376081
        },
    ],
    "creDate": 1389376081,
    "modDate": 1389376081
  }'

  cmd='db.entities.insert(doc)'

  echo "$jsCode ; $ent ; $doc ; $cmd" | mongo $db
}


# ------------------------------------------------------------------------------
#
# orionCurl
#
# Options:
#   -X            <HTTP method>    (default: according to curl. GET if no payload, POST if with payload)
#   --host        <host>           (default: localhost)
#   --port        <port>           (default: $BROKER_PORT)
#   --url         <URL>            (default: empty string)
#   --payload     <payload>        (default: NO PAYLOAD. Possible values: [filename | "${string}"])
#   --in          (input payload)  (default: xml => application/xml, If 'json': application/json)
#   --out         (output payload  (default: xml => application/xml, If 'json': application/json)
#   --json        (in/out JSON)    (if --in/out is used AFTER --json, it overrides) 
#   --httpTenant  <tenant>         (tenant in HTTP header)
#   --urlTenant   <tenant>         (tenant in URL)
#
# Any parameters are sent as is to 'curl'
# 
function orionCurl()
{
  #
  # Default values
  #
  _method=""
  _host="localhost"
  _port=$BROKER_PORT
  _url=""
  _payload=""
  _inFormat=application/xml
  _outFormat=application/xml
  _json=""
  _httpTenant=""
  _urlTenant=""
  _xtra=''

  while [ "$#" != 0 ]
  do
    if   [ "$1" == "-X" ]; then                _method="$2"; shift;
    elif [ "$1" == "--host" ]; then            _host="$2"; shift;
    elif [ "$1" == "--port" ]; then            _port="$2"; shift;
    elif [ "$1" == "--url" ]; then             _url="$2"; shift;
    elif [ "$1" == "--payload" ]; then         _payload="$2"; shift;
    elif [ "$1" == "--in" ]; then              _inFormat="$2"; shift;
    elif [ "$1" == "--out" ]; then             _outFormat="$2"; shift;
    elif [ "$1" == "--json" ]; then            _inFormat=application/json; _outFormat=application/json; shift;
    elif [ "$1" == "--httpTenant" ]; then      _httpTenant="$2"; shift;
    elif [ "$1" == "--urlTenant" ]; then       _urlTenant="$2"; shift;
    else                                       _xtra="$_xtra $1"; shift;
    fi

    shift
  done


  #
  # Sanity check of parameters
  #
  if [ "$_url" == "" ]
  then
    echo "No URL";
    return 1;
  fi


  #
  # Fix for 'Content-Type' and 'Accept' short names 'xml' and 'json'
  #
  if [ "$_inFormat" == "xml" ];   then _inFormat=application/xml;   fi;
  if [ "$_outFormat" == "xml" ];  then _outFormat=application/xml;  fi;
  if [ "$_inFormat" == "json" ];  then _inFormat=application/json;  fi;
  if [ "$_outFormat" == "json" ]; then _outFormat=application/json; fi;



  #
  # Cleanup 'compound' variables, so that we don't inherit values from previous calls
  # 
  _PAYLOAD=''
  _METHOD=''
  _URL=''
  _HTTP_TENANT=''


  #
  # Set up the compound variables
  #
  if [ "$_payload" != "" ]
  then
    if [ -f "$_payload" ]
    then
      _PAYLOAD="-d @$_payload"
    else
      _PAYLOAD="-d @-"
    fi
  fi

  if [ "$_method" != "" ];     then    _METHOD=' -X "'$_method'"';   fi
  if [ "$_httpTenant" != "" ]; then    _HTTP_TENANT='--header "Fiware-Service:'$_httpTenant'"';  fi

  if [ "$_urlTenant" != "" ]
  then
    _URL=$_host:$_port/$_tenant$_url
  else
    _URL=$_host:$_port$_url
  fi
  
  _BUILTINS='-s -S -H "Connection: Close" --dump-header /tmp/httpHeaders.out'
#   echo '==============================================================================================================================================================='
#   echo "echo $_payload | curl $_URL $_PAYLOAD $_METHOD --header \"Content-Type: $_inFormat\" --header \"Accept: $_outFormat\" $HTTP_TENANT $_BUILTINS $_xtra"
#   echo '==============================================================================================================================================================='

  _response=$(echo $_payload | curl $_URL $_PAYLOAD $_METHOD --header "Content-Type: $_inFormat" --header "Accept: $_outFormat" $HTTP_TENANT $_BUILTINS $_xtra)
  
  #
  # Remove "Connection: Keep-Alive" header and print headers out
  #
  sed '/Connection: Keep-Alive/d' /tmp/httpHeaders.out
  
  #
  # Print and beautify response body
  #
  if [ "$_outFormat" == application/xml ] || [ "$_outFormat" == "" ]
  then
    echo $_response | xmllint --format -
  elif [ "$_outFormat" == application/json ]
  then
    echo $_response | python -mjson.tool
  fi
}

export -f dbInit
export -f brokerStart
export -f localBrokerStop
export -f localBrokerStart
export -f brokerStop
export -f localBrokerStop
export -f accumulatorStart
export -f accumulatorStop
export -f curlXml
export -f curlIt
export -f curlJson
export -f orionCurl
export -f printXmlWithHeaders
export -f printJsonWithHeaders
export -f curlXmlNoPayload
export -f curlJsonNoPayload
export -f curlNoPayload
export -f curlXmlTenants
export -f dbInsertEntity
export -f mongoCmd
