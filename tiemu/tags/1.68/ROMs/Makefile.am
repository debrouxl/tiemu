SHARE_DIR=$(prefix)/share/tiemu
INSTALL=@INSTALL@

ROMS = *.rom
EXTRA_DIST = $(ROMS)

install: $(ROMS)
	@echo Installing ROMs...
	@$(INSTALL) -m755 -d $(SHARE_DIR)
	@$(INSTALL) -m755 -d $(SHARE_DIR)/ROMs
	@for I in ./*.rom ; do \
	$(INSTALL) -m644 $$I $(SHARE_DIR)/ROMs/; \
	done
	@echo Done

uninstall:
	@echo Removing ROMs...
	$(RM) $(SHARE_DIR)/ROMs/*.rom

clean: 
	$(RM) \#*
	$(RM) *~
	$(RM) core