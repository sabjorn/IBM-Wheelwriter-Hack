# coding: utf-8
# used with current state of 'esp32_input' to try to decode bytes
import serial
import json

ser = serial.Serial("/dev/cu.usbserial-0001", 115200)

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

pulse_length = 4
bits = []
for line in lines:
    bin0 = [1 if line["state0"] == "high" else 0] * (line["duration0"] // pulse_length)
    bin1 = [1 if line["state1"] == "high" else 0] * (line["duration1"] // pulse_length)
    bits += bin0
    bits += bin1
bits = bits[1:]

coded_bytes = []
num_bits = len(bits) - 1
for i in range(num_bits // 10):
    start = i * 10
    end = start + 10
    b = bits[start:end]
    b.reverse()
    coded_bytes.append(b) #bin(int(''.join(map(str, b)), 2) << 1))

print(coded_bytes)
