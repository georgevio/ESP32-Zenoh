import zenoh
import time
import argparse

# zenoh_server = "udp/demo.zenoh.io:7447"
# zenoh_server = "tcp/localhost:7447"
zenoh_server = "udp/192.168.178.181:7447"
#zenoh_server = "tcp/192.168.28.1:7447"

def listener(sample):
    """Callback function for subscriber."""
    try:
        payload = bytes(sample.payload).decode('utf-8')
        key_expr = sample.key_expr
        print(f" < Received on '{key_expr}': '{payload}'")
    except Exception as e:
        print(f"Error decoding message: {e}")

def main():
    # Command-Line Arguments
	
	# Scenario 1: Server-Only Communication (client mode)
	# Terminal 1 (Subscriber):
	# python zenoh_node.py --role sub --mode client
	# Terminal 2 (Publisher):
	# python zenoh_node.py --role pub --mode client
	
	# Scenario 2: P2P-Allowed Communication (peer mode)
	# Terminal 1 (Subscriber):
	# python zenoh_node.py --role sub --mode peer # peer is the default
	# Terminal 2 (Publisher):
	# python zenoh_node.py --role pub --mode peer

    parser = argparse.ArgumentParser(
        prog="zenoh_node",
        description="Zenoh publisher/subscriber with multi communication modes."
    )
    parser.add_argument(
        "-r", "--role",
        required=True,
        choices=['pub', 'sub'],
        help="Options: 'pub' is publisher, 'sub' is subscriber."
    )
    parser.add_argument(
        "-m", "--mode",
        choices=['client', 'peer'],
        default='peer',
        help=(
            "'client': Connect only to the router (server-only mode, no P2P). "
            "'peer': Allow P2P discovery (default)."
        )
    )
    parser.add_argument("-e", "--endpoint", default=zenoh_server, help="The Zenoh router endpoint.")
    parser.add_argument("-k", "--key", default="esp32/public/test", help="The key to pub/sub to.")
    args = parser.parse_args()

    # Configuration
    print(f" Zenoh {args.role.capitalize()} ")
    print(f"Role: {args.role.upper()} | Mode: {args.mode.upper()} | Endpoint: {args.endpoint} | Key: {args.key}\n")
    conf = zenoh.Config()

    # Connection mode: peer vs client
    if args.mode == 'client':
        print("Mode == 'client'. Server only communication, no multicast discovery")
        # Disable multicast scouting, so prevent P2P connections
        conf.insert_json5("scouting", '{"multicast": {"enabled": false}}')
    else:
        print("Mode == 'peer'. Multicast discovery enabled (default).")

    # Server/router to connect to
    conf.insert_json5("connect", f'{{"endpoints": ["{args.endpoint}"]}}')

    # Main
    print("Opening session...")
    session = zenoh.open(conf)

    try:
        if args.role == 'pub':
            print(f"Publisher for key '{args.key}'...")
            pub = session.declare_publisher(args.key)
            value_counter = 0
            print("\nStart publishing every 11 seconds...")
            while True:
                message = f"Hello from Python PUB ({args.mode.upper()}) #{value_counter}"
                print(f" >>> Sending: '{message}'")
                pub.put(message)
                value_counter += 1
                time.sleep(11)
        
        elif args.role == 'sub':
            print(f"Subscribed at: '{args.key}'...")
            sub = session.declare_subscriber(args.key, listener)
            print("\nWaiting for publishers... Press Ctrl+C to quit.")
            while True:
                time.sleep(1)

    except KeyboardInterrupt:
        print("\nInterrupted by user.")
    finally:
        # Cleanup & Exxit
        print("Ending session...")
        if 'pub' in locals():
            pub.undeclare()
        if 'sub' in locals():
            sub.undeclare()
        session.close()

if __name__ == "__main__":
    main()