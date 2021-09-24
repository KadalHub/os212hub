#!/bin/bash

# Create users from ssh keys
# Usage:
#   bash genuser.sh file1 file2 file3
#
# `file` contains list of usernames with their respective public key:
#   cbkadal ssh-rsa AAAAB3n+w== cbkadal@pamulang1 # have public key
#   dummy                                         # doesn't have public key
# 
# Author Dennis A. Walangadi (dennis.al@ui.ac.id)
# History:
# - September 5th 2021 : Initial implementation
# - September 12th 2021: Remove custom skeleton and create user in one pass 

[[ $EUID > 0 ]] && {
   echo "This script must be run as root" 1>&2
   exit 1
}

for FILE in "$@"; do
    [ -f "$FILE" ] && {
        while read l; do
            uname=$(echo "$l" | cut -d ' ' -f1)
            pbkey=$(echo "$l" | cut -d ' ' -f2-)
            if [[ "$pbkey" = "$uname" ]]; then
                # Delete user if their doesn't have any public key
                echo "deleting $uname"
                userdel -r $uname
            else
                # Create user and add their public key to authorized keys
                echo "creating $uname"
                useradd -k /etc/skel -m -s /bin/bash $uname
                mkdir /home/$uname/.ssh/
                echo "$pbkey" >> /home/$uname/.ssh/authorized_keys
            fi
        done < "$FILE"
    }
done

exit 0
