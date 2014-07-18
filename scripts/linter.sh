#!/bin/bash
# -*- coding: latin-1 -*-
# Copyright 2014 Telefonica Investigacion y Desarrollo, S.A.U
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
#
# Author: Ken Zangelin



# -----------------------------------------------------------------------------
#
# vars
#
ME=$(basename $0)



# -----------------------------------------------------------------------------
#
# usage
#
function usage()
{
  sfile="Usage: "$(basename $0)
  empty=$(echo $sfile | tr 'a-zA-z/0-9.:' ' ')

  echo "$sfile [-u (usage)]"
  echo "$empty [-v (verbose)]"
  echo "$empty [-d <directory>]"
  echo "$empty [-f <file>]"

  exit $1
}



# -----------------------------------------------------------------------------
#
# vMsg - 
#
function vMsg()
{
  if [ "$verbose" = "on" ]
  then
    echo $ME: $*
  fi
}



# -----------------------------------------------------------------------------
#
# mMsg - 
#
function mMsg()
{
  echo $ME: $*
}



# ------------------------------------------------------------------------------
#
# Argument parsing
#
verbose=off
dirList=""
fileList=""

vMsg "parsing options"
while [ "$#" != 0 ]
do
  if   [ "$1" == "-u" ];            then usage 0;
  elif [ "$1" == "-v" ];            then verbose=on;
  else
  {
    echo $0: bad parameter/option: "'"${1}"'";
    echo
    usage 1
  }
  fi

  shift
done



#
# Environment vars
#
if [ "$LINTER_VERBOSE" == "1" ]
then
  verbose=on
fi



