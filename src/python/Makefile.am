EXTRA_DIST= \
 AUTHORS \
 COPYING \
 MANIFEST.in \
 ostatistic \
 otestpoint \
 scripts \
 setup.py.in 

BUILT_SOURCES = \
 setup.py \
 ostatistic/interface/openstatisticapi_pb2.py

all-local:
	$(PYTHON) setup.py build

clean-local: setup.py
	$(PYTHON) setup.py clean
	-rm -f $(BUILT_SOURCES)
	-rm -rf build
	-rm -f MANIFEST
	-rm -rf openstatistic_python.egg-info
	-find . -name "*.pyc" -delete

DISTCLEANFILES=setup.py

sdist:
	$(PYTHON) setup.py sdist

edit = sed \
        -e 's|@VERSION[@]|$(VERSION)|g' \
        -e 's|@PYTHON[@]|$(PYTHON)|g'

setup.py:	setup.py.in
	if test -f $@; then chmod u+w $@; fi
	$(edit) $< > $@
	chmod g-w,u-w $@

install-exec-hook: $(BUILT_SOURCES)
	$(PYTHON) setup.py install \
	    -O1 \
	    --prefix=$(prefix) \
	    --exec-prefix=$(exec_prefix) \
	    $(if $(DESTDIR),--root=$(DESTDIR))

ostatistic/interface/openstatisticapi_pb2.py: ../libostatistic/openstatisticapi.proto
	protoc -I../libostatistic --python_out=ostatistic/interface  $<
