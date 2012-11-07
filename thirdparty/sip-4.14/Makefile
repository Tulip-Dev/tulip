
all:
	@(cd sipgen; $(MAKE))
	@(cd siplib; $(MAKE))

install:
	@(cd sipgen; $(MAKE) install)
	@(cd siplib; $(MAKE) install)
	@test -d $(DESTDIR)/home/antoine/tulip_build/thirdparty/sip-4.14/siplib || mkdir -p $(DESTDIR)/home/antoine/tulip_build/thirdparty/sip-4.14/siplib
	cp -f sipconfig.py $(DESTDIR)/home/antoine/tulip_build/thirdparty/sip-4.14/siplib/sipconfig.py
	cp -f /home/antoine/tulip/thirdparty/sip-4.14/sipdistutils.py $(DESTDIR)/home/antoine/tulip_build/thirdparty/sip-4.14/siplib/sipdistutils.py

clean:
	@(cd sipgen; $(MAKE) clean)
	@(cd siplib; $(MAKE) clean)
