/*
 * Copyright (c) Parade Technologies, Ltd. 2023.
 */

#ifndef _I2CBUSSES_H
#define _I2CBUSSES_H

#include <unistd.h>

#define EEPROM_I2C_GROUP 1
#define EEPROM_I2C_ADDR 0x50

#define PT_SUPPLY_POWER_I2C_GROUP 0
#define PT_SUPPLY_POWER_I2C_ADDRESS 0x28

#define PT_SUPPLY_ADC_I2C_GROUP 0
#define PT_SUPPLY_ADC_I2C_ADDRESS 0x48

#define I2C_SLAVE_TPS65132_BUS 0
#define I2C_SLAVE_TPS65132_ADDRESS 0x3E

struct i2c_adap {
	int nr;
	char *name;
	const char *funcs;
	const char *algo;
};

struct i2c_adap *gather_i2c_busses(void);
void free_adapters(struct i2c_adap *adapters);

int lookup_i2c_bus(const char *i2cbus_arg);
int parse_i2c_address(const char *address_arg);
int open_i2c_dev(int i2cbus, char *filename, size_t size, int quiet);
int set_slave_addr(int file, int address, int force);

#define MISSING_FUNC_FMT	"Error: Adapter does not have %s capability\n"

#endif
