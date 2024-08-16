CC=g++ 
# change CC to your compiler
SRCS = $(wildcard ./src/*.cpp ./include/bigint/Big*.cc)
HEAD_SRC = $(wildcard ./include/*.h)
IDIR = ./include
CFLAGS=-O2 -I$(IDIR) -std=c++14
OBJDIR = ./build

all: messenger-test hmac aes-gcm hkdf messenger

messenger-test: $(OBJDIR)/hmac.o $(OBJDIR)/hkdf.o $(OBJDIR)/sha256.o $(OBJDIR)/common.o $(OBJDIR)/aes-gcm.o $(OBJDIR)/aes.o $(OBJDIR)/aes-ctr.o $(OBJDIR)/messenger.o $(OBJDIR)/message.o $(OBJDIR)/double-ratchet.o $(OBJDIR)/diffie-hellman.o $(OBJDIR)/bigint/BigInteger.o $(OBJDIR)/bigint/BigUnsigned.o $(OBJDIR)/bigint/BigIntegerUtils.o $(OBJDIR)/bigint/BigUnsignedInABase.o $(OBJDIR)/bigint/BigIntegerAlgorithms.o
	mkdir -p bin
	$(CC) -o bin/messenger-test $(OBJDIR)/hmac.o $(OBJDIR)/hkdf.o $(OBJDIR)/sha256.o $(OBJDIR)/common.o $(OBJDIR)/aes-gcm.o $(OBJDIR)/aes.o $(OBJDIR)/aes-ctr.o $(OBJDIR)/messenger.o $(OBJDIR)/message.o $(OBJDIR)/double-ratchet.o $(OBJDIR)/diffie-hellman.o $(OBJDIR)/bigint/BigInteger.o $(OBJDIR)/bigint/BigUnsigned.o $(OBJDIR)/bigint/BigIntegerUtils.o $(OBJDIR)/bigint/BigUnsignedInABase.o $(OBJDIR)/bigint/BigIntegerAlgorithms.o ./src/apps/messenger-test.cpp $(CFLAGS)

hmac: $(OBJDIR)/hmac.o $(OBJDIR)/sha256.o $(OBJDIR)/common.o ./src/apps/hmac-app.cpp
	mkdir -p bin
	$(CC) -o bin/hmac $(OBJDIR)/hmac.o $(OBJDIR)/sha256.o $(OBJDIR)/common.o ./src/apps/hmac-app.cpp $(CFLAGS)

aes-gcm: $(OBJDIR)/aes-gcm.o $(OBJDIR)/aes.o $(OBJDIR)/aes-ctr.o $(OBJDIR)/sha256.o $(OBJDIR)/common.o ./src/apps/aes-gcm-app.cpp
	mkdir -p bin
	$(CC) -o bin/aes-gcm $(OBJDIR)/aes-gcm.o $(OBJDIR)/aes.o $(OBJDIR)/aes-ctr.o $(OBJDIR)/sha256.o $(OBJDIR)/common.o ./src/apps/aes-gcm-app.cpp $(CFLAGS)

hkdf: $(OBJDIR)/hkdf.o $(OBJDIR)/hmac.o $(OBJDIR)/sha256.o $(OBJDIR)/common.o ./src/apps/hkdf-app.cpp
	mkdir -p bin
	$(CC) -o bin/hkdf ./src/apps/hkdf-app.cpp $(OBJDIR)/hkdf.o $(OBJDIR)/hmac.o $(OBJDIR)/sha256.o $(OBJDIR)/common.o  $(CFLAGS)

messenger: $(OBJDIR)/hmac.o $(OBJDIR)/hkdf.o $(OBJDIR)/sha256.o $(OBJDIR)/common.o $(OBJDIR)/aes-gcm.o $(OBJDIR)/aes.o $(OBJDIR)/aes-ctr.o $(OBJDIR)/messenger.o $(OBJDIR)/message.o $(OBJDIR)/double-ratchet.o $(OBJDIR)/diffie-hellman.o $(OBJDIR)/bigint/BigInteger.o $(OBJDIR)/bigint/BigUnsigned.o $(OBJDIR)/bigint/BigIntegerUtils.o $(OBJDIR)/bigint/BigUnsignedInABase.o $(OBJDIR)/bigint/BigIntegerAlgorithms.o
	mkdir -p bin
	$(CC) -o bin/messenger $(OBJDIR)/hmac.o $(OBJDIR)/hkdf.o $(OBJDIR)/sha256.o $(OBJDIR)/common.o $(OBJDIR)/aes-gcm.o $(OBJDIR)/aes.o $(OBJDIR)/aes-ctr.o $(OBJDIR)/messenger.o $(OBJDIR)/message.o $(OBJDIR)/double-ratchet.o $(OBJDIR)/diffie-hellman.o $(OBJDIR)/bigint/BigInteger.o $(OBJDIR)/bigint/BigUnsigned.o $(OBJDIR)/bigint/BigIntegerUtils.o $(OBJDIR)/bigint/BigUnsignedInABase.o $(OBJDIR)/bigint/BigIntegerAlgorithms.o ./src/apps/messenger-app.cpp $(CFLAGS)

$(OBJDIR)/%.o: src/%.cpp
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/bigint/%.o: src/bigint/%.cc
	mkdir -p $(OBJDIR)/bigint
	$(CC) $(CFLAGS) -I$(IDIR)/bigint -c -o $@ $<

clean:
	rm -r bin/
	rm -r build/ 