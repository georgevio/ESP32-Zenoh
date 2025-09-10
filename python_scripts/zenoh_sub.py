import zenoh
import time

# --- Configuration ---
endpoint = "udp/192.168.178.181:7447"
key = "faces/images"

# --- Global Counter ---
message_counter = 0

# --- Functions ---
def listener(sample):
    """Callback function to handle incoming messages."""
    global message_counter
    key_expr = sample.key_expr
    try:
        payload = bytes(sample.payload).decode('utf-8')
        print(f" < Received on '{key_expr}': '{payload}'")
    except:
        message_counter += 1
        print(f" < Received on '{key_expr}': new message '{message_counter}'")

# --- Main Program ---
print("--- Zenoh Subscriber ---")
print("I am a Subscriber")
print(f"Connecting to: {endpoint}")
print(f"Subscribing to Key: {key}\n")

conf = zenoh.Config()
conf.insert_json5("connect", f'{{"endpoints": ["{endpoint}"]}}')

print("Opening session...")
session = zenoh.open(conf)

print(f"Declaring subscriber for '{key}'...")
sub = session.declare_subscriber(key, listener)

print("\nWaiting for messages... Press Ctrl+C to quit.")
try:
    while True:
        time.sleep(1)
except KeyboardInterrupt:
    pass

# --- Cleanup ---
print("\nClosing session...")
sub.undeclare()
session.close()
