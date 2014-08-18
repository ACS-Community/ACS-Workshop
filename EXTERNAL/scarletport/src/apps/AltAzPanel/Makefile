include Makefile.in

all: $(PROGRAMNAME)
	cp src/AltAzPanel.class .

$(PROGRAMNAME): 
	cd src; make

clean:
	rm AltAzPanel.class
	cd src; make clean

run:	
	java -Dabeans.home=$(ACS_ROOT)/acsdata/config/abeans/Config/ -DManager.defaultReference=corbaloc::$(DEFAULT_IP):$(DEFAULT_PORT)/Manager -classpath $(CLASSPATH) AltAzPanel
