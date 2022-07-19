
.patched:
	sed -i "s|#define LIBBEEX_VERSION.*|#define LIBBEEX_VERSION $(LIBBEEX_VERSION)|g" beex_def.h

	touch $@

.configured: .patched

	touch $@

