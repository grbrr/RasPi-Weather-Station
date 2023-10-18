import smbus2

# I2C parameters
port = 1
address = 0x23

# Instruction set
POWER_DOWN                  = 0b0000_0000   # No active state
POWER_ON                    = 0b0000_0001   # Waiting for measurement command. 
RESET                       = 0b0000_0111   # Reset data register value. Reset command is not acceptable in Power Down mode.
CONTINUOUS_HIGH_RES_MODE_1  = 0b0001_0000   # Start measurement at 1lx resolution. Measurement Time is typically 120ms. 
CONTINUOUS_HIGH_RES_MODE_2  = 0b0001_0001   # Start measurement at 0.5lx resolution. Measurement Time is typically 120ms.
CONTINUOUS_LOW_RES_MODE     = 0b0001_0011   # Start measurement at 4lx resolution. Measurement Time is typically 16ms.
ONE_TIME_HIGH_RES_MODE_1    = 0b0010_0000   # Start measurement at 1lx resolution. Measurement Time is typically 120ms. Device is automatically set to Power Down after measurement.
ONE_TIME_HIGH_RES_MODE_2    = 0b0010_0001   # Start measurement at 0.5lx resolution. Measurement Time is typically 120ms. Device is automatically set to Power Down after measurement.
ONE_TIME_LOW_RES_MODE       = 0b0010_0011   # Start measurement at 4lx resolution. Measurement Time is typically 16ms. Device is automatically set to Power Down after measurement.

bus = smbus2.SMBus(port)

def convert_to_number(data):
    return int.from_bytes(data, byteorder="big") / 1.2 

def read_light():
    data = bus.read_i2c_block_data(address, ONE_TIME_HIGH_RES_MODE_1, 2)
    return convert_to_number(data)

print(read_light())
