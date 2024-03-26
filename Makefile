
SRC = \
	src/ptupdater.c \
	src/fw_version.c \
	src/channel/channel.c \
	src/crc16_ccitt.c \
	src/dut_driver.c \
	src/dut_utils/dut_state.c \
	src/dut_utils/dut_utils.c \
	src/file/ptlib_file.c \
	src/hid/hidraw.c \
	src/I2C/i2cbusses.c \
	src/logging.c \
	src/pip/pip2.c \
	src/pip/pip2_cmd_id.c \
	src/pip/pip2_status_code.c \
	src/pip/pip3.c \
	src/pip/pip3_cmd_id.c \
	src/pip/pip3_self_test_id.c \
	src/pip/pip3_status_code.c \
	src/ptstr_char.c \
	src/report_data.c \
	src/sleep/ptlib_sleep.c

OBJ = $(patsubst %.c,%.o, $(SRC))

BIN_DIR = bin

CC ?= gcc
CFLAGS += -Wall -std=gnu99

LIB_FLAGS = -pthread
# Don't use -s/-static on Chromeos
#STATIC_BUILD ?= n
#ifeq ($(STATIC_BUILD), y)
#	LDFLAGS += -static
#endif

%.o: %.c
	$(CC) -c $<  $(CFLAGS) $(CPPFLAGS) -o $@

all: $(SRC)
	mkdir -p $(BIN_DIR)
	$(CC) -o ./$(BIN_DIR)/ptupdater $(SRC) $(CFLAGS) $(CPPFLAGS) $(LIB_FLAGS) $(LDFLAGS)

clean:
	rm -rf $(OBJ)
	rm -rf $(BIN_DIR)
