/*
  ldap_utils.c 
  general functions for LDAP
	 
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
#include <sys/time.h>

#include "defines.h"

int canBind(LDAP *ld, char *dn, char *passwd)
{
  int rc; 
  logging( INFO, "- password check for %s", dn );
  rc = ldap_simple_bind_s( ld, dn, passwd );
  if( rc != LDAP_SUCCESS )
  {
    logging( DEBUG, "- password not OK!");
    return rc;
  }
	
  logging( DEBUG, "- password OK!");
  return 0;
}

int isDNGrp( char *grp )
{
  char **val;
  int x;
  
  val = ldap_explode_dn( grp, 0 );
  
  if( strchr( val[0], '=') == NULL )
  {
    logging( DEBUG, "- no full group");
    return 0;
  }
  
  if( val[0][0] != '\0' )
  {
    int i = 0;
    while( val[i] != NULL )
    {
      logging( DEBUG, "- (%d) %s", i, val[i] );
      i++;
    }
    return 1;
  }
  
  free(val);

  return 0;
}

int getGroupDN( char* ldapServer, int ldapPort, char* srcBase, 
                char* grp, char* grpDN )
{
  LDAP *ld;
  LDAPMessage *searchResult, *entry;
  struct timeval timeOut;
  char searchStr[512];
  char *dn;
  int rc;
  int found = 0;
	
  timeOut.tv_sec = 10L;
  timeOut.tv_usec = 0L;

  if( isDNGrp( grp ) )
  {
    logging( DEBUG, "- group has DN");
    strcpy( grpDN, grp );
    return 1;
  }

  logging( DEBUG, "- open connection to ldapserver: %s:%d", ldapServer, ldapPort);
	
  if( (ld = ldap_open( ldapServer, ldapPort ) ) == NULL )
  {
    logging( WARN, "- cannot login to: %s:%d", ldapServer, ldapPort);
    return -1;
  }
	
  sprintf(searchStr, "(&(objectClass=groupOfNames) (| (cn=%s)))", grp);

  logging( DEBUG, "- search for: %s", searchStr );
  	
  rc = ldap_search_st( ld,
										   srcBase,
											 LDAP_SCOPE_SUBTREE,
											 searchStr,
										   NULL, 0,
											 &timeOut,
											 &searchResult );

  logging( DEBUG, "- search done");

  if( ldap_count_entries( ld, searchResult ) == 0 )
  {
    ldap_unbind(ld);
    return 0;
  }
	
  logging( DEBUG, "- found an entry");
  
  entry = ldap_first_entry( ld, searchResult );											 
  if( entry != NULL )
  {
    dn = ldap_get_dn( ld, entry );
    strcpy( grpDN, dn );
    logging( DEBUG, "- entry found: %s", grpDN );
    free( dn );
    found = 1;
  }
					
  ldap_msgfree(searchResult);
  ldap_unbind( ld );						 
	
  return found;
}

int findUserDN( LDAP *ld, char* searchBase, char* uid, char* udn )
{
  char searchStr[512];
  LDAPMessage *searchResult, *entry;
  struct timeval timeOut;
  char *dn;
  int rc;
  int found = 0;
	
  timeOut.tv_sec = 10L;
  timeOut.tv_usec = 0L;
	
  sprintf (searchStr, "(&(objectClass=person) (| (%s=%s)))", USRSRCATTR, uid);
  logging( DEBUG, "- searchstr: %s", searchStr );
  logging( DEBUG, "- start searching for uid: %s", uid );
	
  rc = ldap_search_st (ld,
                 searchBase,
		 LDAP_SCOPE_SUBTREE,
		 searchStr, 
		 NULL, 
		 0, 
		 &timeOut, 
		 &searchResult);

  logging( DEBUG, "- search done");

  if( ldap_count_entries(ld, searchResult) == 0 )
  {
    logging( WARN, "- user \"%s\", not found!\n", uid);
    ldap_msgfree (searchResult); 
		return 0;
  }

  logging( DEBUG, "- found a entry");
    
  entry = ldap_first_entry(ld, searchResult );
	
  if( entry != NULL )
  {
    dn = ldap_get_dn(ld,entry);
		
    if( dn != NULL )
    {
      strcpy( udn, dn );
      free(dn);
			
      logging( DEBUG, "- DN found: %s", udn );			
      found = 1;
    }	
    else  
    {
      udn[0] = '\0';
      found = 0;
    }	
  }

  ldap_msgfree (searchResult);    
	
  return found;	
}

int isUserInLDAPgrp( LDAP *ld, char *gdn, char *dn )
{
  int rc;
	
  logging( DEBUG, "- is user %s in %s\n", dn, gdn );
  if( ldap_compare_s( ld, gdn, MEMBERATTR, dn ) == LDAP_COMPARE_TRUE )
  {
    logging( DEBUG, "- user \"%s\" is in Group \"%s\"", dn, gdn );
    return 1;		
  }
	
  logging( DEBUG, "- user \"%s\" is NOT in Group \"%s\"", dn, gdn );
  return 0;
}

