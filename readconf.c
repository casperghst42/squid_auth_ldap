/*
  readconf.c
  read the configuration file
  
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
*/

#include "defines.h"

static void head( char* );
static void trail( char* );

int readconf( char* conffile, struct config* conf )
{
  FILE *fp;
  char buf[512];
  char *p;
  int  line = 0;
  int  found = 0;

  conf->is_ldapServer = 0;
  conf->is_ldapPort = 0;
  conf->is_searchBase = 0;
  conf->is_pxyGroup = 0;
  conf->is_confFile = 0;
  conf->is_logFile = 0;
  conf->logLevel = 0;
  conf->logDebug = 0;

  conf->ldapServer[0] = '\0';
	conf->ldapPort = 389;
  conf->searchBase[0] = '\0';
  conf->pxyGroup[0] = '\0';
  conf->logFile[0] = '\0';
  
  fp = fopen( conffile, "r" );
  if( fp == NULL )
  {
    logging( RUN, "- cannot open configuration file: %s", conffile );
    return 0;
  }
  
  while( (fgets(buf, 512, fp)) != NULL)
  {
    char *parm;
    char *val;
    
    line++;
    
    if( (p = strchr(buf, '\n')) != NULL )
      *p = '\0';
    
    if( buf[0] == '#' )
      continue;
      
    logging(DEBUG,"- conf. string %s", buf);
    
    parm = strtok( buf, " " );
    val = strtok( NULL, "" );
    
    
    if( (parm == NULL) || (val == NULL) )
    {
      logging(DEBUG,"- invalid entry in line: %d", line);
      fclose(fp);
      return 0;
    }

    trail( parm ); head( parm );
    trail( val ); head( val );
    logging(DEBUG,"- parm: %s - val: %s", parm, val );
  
    if( (strcasecmp(LDAPSERVER, parm)) == 0 )
    {
      strcpy( conf->ldapServer, val );
      conf->is_ldapServer = 1;
    }
    else
    if( (strcasecmp(LDAPPORT, parm)) == 0 )
    {
      conf->ldapPort = atoi( (char*) val );
      conf->is_ldapPort = 1;
    }
    else
    if( (strcasecmp(SEARCHBASE, parm)) == 0 )
    {
      strcpy( conf->searchBase, val );
      conf->is_searchBase = 1;
    }
    else
    if( (strcasecmp(PROXYGROUP, parm)) == 0 )
    {
      strcpy( conf->pxyGroup, val );
      conf->is_pxyGroup = 1;
    } 
    else
    if( (strcasecmp(LOGFILE, parm)) == 0 )
    {
      strcpy( conf->logFile, val );
    }
    else
    if( (strcasecmp(LOGLEVEL, parm)) == 0 )
    {
      conf->logLevel = atoi( val );
    }
    
  } 
  
  fclose(fp);
 
  if( !conf->is_ldapServer )
  {
    fprintf(stderr,"- missing LDAPSERVER <param> in %s\n", conffile );
    return 0;
  }
  if( !conf->is_ldapPort )
  {
    fprintf(stderr,"- missing LDAPPORT <param> in %s\n", conffile );
    return 0;
  }
  if( !conf->is_searchBase )
  {
    fprintf(stderr,"- missing SEARCHBASE <param> in %s\n", conffile );
    return 0;
  }
  
  return 1;
}

// strip spaces from beginning
void head( char *buf )
{
  int l;
  
  if( buf == NULL ) return;

  l = strlen( buf );
  
  while( (buf[0] == ' ') && (l > 0) )
  {    
    int x;
    for( x = 0; x < l; x++ )
      buf[x] = buf[x+1];
      
    buf[l-1] = '\0';  
    
    l = strlen(buf);  
  }
}

// strip spaces from end
void trail( char *buf )
{
  int l;
  int i;

  if( buf == NULL ) return;
  
  l = strlen( buf ) - 1;
  
  for( i = l; i >= 0; i-- )
  {
    if( buf[i] == ' ' )
      buf[i] = '\0';
    else
      break; 
  }
}
