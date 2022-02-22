#!/bin/bash

# Set backup path
backup_path="/mnt/usb1"

# Create output filename
DATE=$(date +-%Y-%m-%d-%H-%M)
output_filename="project-backup$DATE"

# Archive, zip and move project folder to backup path
cd /home/pi
tar -zcf $output_filename.tgz docker
mv $output_filename.tgz $backup_path

# Delete files older than 7 days (-mmin for minutes)
find $backup_path -type f -mtime +7 -name '*.tgz' -execdir rm -- '{}' \;

