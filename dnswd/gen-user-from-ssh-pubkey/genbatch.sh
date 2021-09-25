#!/bin/bash
# (c) 2021-2021 This is a free script

# REV06 dnswd Fri 24 Sep 2021 23:31:50 WIB
#       Add read Google Form's csv functionality
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
# Usage:
#   bash genuser.sh file1 file2 file3
#
# `file` contains list of usernames with their respective public key:
#   cbkadal ssh-rsa AAAAB3n+w== cbkadal@pamulang1 # have public key
#   dummy                                         # doesn't have public key
# 

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

for FILE in "$@"; do
    [ -f "$FILE" ] || continue;
    while IFS="," read -r uname pbkey; do
        uname=$(echo "$uname" | cut -d'"' -f 2)
        [[ "$uname" == "" ]] && continue;
	pbkey=$(echo "$pbkey" | cut -d'"' -f 2)
        pkey1=$(echo "$pbkey" | awk '{print $1}')
        pkey2=$(echo "$pbkey" | awk '{print $2}')
        pkey3=$(echo "$pbkey" | awk '{print $3}')
        echo $NONO | grep -q "$uname" && error "$uname is reserved"
        echo "$uname $pkey1 $pkey2 $pkey3"
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
done
exit
