# genuser.sh

Create user from list of usernames and pubkey pair. This script must be run as root.  

### Running examples  
```bash
su -
bash genuser.sh example1 example2 example3
```

### Cleaning examples  
```bash
su -
bash genuser.sh example4
```

### Customization  
This script uses `useradd` to create user. You can tweak the line to customize generated users.

2021 Dennis A. Walangadi (dnswd)
