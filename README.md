# syslog-ng-persist
syslog-ng-persists is a simple manipulation tool for ```syslog-ng.persist``` file.

## Building
Following components are required to build syslog-ng-persist:
* [cmake] - cross-platform open-source build system;
* [syslog-ng] - Open Source log management solution

Then, the application can be compiled as the following:
```
mkdir build && cd build
cmake ..
make all
```

[cmake]:http://www.cmake.org/
[syslog-ng]:https://syslog-ng.org/

## Running
Currently, only three following actions are supported.
### List entries from the file
```
> syslog-ng-persist --list syslog-ng.persist 
host_id
systemd-journal
run_id
```
### Show saved ```journald``` cursor
```
> syslog-ng-persist --show-journald-cursor syslog-ng.persist 
journald cursor: s=4668dee673b04869b93bf96d8234d487;i=3996c2;b=c4f333e72d074daebddc586f1b15f247;m=735deb81cd4;t=51f367e2bbaec;x=45b8386ed7eab66c
```
### Save ```journald``` cursor
Please, stop ```syslog-ng``` daemon and do ```syslog-ng.persist``` backup before doing this action. Note, that the software is provided under GPL-2.0 terms without any warranty.
```
> journalctl --show-cursor -n 1  -r
-- Logs begin at Ср 2013-08-07 17:16:27 MSK, end at Чт 2016-06-02 11:30:06 MSK. 
-- cursor: s=4668dee673b04869b93bf96d8234d487;i=6a6b95;b=9a5fbfb3adb14f3ca56d9bf457cd9fc6;m=450a3a9d9a6;t=534476a2cb61a;x=4b79921fe6c1620c
> syslog-ng-persist --save-journald-cursor='s=4668dee673b04869b93bf96d8234d487;i=6a6b95;b=9a5fbfb3adb14f3ca56d9bf457cd9fc6;m=450a3a9d9a6;t=534476a2cb61a;x=4b79921fe6c1620c' syslog-ng.persist
```
