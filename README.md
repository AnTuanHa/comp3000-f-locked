# COMP3000 F-Locked Project

F-locked is a file locking solution that aims to provide users with a way to
lock files with a password, restricting all file permissions after a lock has
been set. The user is only allowed to lock files it owns. Once a lock has been
set, the user may call the program again to unlock it with the set password.

# How to build and install
```bash
$ git clone https://github.com/zero57/comp3000-f-locked
$ cd comp3000-f-locked
$ make
$ sudo make install
```

Note, the `flocked` program must be run on a patched Linux Kernel, as we must
modify the `chmod` syscall so that the user cannot change the permissions of a
locked file. F-locked has been built and tested upon the linux kernel version
4.4.121, but newer versions will still work. The linux kernel installation
process may be different depending on the linux distribution that the user is
using, but applying the patch should not differ.

```bash
$ wget https://cdn.kernel.org/pub/linux/kernel/v4.x/linux-4.4.121.tar.xz
$ tar xvf linux-4.4.121.tar.xz
$ cd linux-4.4.121
$ patch -p1 < ../kernel_patches/*.patch
patching file fs/open.c
```

Alternatively, one can clone the linux kernel git repository that we have built
upon and install the modified linux kernel from there.

```bash
$ git clone https://github.com/zero57/linux-stable-v4.4.121
```

Once the patch has been applied or the Linux kernel git repository has been
cloned, follow your Linux distribution's instructions on installing a custom
Linux kernel.

# Example
```bash
$ mkdir /tmp/test
$ cd /tmp/test
$ echo "hello world" > myfile
$ ls -l myfile
-rw-r--r-- 1 MYUSER MYUSER 0 Apr 12 18:17 myfile
$ cat myfile
hello world
$ flocked myfile
Enter your password: mypassword
File successfully locked with the given password
$ ls -l myfile
---------- 1 MYUSER MYUSER 0 Apr 12 18:19 myfile
$ cat myfile
cat: myfile: Permission denied
$ chmod 0666 myfile
$ ls -l myfile
---------- 1 MYUSER MYUSER 0 Apr 12 18:20 myfile
$ flocked myfile
Enter your password to unlock: mypassword
Unlocking file...
Success
$ ls -l myfile
-rw-r--r-- 1 MYUSER MYUSER 0 Apr 12 18:21 myfile
$ cat myfile
hello world
```
# Configuring the custom action 

To set up the custom action in the file manager, click the "Edit" tab, and select "Configure custom actions..."
Add a new custom action titled "F-Locked" with the following command. 
```
/usr/local/bin/flocked.sh %f
```
Under the "Appearance Condtions" tab, check every box. 

# Software used

This program was made and tested in the COMP2404-2406-W18 Virtual Machine found on:
https://carleton.ca/scs/technical-support/virtual-machines/

The custom action was added using the file manager, "Thunar" version 1.6.11.
