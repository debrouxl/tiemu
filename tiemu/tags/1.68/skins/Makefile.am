SHARE_DIR=$(prefix)/share/tiemu
INSTALL=@INSTALL@

SKINS = *.skn
EXTRA_DIST = $(SKINS)

install: $(SKINS)
	@echo Installing skins...
	@$(INSTALL) -m755 -d $(SHARE_DIR)
	@$(INSTALL) -m755 -d $(SHARE_DIR)/skins
	@for I in ./*.skn ; do \
		$(INSTALL) -m644 $$I $(SHARE_DIR)/skins/; \
	done
	@echo Done

uninstall:
	@echo Removing skins...
	$(RM) $(SHARE_DIR)/skins/*.skn

clean: 
	$(RM) \#*
	$(RM) *~
	$(RM) core