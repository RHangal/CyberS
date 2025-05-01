import subprocess
import struct
import hashlib
import os
import time
from pathlib import Path

BLOCK_TRANSACTIONS_DIR = Path("./block_transactions")
BLOCK_OUTPUT_DIR = Path("./block")
MERKLE_BIN = "./merkle"
PARSE_BIN_SCRIPT = "make_parsed_bin.py"
MAKE_BLOCK_SCRIPT = "make_block_transaction.py"
VERIFY_BLOCK_BIN = "./verify_block"

DIFFICULTY_PREFIX = b'\x00'
STATE_FILE = Path("./last_group.txt")
TEMP_BLOCK_FILE = Path("temp_block.bin")

def get_next_group():
    if not STATE_FILE.exists():
        STATE_FILE.write_text("t_00")
        return "t_00"
    current = STATE_FILE.read_text().strip()
    next_group = "t_01" if current == "t_00" else "t_00"
    STATE_FILE.write_text(next_group)
    return next_group

def get_latest_block_index():
    existing = sorted(BLOCK_OUTPUT_DIR.glob("block_*.bin"))
    if not existing:
        return -1
    return max(int(p.stem.split("_")[1]) for p in existing)

def get_previous_block_hash():
    latest_index = get_latest_block_index()
    if latest_index == -1:
        return b'\x00' * 32
    with open(BLOCK_OUTPUT_DIR / f"block_{latest_index:02d}.bin", "rb") as f:
        return hashlib.sha256(f.read()).digest()

def mine_block(header_base):
    nonce = 0
    while True:
        header = header_base + struct.pack("<I", nonce)
        block_hash = hashlib.sha256(header).digest()
        if block_hash.startswith(DIFFICULTY_PREFIX):
            return nonce, block_hash
        nonce += 1

def main():
    BLOCK_OUTPUT_DIR.mkdir(exist_ok=True)
    next_index = get_latest_block_index() + 1

    # Step 1: Alternate and generate new b_XX
    next_group = get_next_group()
    subprocess.run(["python3", MAKE_BLOCK_SCRIPT, next_group], check=True)

    batch_dir = sorted(BLOCK_TRANSACTIONS_DIR.glob("b_*"))[-1]
    parsed_path = batch_dir / "parsed_transactions.bin"

    # Step 2: Parse to binary
    subprocess.run(["python3", PARSE_BIN_SCRIPT, str(batch_dir)], check=True)

    # Step 3: Generate Merkle root
    merkle_process = subprocess.run([MERKLE_BIN, str(parsed_path)],
                                    capture_output=True, text=True, check=True)
    for line in merkle_process.stdout.splitlines():
        if line.strip().startswith("Merkle root:"):
            merkle_root = bytes.fromhex(line.split(":")[1].strip())
            break
    else:
        raise RuntimeError("Merkle root not found")

    # Step 4: Construct block header
    prev_hash = get_previous_block_hash()
    difficulty = 1
    timestamp = int(time.time())

    with open(parsed_path, "rb") as f:
        tx_count = struct.unpack("<I", f.read(4))[0]
        tx_data = f.read()

    header_base = (
        prev_hash +
        merkle_root +
        struct.pack("<I", difficulty) +
        struct.pack("<I", timestamp) +
        struct.pack("<I", tx_count) +
        tx_data
    )

    # Step 5: Mine block
    nonce, block_hash = mine_block(header_base)
    full_block = header_base + struct.pack("<I", nonce) 

    # Step 6: Verify block
    with open(TEMP_BLOCK_FILE, "wb") as f:
        f.write(full_block)
    subprocess.run([VERIFY_BLOCK_BIN, str(TEMP_BLOCK_FILE)], check=True)

    # Step 7: Save block to ./block/block_XX.bin
    block_filename = BLOCK_OUTPUT_DIR / f"block_{next_index:02d}.bin"
    with open(block_filename, "wb") as f:
        f.write(full_block)

    print(f"✅ Block {next_index:02d} mined with nonce={nonce} and hash={block_hash.hex()}")

    TEMP_BLOCK_FILE.unlink(missing_ok=True)

if __name__ == "__main__":
    main()
