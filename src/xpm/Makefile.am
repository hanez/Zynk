stock_images =

EXTRA_DIST = \
	$(stock_images)

noinst_HEADERS = \
	zynk-pixbufs.h

zynk-pixbufs.h: $(stock_images)
		-rm -f zynk-pixbufs.h
		var=`echo "$(stock_images)" | sed -e 's,\([^\.]*\)\.png,\1 \0,g'` ; \
		   echo $$var; \
		   gdk-pixbuf-csource --raw --build-list $$var > zynk-pixbufs.h
