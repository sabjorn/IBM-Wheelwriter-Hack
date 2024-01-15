import numpy as np
import matplotlib.pyplot as plt
import csv


def load_oscilloscope_data(filename: str) -> list[dict[str, str]]:
    with open(filename, newline="") as csvfile:
        entries = [lines for lines in csv.DictReader(csvfile, delimiter=",")]
    return entries
  
def edge_detect(vals, thresh, min_ticks=100):
    derivative = np.diff(vals)

    # Set a threshold for detecting edges
    threshold = thresh  # This is a simple heuristic; adjust as necessary
    #threshold = np.max(np.abs(derivative)) * thresh  # This is a simple heuristic; adjust as necessary

    # Find where the derivative exceeds the threshold
    edges = np.where(np.abs(derivative) >= threshold)[0]
    
    # remove false postivies that are too close together in time
    filtered_edges = edges[np.insert(np.diff(edges) >= min_ticks, 0, True)]
    return filtered_edges

c = load_oscilloscope_data("a.csv")
threshold = 0.2
signal_key = list(c[0].keys())[1]
vals = np.asarray([float(v[signal_key]) for v in c[11:]])
times = np.asarray([float(v["Record Length"]) for v in c[11:]])

sample_period = times[1] - times[0]
bit_period = 5.35e-6 
bit_interval = bit_period / sample_period
   
edges = edge_detect(vals, thresh=threshold)
print(f"thesh: {threshold}, edge count: {len(edges)}")
signal_start_offset = edges[0] - 500
signal_end_offset = edges[-1] + 500

times = times[signal_start_offset:signal_end_offset]
vals = vals[signal_start_offset:signal_end_offset]
edges = edges - signal_start_offset

grid_steps = int(len(vals) // bit_interval)

#plt.plot(np.arange(len(times)),vals)
#for edge in edges:
#    plt.axvline(x=edge, color='r')
#    
#for t in range(0, grid_steps):
#    plt.axvline(x=edges[0] + (t*bit_interval), color='black', alpha=0.1)
#plt.show()

t_edges = times[edges]
bits = np.ceil(np.diff(t_edges) / bit_period)
z_flag = False
all_bits = []
for b in bits:
    z_flag = not z_flag
    if z_flag:
        all_bits += [0] * int(b)
        continue
    all_bits += [1] * int(b)

#all_bits = [1] + all_bits # needed for lower to function
state = "start_frame"
offset = 0 
messages = {}
for i in range(len(all_bits)):
    position = i + offset

    if position > len(all_bits):
        break

    b = all_bits[position]
    if state == "open":
        if b == 1 and all_bits[position + 1] == 0:
           state = "start_frame"
           continue

    if state == "start_frame":
        messages[position] = all_bits[position : position + 10]
        offset += 10 
        state = "open"
        continue

def _join_bytes(b: list[int]) -> str:
    if len(b) < 10:
        b+=[1]
    return ''.join([str(e) for e in b])

MESSAGE_MAP = {
        _join_bytes([0, 1, 0, 0, 0, 0, 1, 0, 0, 1]) : "START",
        _join_bytes([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]) : "BLANK",
        _join_bytes([0, 1, 1, 0, 1, 0, 0, 0, 0, 0]) : "LINE 336 -- send_letter -- after START",
        _join_bytes([0, 0, 0, 1, 1, 0, 0, 0, 0, 0]) : "LINE 456",
        _join_bytes([0, 0, 0, 1, 0, 0, 0, 0, 0, 0]) : "LINE 375 -- comes before letter?",
        _join_bytes([0, 0, 1, 1, 0, 0, 0, 1, 0, 0]) : "LETTER? -- a?",
        _join_bytes([0, 0, 1, 1, 1, 0, 1, 1, 0, 0]) : "AFTER LETTER?--not in doc",
        _join_bytes([0, 1, 1, 0, 0, 0, 0, 0, 0, 0]) : "LINE 338 (in function called send_letter)",
        _join_bytes([0, 1, 0, 0, 0, 0, 0, 0, 0, 0]) : "LINE 140 -- actual letter 'a'",
        _join_bytes([0, 0, 1, 0, 1, 0, 0, 0, 0, 0]) : "LINE 340 -- send_letter -- happens after 'letter'",
}
print("all lines referencing: beanWheelwriterControlWemos.ino")
for position, message in messages.items():
    message_type = MESSAGE_MAP.get(_join_bytes(message), "UNKNOWN")
    message.reverse()
    m_string=_join_bytes(message)
    print(f"{m_string} : start_position: {position} : {message_type}")
