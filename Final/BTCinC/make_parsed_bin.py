import subprocess
import struct
import sys
from pathlib import Path

TX_BIN = "./txt_to_bin"

def get_sorted_transaction_files(tx_dir):
    return sorted(tx_dir.glob("block_transaction_*.txt"))

def main():
    if len(sys.argv) != 2:
        print("Usage: python3 make_parsed_bin.py <block_directory>")
        sys.exit(1)

    block_dir = Path(sys.argv[1])
    if not block_dir.is_dir():
        print(f"❌ Directory not found: {block_dir}")
        sys.exit(1)

    tx_files = get_sorted_transaction_files(block_dir)
    if not tx_files:
        print(f"❌ No transaction files found in {block_dir}")
        return

    out_file = block_dir / "parsed_transactions.bin"
    with open(out_file, "wb") as out:
        out.write(struct.pack("<I", len(tx_files)))  # number of transactions

        for path in tx_files:
            print(f"Processing {path.name}")
            result = subprocess.run([TX_BIN, str(path)], capture_output=True)
            if result.returncode != 0:
                print(f"Error processing {path.name}: {result.stderr.decode()}")
                continue
            if result.stderr:
                print(result.stderr.decode())

            out.write(result.stdout)

    print(f"✅ Binary output written to {out_file}")

if __name__ == "__main__":
    main()
