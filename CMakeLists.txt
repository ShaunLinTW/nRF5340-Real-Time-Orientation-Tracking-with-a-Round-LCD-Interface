#
# Origanization: Rice University & HealthSeers Inc.
# Project: Cairdio Project
# Author: Shaun Lin (hl116@rice.edu)
#

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(nrf53_bmi270+gc9a01)

FILE(GLOB app_sources src/*.c ui/*.c)
target_sources(app PRIVATE ${app_sources})

# NORDIC SDK APP END
zephyr_library_include_directories(.)
