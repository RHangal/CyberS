import struct
from pathlib import Path

BLOCKCHAIN_FILE = Path("./blockchain/blockchain.bin")
BLOCK_START = b"BLOCKSTART"
BLOCK_END = b"BLOCKEND__"
HASH_SIZE = 32

def read_block(f):
    while True:
        prefix = f.read(len(BLOCK_START))
        if not prefix:
            return None
        if prefix == BLOCK_START:
            break

    block_data = bytearray()
    while True:
        b = f.read(1)
        if not b:
            raise RuntimeError("Unexpected EOF")
        block_data += b
        if block_data[-len(BLOCK_END):] == BLOCK_END:
            return block_data[:-len(BLOCK_END)]

def parse_and_print_block(i, block_bytes):
    cursor = 0
    def read(fmt):
        nonlocal cursor
        size = struct.calcsize(fmt)
        if cursor + size > len(block_bytes):
            raise RuntimeError(f"Invalid read at block {i}, offset {cursor}")
        val = struct.unpack(fmt, block_bytes[cursor:cursor+size])[0]
        cursor += size
        return val

    def read_bytes(n):
        nonlocal cursor
        if cursor + n > len(block_bytes):
            raise RuntimeError(f"Invalid read for {n} bytes at block {i}, offset {cursor}")
        val = block_bytes[cursor:cursor+n]
        cursor += n
        return val

    prev_hash = read_bytes(HASH_SIZE).hex()
    merkle_root = read_bytes(HASH_SIZE).hex()
    difficulty = read("<I")
    timestamp = read("<I")
    tx_count = read("<I")
    nonce = read("<I")

    print(f"🧱 Block {i:02d}")
    print(f"├─ Previous Hash: {prev_hash}")
    print(f"├─ Merkle Root:   {merkle_root}")
    print(f"├─ Difficulty:    {difficulty}")
    print(f"├─ Timestamp:     {timestamp}")
    print(f"├─ Nonce:         {nonce}")
    print(f"└─ Transactions:  {tx_count}")

    for t in range(tx_count):
        text_len = read("<I")
        text = read_bytes(text_len).decode(errors="replace")
        sig_len = read("<I")
        read_bytes(sig_len)
        print(f"   [{t}] '{text}'")
        print(f"        Signature Length: {sig_len} bytes")
    print("")

def main():
    if not BLOCKCHAIN_FILE.exists():
        print("❌ blockchain.bin not found.")
        return

    with open(BLOCKCHAIN_FILE, "rb") as f:
        print("📚 Blockchain View\n")
        block_index = 0
        while True:
            block = read_block(f)
            if not block:
                break
            if block_index == 0:
                print(f"(Skipping genesis block 00)\n")
                block_index += 1
                continue
            parse_and_print_block(block_index, block)
            block_index += 1

if __name__ == "__main__":
    main()
