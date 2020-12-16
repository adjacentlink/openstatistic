ACLOCAL_AMFLAGS = -I m4

SUBDIRS= \
 include \
 src

EXTRA_DIST=\
 AUTHORS \
 ChangeLog \
 COPYING \
 INSTALL \
 NEWS \
 README \
 autogen.sh \
 $(PACKAGE).spec.in \
 debian

BUILT_SOURCES= \
 $(PACKAGE).spec

cleantar:
	@(rm -f $(PACKAGE)*.tar.gz)

dist-hook:
	find $(distdir) -name "*pb2.py" -delete

edit = sed \
       -e 's|@VERSION[@]|$(VERSION)|g'\
       -e 's|@DATE_RFC2822[@]|$(DATE_RFC2822)|g'\
       -e 's|@DISTRIB_CODENAME[@]|$(DISTRIB_CODENAME)|g'

RPMARCH=`arch`

if HAVE_RPMBUILD

rpm: $(PACKAGE).spec dist-gzip
	mkdir -p .rpmbuild/BUILD \
	.rpmbuild/SPECS \
	.rpmbuild/SOURCES \
	.rpmbuild/SRPMS \
	.rpmbuild/RPMS/noarch \
	.rpmbuild/RPMS/$(RPMARCH) \
	.rpmbuild/tmp
	cp -f $(PACKAGE)-$(VERSION).tar.gz .rpmbuild/SOURCES
	rpmbuild --clean -ba $(top_srcdir)/$(PACKAGE).spec --target $(RPMARCH) \
    --define "_topdir $$PWD/.rpmbuild" \
    --define "_tmppath $$PWD/.rpmbuild/tmp" \
    $(RPMBUILD_ARGS)
	@echo "============================================================"
	@echo "RPMs located in .rpmbuild/RPMS and .rpmbuild/SRPMS"

endif

if HAVE_DEB

deb: dist-gzip
	rm -rf .debbuild
	mkdir -p .debbuild
	cp -f $(PACKAGE)-$(VERSION).tar.gz .debbuild
	cd .debbuild && tar xzf $(PACKAGE)-$(VERSION).tar.gz
	$(edit) .debbuild/$(PACKAGE)-$(VERSION)/debian/changelog.in > \
          .debbuild/$(PACKAGE)-$(VERSION)/debian/changelog
	cd .debbuild/$(PACKAGE)-$(VERSION) && dpkg-buildpackage -F -us -uc
	@echo "============================================================"
	@echo "debs located in .debbuild"
cleandeb:
	dh_clean
else

cleandeb:

endif

clean-local: cleantar cleandeb
	-rm -f $(BUILT_SOURCES)
	-rm -rf .rpmbuild
	-rm -rf .debbuild
	-rm -f debian/changelog

$(PACKAGE).spec: $(PACKAGE).spec.in
	if test -f $@; then chmod u+w $@; fi
	$(edit) $< > $@
	chmod g-w,u-w $@
