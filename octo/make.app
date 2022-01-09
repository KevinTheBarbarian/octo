# -*- Makefile -*-

INCLUDING += $(LIBRARIES)
LINKPATHS += $(LIBRARIES:%=-L%)
LINKLIBS  += $(LIBRARIES:%=-l%)

include $(BASE)/make.cfg

CCOPTS=$(GENOPTS)
LINKOPTS=$(GENOPTS)

$(TARGET): $(OBJECTS) $(LIBFILES)
	for i in $(LIBRARIES); do (cd $$i ; $(MAKE) ; cp lib$${i}.a $(BASE)); done
	$(COMPILELINK) $(LINKOPTS) -o $(TARGET) $(OBJECTS) $(LINKPATHS) $(LINKLIBS)

%.o: %.cc
	$(COMPILEOBJ) $(CCOPTS) $(INCPATH) -c $<

clean:
	for i in $(LIBRARIES); do (cd $$i && make clean); done
	rm -vf $(TARGET) $(OBJECTS) *.a *.so

