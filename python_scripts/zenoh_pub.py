import zenoh
import time

# --- Configuration ---
# Define connection details
endpoint = "udp/demo.zenoh.io:7447"
key = "esp32/public/test"

# Create a Zenoh config object
conf = zenoh.Config()
conf.insert_json5("connect", f'{{"endpoints": ["{endpoint}"]}}')

# --- Main Program ---
print("--- Zenoh Publisher ---")
print(f"I am a Publisher!")
print(f"Connecting to: {endpoint}")
print(f"Publishing on Key: {key}\n")

print("Opening session...")
session = zenoh.open(conf)

print(f"Declaring publisher for key expression '{key}'...")
pub = session.declare_publisher(key)

value_counter = 0
print("\nStart publishing every 29 seconds...")
while True:
    try:
        message = f"Hello from Python PUB #{value_counter}"
        print(f" >>> Sending: '{message}'")
        pub.put(message)
        value_counter += 1
        time.sleep(29)  # Send a message every 29 seconds
    except KeyboardInterrupt:
        break

# --- Cleanup ---
print("\nClosing session...")
pub.undeclare()
session.close()