vMsg "Running lint"
scripts/cpplint.py src/app/contextBroker/*.cpp src/app/contextBroker/*.h src/lib/*/*.cpp src/lib/*/*.h 2> LINT

errors=$(grep "Total errors found" LINT | awk -F:\  '{ print $2 }')
lines=$(wc -l src/app/contextBroker/*.cpp src/app/contextBroker/*.h src/lib/*/*.cpp src/lib/*/*.h | grep -v src/ | awk '{ print $1 }')

typeset -i files
filesCpp=$(find src -name "*.cpp" | wc -l)
filesH=$(find src -name "*.h" | wc -l)
files=$filesCpp+$filesH
percentage=$(echo "scale=2; $errors*100/$lines" | bc)
echo $errors errors in $lines lines of source code in $files files \($percentage%\ style-guide-incompatibilities\)



#
# If not verbose, then we have finished
#
if [ "$verbose" = "off" ]
then
  exit 0
fi


echo
echo



#
# lint error categories
#
#
declare -A cat

cat[0]='already included at'
cat[1]='Found C system header after C++ system header'
cat[2]='Lines should be <= 120 characters long'
cat[3]='Lines should very rarely be longer than 150 characters'
cat[4]='Line ends in whitespace'
cat[5]='Never use sprintf'
cat[6]='Use int16/int64/etc'
cat[7]='#ifndef header guard has wrong style'
cat[8]='#endif line should be'
cat[9]='Weird number of spaces at line-start'
cat[10]='Blank line at the end of a code block'
cat[11]='Include the directory when naming'
cat[12]='Do not use namespace using-directives'
cat[13]='Is this a non-const reference'
cat[14]='At least two spaces is best between code and comments'
cat[15]='Labels should always be indented at least one space'
cat[16]='Tab found; better to use spaces'
cat[17]='Add #include'
cat[18]='Blank line at the start of a code block'
cat[19]='More than one command on the same line'
cat[20]="You don't need a ; after a }"
cat[21]='Single-argument constructors should be marked explicit'
cat[22]='Streams are highly discouraged'
cat[23]='Do not leave a blank line after'
cat[24]='Else clause should never be on same line as else'
cat[25]='Do not leave a blank line after'
cat[26]='Almost always, snprintf is better than strcpy'
cat[27]='Missing space after ,'
cat[28]='Extra space before ( in function call'
cat[29]='All parameters should be named in a function'
cat[30]='No copyright message found'
cat[31]='Found C system header after other header'
cat[32]='Should have a space between // and comment'
cat[33]='If an else has a brace on one side, it should have it on both'
cat[34]='Almost always, snprintf is better than strcat'
cat[35]='Using sizeof(type).  Use sizeof(varname) instead if possible'
cat[36]='Missing space before {'
cat[37]='Found C++ system header after other header'
cat[38]='Consider using getpwuid_r(...) instead of getpwuid'
cat[39]='Line contains only semicolon'
cat[40]='sscanf can be ok, but is slow and can overflow buffers'
cat[41]='Missing space before ('
cat[42]='Extra space after ( in function call'
cat[43]='If you can, use sizeof'
cat[44]='Missing spaces around ='
cat[45]='Mismatching spaces inside ()'
cat[46]='Closing ) should be moved to the previous line'
cat[47]='Extra space before )'
cat[48]="LAST"


# -----------------------------------------------------------------------------
#
# Counting occurrences of the line error categories
#
typeset -i ix
ix=0

while [ "${cat[$ix]}" != "LAST" ]
do
  errors=$(grep "${cat[$ix]}" LINT | wc -l)
  if [ "$errors" != "" ] && [ "$errors" != 0 ]
  then
    echo $errors errors of category \'${cat[$ix]}\'
  fi

  ix=$ix+1
done



echo
echo
echo

# -----------------------------------------------------------------------------
#
# Now let's see if we have any new lint errors
#
echo "New lint errors:"
echo "-----------------------------------------------------"
grep -v "already included at" LINT | 
grep -v "Found C system header after C++ system header" |
grep -v "Lines should be <= 120 characters long" |
grep -v "Lines should very rarely be longer than 150 characters" |
grep -v "Line ends in whitespace" |
grep -v "Never use sprintf" |
grep -v "Use int16/int64/etc" |
grep -v "#ifndef header guard has wrong style" |
grep -v "#endif line should be" |
grep -v "Weird number of spaces at line-start" |
grep -v "Blank line at the end of a code block" |
grep -v "Done processing" |
grep -v "Include the directory when naming" |
grep -v "Do not use namespace using-directives" |
grep -v "Is this a non-const reference" |
grep -v "At least two spaces is best between code and comments"  |
grep -v "Labels should always be indented at least one space" |
grep -v "Tab found; better to use spaces" |
grep -v "Add #include"  |
grep -v "Blank line at the start of a code block"  |
grep -v "More than one command on the same line" |
grep -v "You don't need a ; after a }" |
grep -v "Single-argument constructors should be marked explicit" |
grep -v "Streams are highly discouraged" |
grep -v "Else clause should never be on same line as else" |
grep -v "Do not leave a blank line after" |
grep -v "Almost always, snprintf is better than strcpy" |
grep -v "Missing space after ," |
grep -v "Extra space before ( in function call"  |
grep -v "All parameters should be named in a function" |
grep -v "No copyright message found" |
grep -v "Found C system header after other header." |
grep -v "Should have a space between // and comment" |
grep -v "If an else has a brace on one side, it should have it on both" |
grep -v "Almost always, snprintf is better than strcat" |
grep -v "Using sizeof(type).  Use sizeof(varname) instead if possible" |
grep -v "Missing space before {" |
grep -v "Found C++ system header after other header" |
grep -v "Consider using getpwuid_r(...) instead of getpwuid" |
grep -v "Line contains only semicolon" |
grep -v "sscanf can be ok, but is slow and can overflow buffers" |
grep -v "Missing space before (" |
grep -v "Extra space after ( in function call" |
grep -v "If you can, use sizeof" |
grep -v "Missing spaces around =" |
grep -v "Mismatching spaces inside ()" |
grep -v 'Closing ) should be moved to the previous line' |
grep -v 'Extra space before )' |
grep -v 'Total errors found'

echo "-----------------------------------------------------"
