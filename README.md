# iplink daemon
Small tool that runs "ip -s link" in a loop and as a daemon. 
Used in my labs to test "ip -s link" as a daemon deployment to check for interface counters. 

### How to use
- Deploy to Openshift
~~~
oc create -f https://raw.githubusercontent.com/backchristoffer/iplink-daemon/refs/heads/master/iplink.yaml
~~~

- Local build
~~~
gcc -pthread -o iplinkd iplink.c
~~~