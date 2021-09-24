#!/bin/bash
# (c) 2021-2021 This is a free script

# REV06 dnswd Fri 24 Sep 2021 23:31:50 WIB
#       Split read files functionality into different script
# REV05 rms46 Fri 24 Sep 2021 21:59:43 WIB
#       Fix empty line bug
#       Add NONO list
# REV04 rms46 Fri 24 Sep 2021 18:42:40 WIB
#       Add NONO list
# REV03 rms46 Fri 24 Sep 2021 14:22:34 WIB
# REV02 dnswd September 12th 2021
#       Remove custom skeleton and create user in one pass 
# REV01 dnswd September 5th 2021
#       Initial implementation
# START

# Create users from ssh keys
# This script will create a user account with with key-only authentication if 
# an ssh key is provided. Otherwise this script will search or remove an 
# account with username.
#
# Usage:
#   bash genuser.sh username [sshkey]
#
# Examples:
#   1. Add user cbkadal with ssh-key
#
#      genuser.sh cbkadal ssh-rsa AAAAB3n+w== cbkadal@pamulang1
#
#   2. Delete user with username dummy
#
#      genuser.sh dummy

error() {
    echo "Error: $1" 1>&2
    exit 1 
}

[[ $EUID > 0    ]] && error "This script must be run as root"
[  -d /etc/skel/ ] || error "No /etc/skel directory"

testAkun() {
   if   [ -z "$1" ]                  ; then error "No Argument" ; fi
   if   getent passwd $1 > /dev/null ; then true
   else false  ; fi
}

NONO="backup bin daemon games gnats irc list lp mail man messagebus news nobody "
NONO="$NONO poor proxy root sshd sync sys systemd-coredump systemd-network"
NONO="$NONO systemd-resolve systemd-timesync uucp www-data "
NONO="$NONO bind dnsmasq fetchmail postfix syslog tcpdump uuidd "

while read l; do
    # skip blank lines
    [ -z "$l" ] && continue

    # Grab username and ssh key
    uname=$(echo "$l" | cut -d ' ' -f1)
    pkey1=$(echo "$ll" | awk '{print $2}')
    pkey2=$(echo "$ll" | awk '{print $3}')
    pkey3=$(echo "$ll" | awk '{print $4}')
    echo $NONO | grep -q "$uname" && error "$uname is reserved"

    if   [[ "$pkey1" == "ssh-rsa" ]] ; then
        if ! testAkun $uname ; then
            echo "Create $uname"
            useradd -k /etc/skel -m -s /bin/bash $uname
        fi
        [ -d /home/$uname/ ] || error "No $uname Home"
        mkdir -pv     /home/$uname/.ssh/
        chmod -v  700 /home/$uname/.ssh/
        echo "$pkey1 $pkey2 $pkey3" > /home/$uname/.ssh/authorized_keys
        touch /var/mail/$uname
        chmod -v  600 /home/$uname/.ssh/authorized_keys
        chmod -v  600 /var/mail/$uname
        chown -vR $uname:$uname /home/$uname/ /var/mail/$uname
    elif [[ "$pkey1" == ""        ]] ; then
        if testAkun $uname ; then
            echo "delete $uname"
            userdel -r $uname
        else
            echo "what $uname ?"
        fi
    else
        error "Wrong Format"
    fi
done < "${1:-/dev/stdin}"

exit 0

