# Squid Auth LDAP

### authentication via ldap for squid proxy server

I last did updated or made changes to this before 2002 (I cannot remember), I'll not look at it again, but put it here for anyone to fork and work if they feel like. Though there are better tools included in Squid these days. 

Based on squid_ldap_auth, by Glen Newton Modified to allow contextless login, to an Novell NDS LDAP Server, plus some other stuff like check for group member ship, and logging.  

Dependencies: You need to get the OpenLDAP libraries from http://www.openldap.org, or Novell's LDAP C Libraries for Linux, http://developer.novell.com

License: squid_ldap_auth is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 or (at your option) any later version.

Configuration: This is the information which has to be entered in squid.conf

```
authenticate_program /usr/local/squid/bin/squid_auth_ldap
  -S ldap.acme.com -s o=acme -g cn=users,o=acme
```
or you can use a configuration file

```
authenticate_program /usr/local/squid/bin/squid_auth_ldap \ 
  -f squid_auth_ldap.conf
```

squid_auth_ldap.conf
```
ldapserver myldap.acme.com
ldapport 389
searchbase o=acme
proxygroup cn=users,o=acme
loglevel 0
```

acl information in squid.conf
```
acl all 0.0.0.0/0.0.0.0
acl user_password proxy_auth REQUIRED
http_access allow user_password
http_access deny all
```
