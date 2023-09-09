/*
  options.c
  get command options
   
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

#include <stdio.h>
#include <unistd.h>

#include "defines.h"

static void usage( void )
{
  fprintf(stderr,"\n");
  fprintf(stderr,"Usage: squid_auth_ldap -[f config-file] -[Sps[gld]] -h\n");
  fprintf(stderr,"       -f cofiguration file\n");
  fprintf(stderr,"       -S ldapServer\n");
  fprintf(stderr,"       -p ldapPort (default 389)\n");
  fprintf(stderr,"       -s searchBase\n");
  fprintf(stderr,"       -g proxyGroup\n");
  fprintf(stderr,"       -l logfile (not implemented - using syslog)\n");
  fprintf(stderr,"       -d loglevel <0..3> (default 0)\n");
  fprintf(stderr,"       -D debug - logs to stderr\n");
  fprintf(stderr,"       -h help\n");
  fprintf(stderr,"\n");
  fprintf(stderr,"- debug levels:\n");
  fprintf(stderr,"        0 - none\n");
  fprintf(stderr,"        1 - info\n");
  fprintf(stderr,"        2 - warnings\n");
  fprintf(stderr,"        3 - debug\n");
  fprintf(stderr,"\n");
  fprintf(stderr,"squid_auth_ldap -f /etc/squid_auth_ldap.conf\n");
  fprintf(stderr,"squid_auth_ldap -S acme_ldap -p 389 -s o=acme -g pxyUsers\n");
  fprintf(stderr,"\n");
}

int options (int argc, char* argv[], struct config* conf)
{
  int opt;
  char msg[2024];
  msg[0] = '\0';
      
  conf->is_ldapServer = 0;
  conf->is_ldapPort = 0;
  conf->is_searchBase = 0;
  conf->is_pxyGroup = 0;
  conf->is_confFile = 0;
  conf->is_logFile = 0;
  conf->logLevel = 0;
  conf->logDebug = 0;
  conf->ldapPort = 389;
	
	
  opt = getopt( argc, argv, "DSpsgfldh" );
  while( opt != -1 )
  {
    switch( opt )
    {
      case 'D': conf->logDebug = 1;
                logging(DEBUG,"- debug");
                break;
    
      case 'S': strcpy( conf->ldapServer, argv[optind] );
                conf->is_ldapServer = 1;
                logging(DEBUG,"- ldapServer: %s ", conf->ldapServer );
								break;
      
      case 'p': conf->ldapPort = atoi( (char*)argv[optind]);
                conf->is_ldapPort = 1;
                logging(DEBUG,"- ldapPort: %d ", conf->ldapPort );
								break;

      case 's': strcpy( conf->searchBase, argv[optind] );
                conf->is_searchBase = 1;
                logging(DEBUG,"- searchBase: %s ", conf->searchBase );
								break;
		
      case 'g': strcpy( conf->pxyGroup, argv[optind] );
                conf->is_pxyGroup = 1;
                logging(DEBUG,"- pxyGroup: %s ", conf->pxyGroup );
								break;

      case 'f': strcpy( conf->confFile, argv[optind] );
                conf->is_confFile = 1;
                logging(DEBUG,"- confFile: %s ", conf->confFile );
								break;

      case 'l': // strcpy( conf->logFile, argv[optind] );
                /// conf->is_logFile = 1;
                /// logging("OPT logFile: %s\n", conf->logFile );
								break;
		
      case 'd': conf->logLevel = atoi(argv[optind]);
                logging(DEBUG,"- logLevel: %d ", conf->logLevel );
								break;		
		
      case 'h': usage( );
                exit(0);
		
      default : usage( ); 
                exit(0);
    }
    opt = getopt( argc, argv, "DSpsgfldh" );
  }
  
  if( conf->is_ldapServer && conf->is_ldapPort && conf->is_searchBase )
  {
    logging(RUN,"- enough params to run!");
    return( OK_NOCFG );
  }
  else
  if( conf->is_confFile )
  {
    logging(RUN,"- enough params to run!");    
    return( OK_CFG );
  }
  
  usage( );
  
  return 0;
}
