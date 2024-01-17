
SRC = \
	src/ptupdater.c \
	src/base64.c \
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
	src/sleep/ptlib_sleep.c \
	src/ptu_parse.c

OBJ = $(patsubst %.c,%.o, $(SRC))

BIN_DIR = bin

# Defaults to args for x86-64 build:
LFLAGS= --static
LIBS= -lxml2 -lm -ldl -lssl -lcrypto -lb64 -lz -llzma

%.o: %.c
	$(CC) -c $< -std=gnu99 -o $@

all: $(SRC)
	mkdir -p $(BIN_DIR)
	$(CC) -pthread -Wall -std=gnu99 -s -o ./$(BIN_DIR)/ptupdater $(INCLUDES) $(SRC) $(LFLAGS) $(LIBS)

clean:
	rm -rf $(OBJ)
	rm -rf $(BIN_DIR)
