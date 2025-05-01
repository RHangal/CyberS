import os
import struct
import hashlib
import time

BLOCK_DIR = "./block"
GENESIS_BLOCK_PATH = os.path.join(BLOCK_DIR, "block_00.bin")
BLOCK_START = b"BLOCKSTART"  # 10 bytes
BLOCK_END = b"BLOCKEND__"    # 10 bytes

GENESIS_TEXT = "Bear receives 100 coins"
tx_bytes = GENESIS_TEXT.encode("utf-8")
tx_len = len(tx_bytes)
tx_count = 1
sig_len = 0
sig_bytes = b""

# Compute Merkle root (hash(tx) || hash(tx))
tx_hash = hashlib.sha256(tx_bytes).digest()
merkle_root = hashlib.sha256(tx_hash + tx_hash).digest()

# Block header values
prev_hash = b"\x00" * 32  # No previous block
difficulty = 0
nonce = 0
timestamp = int(time.time())

# Ensure output directory
os.makedirs(BLOCK_DIR, exist_ok=True)

# Block body: [tx_count][tx_len][tx][sig_len][sig]
block_body = struct.pack("<I", tx_count)
block_body += struct.pack("<I", tx_len)
block_body += tx_bytes
block_body += struct.pack("<I", sig_len)
block_body += sig_bytes

# Full block: [prev_hash][merkle_root][difficulty][nonce][timestamp][block_body]
block_data = (
    prev_hash +
    merkle_root +
    struct.pack("<I", difficulty) +
    struct.pack("<I", timestamp) +
    block_body +
    struct.pack("<I", nonce) 
)

# Write block_00.bin
with open(GENESIS_BLOCK_PATH, "wb") as f:
    f.write(block_data)

print("✅ Genesis block created.")
print(f"   ↳ Block path: {GENESIS_BLOCK_PATH}")
