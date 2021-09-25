# genuser.sh

Collection of script to create user from list of usernames and pubkey pair.  
The collection consist of 3 scripts:
- `reader.sh` -- Read multiple files and output a properly formatted input in form of username and ssh pair.
- `genuser.sh` -- Creates or delete user
- `genbatch.sh` -- Does all the above directly 

> `genuser.sh` and `genbatch.sh` must be run as root.

### Running examples  
1. Reading files with `reader.sh` and feed the output into `genuser.sh` using pipe  

    ```bash
    su -
    bash reader.sh example1 example2 example3 test.csv | bash genuser.sh
    ```
    
2. Read files and create/delete user directly using `genbatch.sh`  

    ```bash
    su -
    bash genbatch.sh example1 example2 example3 test.csv
    ```

### Cleaning examples  
```bash
su -
bash genuser.sh example4
```

### Customization  
This script uses [`useradd`](https://manpage.me/index.cgi?apropos=0&q=useradd&sektion=0&manpath=Debian+8.1.0&arch=default&format=html) to create user and [`userdel`](https://manpage.me/index.cgi?apropos=0&q=userdel&sektion=0&manpath=Debian+8.1.0&arch=default&format=html) to delete users. 
You can add options in both commands to customize user creation.

### Extensibility  
I created separated `genuser` and `reader` based on functionality so it won't be a big blob of mess and/or 
just in case if both script can be useful in other project.

2021 Dennis A. Walangadi (dnswd)
