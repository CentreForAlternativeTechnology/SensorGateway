# Adapted from http://mrbook.org/blog/tutorials/make/

# ==== Configuration ====

# ----- Compilation -----
CC=g++
STD=c++0x
CFLAGS=-Wall -pedantic -std=$(STD) -g -Ofast -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s #-DENCRYPTION
INCS=-I .

# ------ Libraries ------
GMP=\
-lgmp

SQLITE=\
-lsqlite3

RF24=\
-lrf24-bcm \
-lrf24network \
-lrf24mesh

CURL=\
-lcurl

MICROHTTPD=\
-lmicrohttpd

LIBS=\
$(GMP) \
$(SQLITE) \
$(RF24) \
$(CURL) \
$(MICROHTTPD)

# ---- Clean Command ----
RM=rm -f
# =======================

CMD=$(CC) $(CFLAGS) $(INCS)

SRCS=\
tiny-AES128-C/aes.c \
cat/snsrgtwy/radio/messages/MessageException.cpp \
cat/snsrgtwy/radio/messages/CorruptMessageException.cpp \
cat/snsrgtwy/radio/messages/EncryptedMessageException.cpp \
cat/snsrgtwy/radio/messages/Message.cpp \
cat/snsrgtwy/radio/messages/LowPowerRadioCommsSpecMessage.cpp \
cat/snsrgtwy/radio/messages/DataMessage.cpp \
cat/snsrgtwy/radio/messages/KeyExchangeMessage.cpp \
cat/snsrgtwy/radio/messages/RegisterMessage.cpp \
cat/snsrgtwy/radio/messages/EncryptedMessage.cpp \
cat/snsrgtwy/radio/messages/AttrMessage.cpp \
cat/snsrgtwy/radio/messages/RegisterAttrMessage.cpp \
cat/snsrgtwy/radio/messages/PostMessage.cpp \
cat/snsrgtwy/radio/messages/FeedValueMessage.cpp \
cat/snsrgtwy/radio/Data.cpp \
cat/snsrgtwy/radio/DataVector.cpp \
cat/snsrgtwy/radio/MessageFactory.cpp \
cat/snsrgtwy/radio/NodeDatabase.cpp \
cat/snsrgtwy/radio/SQLiteNodeDatabase.cpp \
cat/snsrgtwy/radio/NodeDatabaseCreator.cpp \
cat/snsrgtwy/radio/RF24Handler.cpp \
cat/snsrgtwy/radio/Mailbox.cpp \
cat/snsrgtwy/radio/Inbox.cpp \
cat/snsrgtwy/radio/Outbox.cpp \
cat/snsrgtwy/web/http/Response.cpp \
cat/snsrgtwy/web/http/RequestException.cpp \
cat/snsrgtwy/web/http/Request.cpp \
cat/snsrgtwy/web/http/RequestSslConfigException.cpp \
cat/snsrgtwy/web/http/RequestSslConfig.cpp \
cat/snsrgtwy/web/Api.cpp \
cat/snsrgtwy/web/EmoncmsApi.cpp \
cat/snsrgtwy/web/EmoncmsApiConfig.cpp \
cat/snsrgtwy/web/EmoncmsApiConfigException.cpp \
cat/snsrgtwy/web/server/Server.cpp \
cat/snsrgtwy/web/server/ServerException.cpp \
cat/snsrgtwy/web/server/NodeRequest.cpp \
cat/snsrgtwy/web/server/EncryptedNodeRequest.cpp \
cat/snsrgtwy/web/server/FeedValueNodeRequest.cpp \
cat/snsrgtwy/DiffieHellman.cpp \
cat/snsrgtwy/SensorGateway.cpp

OBJS:=$(addsuffix .o,$(basename $(SRCS)))
EXEC=sensor-gateway

all: $(SRCS) $(EXEC)

$(EXEC): $(OBJS)
	$(CMD) $(OBJS) $(LIBS) -o $@

.cpp.o:
	$(CMD) -c $< $(RF24) -o $@

cat/snsrgtwy/web/http/Request.o:
	$(CMD) -c $(basename $@).cpp $(CURL) -o $@

cat/snsrgtwy/radio/SQLitePublicKeyDatabase.o:
	$(CMD) -c $(basename $@).cpp $(SQLITE) -o $@

cat/snsrgtwy/DiffieHellman.o:
	$(CMD) -c $(basename $@).cpp $(GMP) -o $@

cat/snsrgtwy/web/Server.o:
	$(CMD) -c $(basename $@).cpp $(MICROHTTPD) -o $@

clean:
	$(RM) $(OBJS)
