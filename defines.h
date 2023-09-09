/*
  Author: Casper Pedersen
          casperghst42@fastmail.fm
  
  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
//---------------------------------------------------------------------------
#ifndef SQUID_AUTH_LDAP
#define SQUID_AUTH_LDAP
//---------------------------------------------------------------------------
#define PROG "squid_auth_ldap"
#define VERS "2.0.b2"
//---------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <lber.h>
#include <ldap_cdefs.h>
#include <ldap.h>
#include <sys/time.h>

//---------------------------------------------------------------------------
#define OK "OK\n"
#define ERR "ERR\n"
#define USRSRCATTR "uid"
#define MEMBERATTR "uniqueMember"
//---------------------------------------------------------------------------
#define OK_NOCFG 1
#define OK_CFG 2
//---------------------------------------------------------------------------

// ldap_utils.c
int canBind( LDAP* ld, char *dn, char *passwd );

int findUserDN( LDAP* ld, char* searchBase, char *uid, char *udn );

int getGroupDN( char* ldapServer, int ldapPort, char* srcBase, 
                char* grp, char* grpDN );

int isUserInLDAPgrp( LDAP *ld, char *gdn, char *dn );

// logging.c
#define LOG_FACILITY LOG_LOCAL6 // (see "man 3 syslog")
#define DEBUG 3
#define WARN  2
#define INFO  1
#define RUN   0

// readconf.h

struct config {
  char ldapServer[200];
  int is_ldapServer;
  int  ldapPort;
  int is_ldapPort;
  char searchBase[512];
  int is_searchBase;
  char pxyGroup[512];
  int is_pxyGroup;
  char confFile[256];
  int is_confFile;
  int logLevel;
  char logFile[256];  
  int is_logFile;
  int logDebug;
};

#define LDAPSERVER "LDAPSERVER"
#define LDAPPORT   "LDAPPORT"
#define SEARCHBASE "SEARCHBASE"
#define PROXYGROUP "PROXYGROUP"
#define LOGLEVEL   "LOGLEVEL"
#define LOGFILE    "LOGFILE"

int readconf( char* conffile, struct config* );

// options.c
int options( int argc, char* argv[], struct config* );

// log
void StartLog( struct config* );
void _StartLog( );
void StopLog( );
void logging( int ll, const char* fmt, ... );
void startMsg( );
void logMsg( int ll );
void buildMsg( const char* fmt, ...);

//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------


