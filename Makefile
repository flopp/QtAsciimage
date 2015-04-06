all: lib editor

clean: 
	(cd QtAsciimage ; $(MAKE) clean)
	(cd QtAsciimageEditor ; $(MAKE) clean)

distclean: 
	(cd QtAsciimage ; $(MAKE) distclean)
	(cd QtAsciimageEditor ; $(MAKE) distclean)

lib: QtAsciimage/Makefile
	@echo "-- Building Library"
	(cd QtAsciimage ; $(MAKE))

editor: QtAsciimageEditor/Makefile
	@echo "-- Building Editor"
	(cd QtAsciimageEditor ; $(MAKE))

QtAsciimage/Makefile: 
	@echo "-- creating $@"
	(cd QtAsciimage ; qmake)

QtAsciimageEditor/Makefile: 
	@echo "-- creating $@"
	(cd QtAsciimageEditor ; qmake)
