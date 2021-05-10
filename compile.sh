#!/bin/bash

RED='\e[1;31m'
WHITE='\e[0m'
CYAN='\e[1;36m'
GREEN='\e[1;32m'

gcc -o cache ./src/* 2> /dev/null

if [ $? -eq 1 ]
    then
    echo -e "\n${RED}Error:${WHITE} Could not compile the file, for more info try running << ${CYAN}gcc -o cache ./src/*${WHITE} >>.\n"
    exit 1
else
    echo -e "\n${GREEN}Success! ${WHITE}The code compiled.\n"
    exit 0
fi
