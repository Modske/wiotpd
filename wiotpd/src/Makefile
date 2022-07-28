LDFLAGS:=-liotp-as-device -lblobmsg_json -lubox -lubus -largp
SRCS:=$(wildcard ./*.c)
OBJS:=$(SRCS:.c=.o)
BIN:=wiotpd

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) $(OBJS) $(BIN)