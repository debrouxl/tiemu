SHARE_DIR=$(prefix)/share/tiemu
INSTALL=@INSTALL@

PIXMAPS = *.xpm
EXTRA_DIST = $(PIXMAPS) logo.jpg

install: $(PIXMAPS)
	@echo Installing pixmaps and other images...
	@$(INSTALL) -m755 -d $(SHARE_DIR)
	@$(INSTALL) -m755 -d $(SHARE_DIR)/pixmaps
	@for I in ./*.xpm ; do \
		$(INSTALL) -m644 $$I $(SHARE_DIR)/pixmaps/; \
	done
	@for I in ./*.jpg ; do \
	        $(INSTALL) -m644 $$I $(SHARE_DIR); \
        done
	@echo Done
	@$(INSTALL) -m644 ../COPYING $(SHARE_DIR)

uninstall:
	@echo Removing pixmaps...
	$(RM) $(SHARE_DIR)/pixmaps/*.xpm

clean: 
	$(RM) \#*
	$(RM) *~
	$(RM) core