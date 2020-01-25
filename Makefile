# findwild make file

# defaults for parameters that may be pre-defined
CXXFLAGS += -Wall -g -rdynamic -O2 -Wno-format-truncation 
PREFIX ?= /usr

# target install directories
BINDIR = $(PREFIX)/bin
SHAREDIR = $(PREFIX)/share/findwild
DATADIR = $(SHAREDIR)/data
ICONDIR = $(SHAREDIR)/icons
IMAGEDIR = $(SHAREDIR)/images
LOCALESDIR = $(SHAREDIR)/locales
DOCDIR = $(PREFIX)/share/doc/findwild
MANDIR = $(PREFIX)/share/man/man1
APPDATADIR = $(PREFIX)/share/appdata
MENUFILE = $(PREFIX)/share/applications/findwild.desktop

CFLAGS = $(CXXFLAGS) -c `pkg-config --cflags gtk+-3.0`
CFLAGS += $(CPPFLAGS)
LIBS = `pkg-config --libs gtk+-3.0` -lpthread

findwild: findwild.o zfuncs.o
	$(CXX) $(LDFLAGS) -o findwild findwild.o zfuncs.o $(LIBS)

findwild.o: findwild.cc
	$(CXX) $(CFLAGS) -o findwild.o findwild.cc

zfuncs.o: zfuncs.cc zfuncs.h
	$(CXX) $(CFLAGS) zfuncs.cc  -D PREFIX=\"$(PREFIX)\" -D DOCDIR=\"$(DOCDIR)\"   \

install: findwild uninstall
	mkdir -p  $(DESTDIR)$(BINDIR)
	mkdir -p  $(DESTDIR)$(DATADIR)
	mkdir -p  $(DESTDIR)$(ICONDIR)
	mkdir -p  $(DESTDIR)$(IMAGEDIR)
	mkdir -p  $(DESTDIR)$(LOCALESDIR)
	mkdir -p  $(DESTDIR)$(DOCDIR)
	mkdir -p  $(DESTDIR)$(MANDIR)
	mkdir -p  $(DESTDIR)$(PREFIX)/share/applications
	mkdir -p  $(DESTDIR)$(APPDATADIR)
	cp -f  findwild $(DESTDIR)$(BINDIR)
	cp -f -R  data/* $(DESTDIR)$(DATADIR)
	cp -f -R  images/* $(DESTDIR)$(IMAGEDIR)
	cp -f -R  doc/* $(DESTDIR)$(DOCDIR)
	gzip -f -9 $(DESTDIR)$(DOCDIR)/changelog
	# man page
	cp -f doc/findwild.man findwild.1
	gzip -f -9 findwild.1
	cp findwild.1.gz $(DESTDIR)$(MANDIR)
	rm -f findwild.1.gz
	# menu (desktop) file
	cp -f findwild.desktop $(DESTDIR)$(MENUFILE)
	cp -f findwild.png $(DESTDIR)$(ICONDIR)

uninstall:
	rm -f  $(DESTDIR)$(BINDIR)/findwild
	rm -f -R  $(DESTDIR)$(SHAREDIR)
	rm -f -R  $(DESTDIR)$(DOCDIR)
	rm -f  $(DESTDIR)$(MANDIR)/findwild.1.gz
	rm -f  $(DESTDIR)$(MENUFILE)

clean: 
	rm -f  findwild
	rm -f  *.o
 

