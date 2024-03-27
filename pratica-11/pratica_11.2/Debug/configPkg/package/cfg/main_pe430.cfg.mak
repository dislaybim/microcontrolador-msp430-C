# invoke SourceDir generated makefile for main.pe430
main.pe430: .libraries,main.pe430
.libraries,main.pe430: package/cfg/main_pe430.xdl
	$(MAKE) -f C:\Users\DISLAY~1\workspace_v5_6\pratica_11.2/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\DISLAY~1\workspace_v5_6\pratica_11.2/src/makefile.libs clean

