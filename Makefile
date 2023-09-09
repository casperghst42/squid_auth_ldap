# ----
OBJS		= main.o\
		  ldap_utils.o\
		  logging.o\
		  readconf.o\
		  options.o		  
HEADERS		= defines.h
LIBS		= -lldap -llber
EXEC		= squid_auth_ldap
CONF_FILE       = squid_auth_ldap.conf
README		= README
# DODEBUG enables "quit" command, do NOT use in production enviroment
# OPTIONS         = -DDODEBUG=1 -DDEBUG=1
CFLAGS		=
CPPFLAGS	=


prefix		= /usr/local/squid/bin/auth
install_files   = ${LDAP_EXE} ${CONF_FILE} ${README}

exec_prefix	= ${prefix}
exec_suffix	= 
cgi_suffix	= .cgi
top_srcdir	= ../..
bindir		= ${exec_prefix}
libexecdir	= ${bindir}
sysconfdir	= ${prefix}
localstatedir	= ${prefix}
srcdir		= .

CC		= gcc ${OPTIONS}
MAKEDEPEND	= /usr/X11R6/bin/makedepend
INSTALL		= /usr/bin/install -c
INSTALL_BIN	= ${INSTALL}
INSTALL_FILE	= ${INSTALL} -m 644
INSTALL_SUID	= ${INSTALL} -o root -m 4755
RANLIB		= ranlib
LN_S		= ln -s
PERL		= /usr/bin/perl
CRYPTLIB	= -lcrypt
REGEXLIB	= 
PTHREADLIB	= -lpthread
MALLOCLIB	= 
AC_CFLAGS	= -g -O2 -Wall
LDFLAGS		= -g
XTRA_LIBS	= -lm -lresolv -lbsd -lnsl
XTRA_OBJS	= 
MV		= /bin/mv
RM		= /bin/rm
SHELL		= /bin/sh


all: $(EXEC) 

$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(LIBS) $^

%.o: %.c $(HEADERS)
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

install-mkdirs:
	-@if test ! -d $(prefix); then \
		echo "mkdir $(prefix)"; \
		mkdir $(prefix); \
	fi
	-@if test ! -d $(libexecdir); then \
		echo "mkdir $(libexecdir)"; \
		mkdir $(libexecdir); \
	fi

install: all install-mkdirs
	@for f in $(install_files); do \
		if test -f $(libexecdir)/$$f; then \
			echo $(RM) $(libexecdir)/$$f; \
			$(RM) $(libexecdir)/$$f; \
		fi; \
		echo $(INSTALL_BIN) $$f $(libexecdir); \
		$(INSTALL_BIN) $$f $(libexecdir); \
	done

clean:
	-$(RM) -f $(OBJS)
	-$(RM) -f $(EXEC)

