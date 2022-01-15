CC=gcc
INCLUDES=-I/usr/lib/jvm/java-11-openjdk-amd64/include/ -I/usr/lib/jvm/java-11-openjdk-amd64/include/linux
CFLAGS=-shared -fPIC
TARGET_LIB=/usr/lib/libCubiomes.so #Java expects certain locations, /usr/local/lib is not one of them
TARGET_JAR=Cubiomes.jar

# .so first because it doesn't know how to make .class
.PHONY: all
all: Cubiomes.so $(TARGET_JAR) perform_tests
	\

# clean_lib before `ln` or else it will fail
.PHONY: install
install: all clean_lib 
	sudo ln ./Cubiomes.so $(TARGET_LIB)
	mvn install:install-file -Dfile=Cubiomes.jar -DgroupId=dev.corr -DartifactId=Cubiomes \
	-Dversion=1.0 -Dpackaging=jar -DgeneratePom=true

### Java
### Java
### Java
$(TARGET_JAR): dev/corr/Cubiomes.class
	jar -cf $@ $<

dev_corr_Cubiomes.h: dev/corr/Cubiomes.java
	javac -h . $<

### Native
### Native
### Native
cubiomes:
	git clone https://github.com/Cubitect/cubiomes

cubiomes/libcubiomes.a: cubiomes
	$(MAKE) -C cubiomes

Cubiomes.so: Cubiomes.c Cubiomes.h dev_corr_Cubiomes.h cubiomes/libcubiomes.a
	$(CC) -o $@ $< cubiomes/libcubiomes.a $(CFLAGS) $(INCLUDES) -lm

### Tests
### Tests
### Tests
perform_tests: tests.c cubiomes/libcubiomes.a
	$(CC) -o perform_tests tests.c cubiomes/libcubiomes.a -lm
	./perform_tests

### Utility
### Utility
### Utility
.PHONY: clean
clean: clean_lib
	-rm dev_corr_Cubiomes.h
	-rm dev/corr/Cubiomes.class 
	-rm *.so
	-rm perform_tests
	-rm $(TARGET_JAR)

.PHONY: clean_lib
clean_lib:
	-sudo rm $(TARGET_LIB)

.PHONY: clean_full
clean_full: clean
	-rm -rf ./cubiomes