#!/bin/bash
# Prints Raspberry's GPU temperature every 2 seconds
while : 
do
   echo $(vcgencmd measure_temp)
   sleep 2 
done