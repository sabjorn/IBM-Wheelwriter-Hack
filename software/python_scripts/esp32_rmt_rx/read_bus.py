# coding: utf-8
# used with current state of 'esp32_input' to try to decode bytes
import serial
import json
from math import ceil

def ticks_to_bits(tick_length, ideal_tick_per_bit=420):
    """
    Convert a length in ticks to the equivalent length in bits based on magnitudes of the ideal tick length.

    Args:
    tick_length (int): The length in ticks to be converted.
    ideal_tick_per_bit (int): The ideal number of ticks that represent one bit.

    Returns:
    int: The length in bits.
    """
    # Calculate the number of bits by dividing the tick length by the ideal tick length
    # and rounding up to account for any partial bits.
    return ceil(tick_length // ideal_tick_per_bit)  # Using negative floor division for ceiling effect.

ser = serial.Serial("/dev/cu.usbserial-1120", 115200)

# take one sample of 'a'
ser.flushInput()
ser.flushOutput()
ser.flush()
lines = []
while True:
    try:
        line = ser.readline()
        line = line[:-2]
        line = json.loads(line.decode('utf-8'))
        lines.append(line)
    except KeyboardInterrupt:
        break # ctrl+c
    except Exception as exp:
        print(exp)

minimum_value = 9999999
for line in lines:
    if line["duration0"] < 1 or line["duration1"] < 1: continue
    minimum_value = min(minimum_value, line["duration0"]) 
    minimum_value = min(minimum_value, line["duration1"]) 
print(f"minimum_value: {minimum_value}")

pulse_length = 415
bits = []
for line in lines:
    bin0 = [1 if line["state0"] == "high" else 0] * ticks_to_bits(line["duration0"], ideal_tick_per_bit=pulse_length)#int(line["duration0"] // pulse_length)
    bin1 = [1 if line["state1"] == "high" else 0] * ticks_to_bits(line["duration1"], ideal_tick_per_bit=pulse_length)#int(line["duration1"] // pulse_length)
    bits += bin0
    bits += bin1
#bits = bits[1:]

# Note:
# from README
# Commands seem to always start with 0b100100001
# should be possible to frame data with this
# also, it's LSB../
coded_bytes = []
num_bits = len(bits) - 1
for i in range(num_bits // 10):
    start = i * 10
    end = start + 10
    b = bits[start:end]
    b.reverse() # maybe for now don't do this
    coded_bytes.append(b) #bin(int(''.join(map(str, b)), 2) << 1))

print(coded_bytes)
