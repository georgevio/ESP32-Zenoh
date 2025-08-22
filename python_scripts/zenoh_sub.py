import zenoh
import time

# --- Configuration ---
# Define connection details
endpoint = "udp/demo.zenoh.io:7447"
key = "esp32/public/test"

# --- Functions ---
def listener(sample):
    """Callback function to handle incoming messages."""
    payload = bytes(sample.payload).decode('utf-8')
    key_expr = sample.key_expr
    print(f" < Received on '{key_expr}': '{payload}'")

# --- Main Program ---
print("--- Zenoh Subscriber ---")
print(f"I am a Subscriber")
print(f"Connecting to: {endpoint}")
print(f"Subscribing to Key: {key}\n")

# Create a Zenoh config object
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
