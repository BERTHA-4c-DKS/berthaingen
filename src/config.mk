##################################################
#
# config.mk for berthaingen
#
# $Id$
#
###################################################

PLATFORM = $(shell uname -s)

# DEBUG: 
# yes, 
# no
DEBUG=no

CC = gcc
CXX = g++

ifeq ($(DEBUG),no)
	CFLAGS = -Wall -W -O2
else
	CFLAGS = -Wall -W -O0 -g 
endif

