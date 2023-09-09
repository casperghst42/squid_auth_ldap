/*
  main.c
   
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
#include <string.h>
#include <lber.h>
#include <ldap_cdefs.h>
#include <ldap.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdarg.h>
#include <unistd.h>

// --------------------------------------------------------
#include "defines.h"
// --------------------------------------------------------


// --------------------------------------------------------
int getUIDPass (char* buf, char *user, char *pass);

int main (int argc, char **argv)
{
	char dnGrp[1024];  
  char buf[256];
  char udn[256];

  LDAP *ld;

  int rc, retVal = 1;
  int chkGroup = 0;
  int opt = 0;
  struct config conf;

  setbuf(stdout,NULL);

  _StartLog( );
  logging( RUN, "%s - %s - starting", PROG, VERS );

  if( (opt = options( argc, argv, &conf )) == 0 )
  {
    exit(1);
  }

  if( opt == OK_CFG )
  {
    if( readconf( conf.confFile, &conf ) == 0 )
      exit(1);
  }

  StartLog( &conf );

	// Get FQN of group - crappy code
  if( conf.is_pxyGroup )
  {
    int ret;
    logging( RUN, "- find DN for group %s\n", conf.pxyGroup );
    ret = getGroupDN(conf.ldapServer, conf.ldapPort, conf.searchBase, conf.pxyGroup, dnGrp );
    if( ret == -1 )
      exit(1);
    else
    if( ret != 1 )
    {
      logging( WARN, "- unable to find group: %s", conf.pxyGroup );
      exit(1);
    }
    
    logging( DEBUG, "- group DN: %s", dnGrp );
    chkGroup = 1;
  }  

  logging( RUN, "%s - %s - ready", PROG, VERS );

	// Main loop
  while( fgets(buf, 256, stdin) != NULL )
  {    
		char user[256];
		char passwd[256];
    int getid;
		
		user[0] = '\0';
		passwd[0] = '\0';
				
		startMsg( );	
	
    // If we didn't get both user and passwd - try again.
    getid = getUIDPass(buf, user, passwd);

		buildMsg( " - usr %s ", user );

    if( getid == 100 )
    {
      StopLog( );
      exit(100);
    }
      
    if( getid == 0 )
    {
      logging( DEBUG, "- did not get a valide string from the browser, ie. user pass");
      continue;
    }
    
		// ldap_open is faster than ldap_init
    if ((ld = ldap_open(conf.ldapServer, conf.ldapPort)) == NULL)
    {
      logging( RUN, "- unable to connect to LDAP server: %s:%d", conf.ldapServer, conf.ldapPort);
      exit (1);
    }
    
    logging( DEBUG, "- connected to ldapServer %s:%d", conf.ldapServer, conf.ldapPort);
			
    if( findUserDN( ld, conf.searchBase, user, udn ) )
    {
		  int isInGrp = 0;
    
		  buildMsg( "-> %s ", udn );
    
		  if( chkGroup )
      {
        if( isUserInLDAPgrp( ld, dnGrp, udn ) )
          isInGrp = 1;
      }  
      else
        isInGrp = 1;
    
      if( isInGrp )
      {
        retVal = canBind (ld, udn, passwd);
        if( retVal == LDAP_SUCCESS ) 
				{
					buildMsg( "-> Authenticated!" );
				  printf(OK);
				}
        else
				{
					buildMsg( "-> Not Authenticated!");
          printf(ERR);	
				}
      }
      else
			{
				buildMsg( "-> Not in Grp %s", udn );			
        printf(ERR);
			}
    }
    else
    {
		  buildMsg( "-> Cannot find user!");
      printf(ERR);
    }      
    
		logMsg( INFO );
    ldap_unbind (ld);    
  }


  logging( RUN, "%s - %s - stopping", PROG, VERS );
  StopLog();
  return 0;
}

int getUIDPass(char *buf, char *user, char *pass)
{
  char *p, *uid, *pwd;

  if(( p = strchr(buf, '\n')) != NULL)
    *p = '\0';

  logging( DEBUG, "- user string: %s", buf);

  if ((uid = strtok(buf, " ")) == NULL)
  {
    printf(ERR);
    logging( WARN, "- ERR: user");
    return 0;
  }

#ifdef DODEBUG
  if ((strcasecmp(uid, "quit")) == 0)
  {
    logging( WARN, "- quitting...");
    return 100;
  }
#endif
    
  if ((pwd = strtok(NULL, "")) == NULL)
  {
    printf (ERR);
    logging( WARN, "- ERR: no password!");
    return 0;
  }

  strcpy (user, uid);
  strcpy (pass, pwd);
  logging( DEBUG, "- got User: %s", user );
 	logging( DEBUG, "- got Password: %s", pass );

  return 1;
}


