#! /bin/bash

# Read files for genuser.sh
#
# This script read from regular text files and google form generated csv files
# and echo the result in "<username> <ssh key>" format.
# This script is intended to be used alongside genuser.ssh
#
# Usage:
#     bash reader.sh [files...]
#  
# Example:
#     bash reader.sh input1 test.csv input2
#
# Author Dennis A. Walangadi (dennis.al@ui.ac.id)
# History:
# - September 24th 2021: Initial implementation

for FILE in "$@"; do
    [ -f "$FILE" ] && {
        while IFS="," read -r uname pbkey
        do
            # Strip quotes in both username and sshkey
            uname=`echo "$uname" | cut -d'"' -f 2`
            pbkey=`echo "$pbkey" | cut -d'"' -f 2`
            [[ -z "$uname" ]] && continue
	    echo "$uname $pbkey"
        done < <(
	    # Split on commas and use 3rd and 4th column
            cut -d "," -f3,4 "$FILE" | 

            # Cut CSV headers
            if [[ $FILE == *.csv ]]; then 
                tail -n +2 
            else 
                cat 
            fi
        )
    }
done

exit 0

