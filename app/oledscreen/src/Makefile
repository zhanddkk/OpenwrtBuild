# Build Example for OpenWRT
# make package/oled/{clean,compile} V=sc

# Toolchain
CROSS_COMPILE ?=
STRIP := ${CROSS_COMPILE}strip
GCC := ${CROSS_COMPILE}gcc

# Targe
TARGET := oledscreen
SRC = $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,${SRC})
DEPS = $(patsubst %.c,%.d,${SRC})

INC = 
LIBS = -lpthread -lm
LIB_PATH =

LDFLAGS += ${LIB_PATH} ${LIBS}
CFLAGS += -std=gnu99 -g -Wall ${INC}

all: ${TARGET}

${TARGET}: ${TARGET}.elf
	${STRIP} "$<" -o "$@"

${TARGET}.elf: ${OBJS}
	${GCC} ${CFLAGS} ${LDFLAGS} -o $@ ${OBJS}

%.o: %.c
	${GCC} ${CFLAGS} ${INC} -D__SOURCE_F__="\"$(notdir $<)\"" -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"

clean:
	rm -rfv ${OBJS} ${DEPS} ${TARGET}.elf ${TARGET}

-include $(DEPS)

.PHONY: all clean
