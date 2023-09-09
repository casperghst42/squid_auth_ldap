/*
  logging.c
  general function for logging
  
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
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>


#include "defines.h"

static char _msgBuffer[2048];
static int  _logLevel;

void _StartLog( )
{
  openlog(PROG, LOG_CONS | LOG_PID, LOG_FACILITY );
}

void StartLog( struct config *conf )
{
  StopLog( );
	_logLevel = conf->logLevel;
  if( conf->logDebug == 0 )
    openlog(PROG, LOG_CONS | LOG_PID, LOG_FACILITY );
  else
  if( conf->logDebug == 1 )
  {
    openlog(PROG, LOG_CONS | LOG_PERROR | LOG_PID, LOG_FACILITY );
  }  
}
  
void StopLog( )
{
  closelog( );
}

void logging( int ll, const char* fmt, ... )
{
  char buffer[1024];
  va_list ap;
  va_start( ap, fmt );
  
  vsnprintf( buffer, 1024, fmt, ap );  

  if( ll == DEBUG && _logLevel >= DEBUG )
	{
	  syslog( LOG_INFO, buffer );
#ifdef DEBUG		
		printf("DEBUG\n");
#endif		
	}
	else 
	if( ll == WARN && _logLevel >= WARN )
	{
	  syslog( LOG_INFO, buffer );
#ifdef DEBUG		
		printf("WARN\n");	
#endif		
	}
	else
	if( ll == INFO && _logLevel >= INFO )
	{
	  syslog( LOG_INFO, buffer );
#ifdef DEBUG		
		printf("INFO\n");
#endif		
	}
	else
	if( ll == RUN && _logLevel >= RUN )
	{
	  syslog( LOG_INFO, buffer );		
#ifdef DEBUG		
		printf("RUN\n");		
#endif		
	}
}

void startMsg( )
{
  _msgBuffer[0] = '\0';
}

void logMsg( int ll )
{
	logging( ll, _msgBuffer );
}

void buildMsg( const char *fmt, ... )
{
  char buffer[1024];
  va_list ap;
  va_start( ap, fmt );
  
  vsnprintf( buffer, 1024, fmt, ap );  
  strcat( _msgBuffer, buffer );  
}
