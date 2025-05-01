import sys
import struct

def read_block(path):
    with open(path, "rb") as f:
        prev_hash = f.read(32)
        merkle_root = f.read(32)

        # Read difficulty, timestamp, tx_count
        difficulty = struct.unpack("<I", f.read(4))[0]
        timestamp = struct.unpack("<I", f.read(4))[0]
        tx_count = struct.unpack("<I", f.read(4))[0]

        # Read nonce (always last in header)
        # Seek from end minus 4 bytes (uint32_t nonce)
        f.seek(-4, 2)
        nonce = struct.unpack("<I", f.read(4))[0]

        # Go back to the transaction data start
        f.seek(32 + 32 + 4 + 4 + 4)

        print(f"📦 Block File: {path}")
        print(f"├─ Previous Hash: {prev_hash.hex()}")
        print(f"├─ Merkle Root:   {merkle_root.hex()}")
        print(f"├─ Difficulty:    {difficulty}")
        print(f"├─ Timestamp:     {timestamp}")
        print(f"├─ Nonce:         {nonce}")
        print(f"├─ Transactions:  {tx_count}")
        print("└─ Contents:")

        for i in range(tx_count):
            text_len_bytes = f.read(4)
            if len(text_len_bytes) != 4:
                print(f"❌ Unexpected EOF reading tx[{i}] text_len.")
                break
            text_len = struct.unpack("<I", text_len_bytes)[0]

            text = f.read(text_len).decode("utf-8", errors="replace")

            sig_len_bytes = f.read(4)
            if len(sig_len_bytes) != 4:
                print(f"❌ Unexpected EOF reading tx[{i}] sig_len.")
                break
            sig_len = struct.unpack("<I", sig_len_bytes)[0]

            sig = f.read(sig_len)
            if len(sig) != sig_len:
                print(f"❌ Unexpected EOF reading tx[{i}] sig bytes.")
                break

            print(f"   [{i}] '{text}'")
            print(f"       Signature ({sig_len} bytes): {sig.hex() if sig_len > 0 else '[none]'}")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python3 block_explorer.py block_XX.bin")
        sys.exit(1)
    read_block(sys.argv[1])
