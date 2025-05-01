import subprocess
import os
import binascii
from pathlib import Path
import sys

SHA_PROGRAM = "./sha"
RSA_PROGRAM = "./rsa"
TEMP_HASH_FILE = "temp_hash.bin"
TEMP_SIG_FILE = "temp_sig.bin"

GROUP_TO_KEY = {
    "t_00": "bear_keys/unsafe.bad",
    "t_01": "shadow_keys/unsafe.bad",
    # Add more mappings if needed
}

def get_next_block_dir(base):
    existing = sorted(p.name for p in base.glob("b_*"))
    next_idx = len(existing)
    return base / f"b_{next_idx:02d}"

def run_sha(transaction_file):
    result = subprocess.run([SHA_PROGRAM, transaction_file], capture_output=True, text=True)
    return result.stdout.strip()

def save_temp_hash_file(raw_hash_bytes):
    with open(TEMP_HASH_FILE, "wb") as f:
        f.write(raw_hash_bytes)

def run_rsa_on_temp_hash(key_path):
    result = subprocess.run([RSA_PROGRAM, "-d", TEMP_HASH_FILE, TEMP_SIG_FILE, key_path])
    if result.returncode != 0:
        raise RuntimeError("RSA signing failed")

    with open(TEMP_SIG_FILE, "rb") as f:
        sig_raw = f.read()
        print(f"[DEBUG] Raw RSA sig bytes: {len(sig_raw)}")
        return binascii.hexlify(sig_raw).decode('ascii')

def process_transaction(transaction_path, idx, out_dir, key_path):
    with open(transaction_path, "r") as f:
        text = f.read().strip()

    hex_hash = run_sha(str(transaction_path))
    save_temp_hash_file(bytes.fromhex(hex_hash))
    signature_hex = run_rsa_on_temp_hash(key_path)

    out_file = out_dir / f"block_transaction_{idx:02d}.txt"
    with open(out_file, "w") as f:
        f.write("Transaction:\n")
        f.write(text + "\n\n")
        f.write("Transaction Hash:\n")
        f.write(hex_hash + "\n\n")
        f.write("Transaction Signature:\n")
        f.write(signature_hex + "\n")

    print(f"✅ Created {out_file}")

def main():
    if len(sys.argv) > 1:
        group = sys.argv[1]
    else:
        group = "t_00"

    if group not in GROUP_TO_KEY:
        print(f"❌ Unknown group: {group}")
        return

    TRANSACTIONS_DIR = Path("./transactions") / group
    OUTPUT_BASE = Path("./block_transactions")
    OUTPUT_DIR = get_next_block_dir(OUTPUT_BASE)
    KEY_PATH = GROUP_TO_KEY[group]

    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
    tx_files = sorted(TRANSACTIONS_DIR.glob("transaction*.txt"))

    if not tx_files:
        print(f"❌ No transactions found in {TRANSACTIONS_DIR}")
        return

    for idx, path in enumerate(tx_files):
        try:
            process_transaction(path, idx, OUTPUT_DIR, KEY_PATH)
        except Exception as e:
            print(f"❌ Failed to process {path.name}: {e}")

    for temp in [TEMP_HASH_FILE, TEMP_SIG_FILE]:
        if os.path.exists(temp):
            os.remove(temp)

if __name__ == "__main__":
    main()
