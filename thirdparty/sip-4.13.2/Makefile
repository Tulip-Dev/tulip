
all:
	@(cd sipgen; $(MAKE))
	@(cd siplib; $(MAKE))

install:
	@(cd sipgen; $(MAKE) install)
	@(cd siplib; $(MAKE) install)
	@test -d $(DESTDIR)/home/antoine/tulip_3_7_maint/build_release/thirdparty/sip-4.13.2/siplib || mkdir -p $(DESTDIR)/home/antoine/tulip_3_7_maint/build_release/thirdparty/sip-4.13.2/siplib
	cp -f sipconfig.py $(DESTDIR)/home/antoine/tulip_3_7_maint/build_release/thirdparty/sip-4.13.2/siplib/sipconfig.py
	cp -f /home/antoine/tulip_3_7_maint/thirdparty/sip-4.13.2/sipdistutils.py $(DESTDIR)/home/antoine/tulip_3_7_maint/build_release/thirdparty/sip-4.13.2/siplib/sipdistutils.py

clean:
	@(cd sipgen; $(MAKE) clean)
	@(cd siplib; $(MAKE) clean)
