# Adapted from http://mrbook.org/blog/tutorials/make/

# ==== Configuration ====
CC=g++
CFLAGS=-Wall -pedantic -std=c++0x -g -DENCRYPTION

INCS=\
-I ../../.. \
-I .. 

LIBS=\
-lboost_unit_test_framework \
-lgmp \
-lsqlite3 \
-lcurl \
-lrf24-bcm \
-lrf24network \
-lrf24mesh


RM=rm -f
# =======================

CMD=$(CC) $(CFLAGS) $(INCS)

MAIN=../boost_test_main.cpp
SRCS=\
../../../tiny-AES128-C/aes.c \
../../snsrgtwy/DiffieHellman.cpp \
../../snsrgtwy/radio/RF24Handler.cpp \
../../snsrgtwy/radio/NodeDatabaseCreator.cpp \
../../snsrgtwy/radio/messages/MessageException.cpp \
../../snsrgtwy/radio/messages/CorruptMessageException.cpp \
../../snsrgtwy/radio/messages/Message.cpp \
../../snsrgtwy/radio/messages/LowPowerRadioCommsSpecMessage.cpp \
../../snsrgtwy/radio/messages/DataMessage.cpp \
../../snsrgtwy/radio/messages/KeyExchangeMessage.cpp \
../../snsrgtwy/radio/messages/EncryptedMessage.cpp \
../../snsrgtwy/radio/messages/RegisterMessage.cpp \
../../snsrgtwy/radio/messages/AttrMessage.cpp \
../../snsrgtwy/radio/messages/PostMessage.cpp \
../../snsrgtwy/radio/messages/RegisterAttrMessage.cpp \
../../snsrgtwy/radio/messages/FeedValueMessage.cpp \
../../snsrgtwy/radio/Data.cpp \
../../snsrgtwy/radio/DataVector.cpp \
../../snsrgtwy/radio/MessageFactory.cpp \
../../snsrgtwy/radio/NodeDatabase.cpp \
../../snsrgtwy/radio/SQLiteNodeDatabase.cpp \
../../snsrgtwy/web/http/Response.cpp \
../../snsrgtwy/web/http/Request.cpp \
../../snsrgtwy/web/http/RequestSslConfigException.cpp \
../../snsrgtwy/web/http/RequestSslConfig.cpp \
../../snsrgtwy/web/server/NodeRequest.cpp \
../../snsrgtwy/web/server/EncryptedNodeRequest.cpp \
../../snsrgtwy/web/server/FeedValueNodeRequest.cpp \
../../snsrgtwy/web/Api.cpp \
../../snsrgtwy/web/EmoncmsApi.cpp \
../../snsrgtwy/web/EmoncmsApiConfig.cpp \
../../snsrgtwy/web/EmoncmsApiConfigException.cpp \
all_tests.cpp



MAINO=$(MAIN:.cpp=.o)
OBJS=$(addsuffix .o,$(basename $(SRCS)))
EXEC=test-snsrgtwy

all: $(MAIN) $(SRCS) $(EXEC)

$(EXEC): $(MAINO) $(OBJS)
	$(CMD) $(OBJS) $(MAINO) $(LIBS) -o $@

.cpp.o:
	$(CMD) -c $< $(LIBS) -o $@

refresh: all
	rm -f all_tests.o

clean:
	$(RM) $(OBJS)
