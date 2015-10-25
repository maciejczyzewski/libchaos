#!/bin/bash

# Platform
OS=$(uname -mnprs)

# Path
PL=$(pwd)

###############################################################################
# VARIABLES
###############################################################################

# Colors
LOG_OK=$(echo -e "\033[1;32m[+]\033[0m"); LOG_ERR=$(echo -e "\033[1;31m[!]\033[0m")
LOG_WRN=$(echo -e "\033[1;33m[~]\033[0m"); LOG_INF=$(echo -e "[.]")

# Background
LOG_BF=$(echo -e "\033[45m==="); LOG_BL=$(echo -e "===\033[0m")

###############################################################################
# PRELUDE
###############################################################################

requirements () {
  for n in python pip git
  do
    command -v $n >/dev/null 2>&1 || {
      echo $LOG_ERR "The project require \`${n}\` but it's not installed! Aborting..." >&2
      exit 1
    }
  done

  echo $LOG_OK "All requirements and packages are installed and up to date..."
}

maintainers () {
  git shortlog -sne --all > MAINTAINERS
  echo $LOG_OK "List of project maintainers successfully generated..."
}

###############################################################################
# COMMANDS
###############################################################################

build () {
  echo $LOG_BF "BUILD" $LOG_BL
  pip install -r tools/requirements.txt
  python tools/build.py $PL
}

###############################################################################
# UTILITIES
###############################################################################

lint () {
  echo $LOG_BF "LINT" $LOG_BL
  flake8 tools/
}

###############################################################################
# MAIN
###############################################################################

echo -e "Path:     ${PL}"
echo -e "Platform: ${OS}"

echo $LOG_BF "PREPARATION" $LOG_BL

requirements
maintainers

$@
