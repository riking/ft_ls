#!/bin/bash

diff <($(dirname $0)/../ft_ls $@ | tee /dev/fd/2) <(ls $@)
if [ $? == 0 ]; then
	printf "\e[32mOK\n"
else
	printf "\e[31mKO\n"
fi
