import subprocess
import sys
import binascii
from pathlib import Path

SHA_PROGRAM = "./sha"
RSA_PROGRAM = "./rsa"

TEMP_TX_FILE = "temp_tx_for_hash.txt"
TEMP_SIG_BIN = "temp_sig.bin"
TEMP_DECRYPTED_BIN = "temp_decrypted.bin"

PUB_KEYS = {
    "bear": "bear_keys/unsafe.pub",
    "shadow": "shadow_keys/unsafe.pub"
}

def parse_transaction_file(path):
    with open(path, "r") as f:
        lines = f.readlines()

    text = None
    sig_lines = []
    section = None
    for line in lines:
        line = line.strip()
        if line == "Transaction:":
            section = "text"
            continue
        elif line == "Transaction Signature:":
            section = "sig"
            continue
        elif line == "Transaction Hash:":
            section = "skip"
            continue

        if section == "text" and not text:
            text = line
        elif section == "sig":
            sig_lines.append(line)

    if not text or not sig_lines:
        raise ValueError("Missing transaction text or signature")

    sig_hex = "".join(sig_lines)
    return text, sig_hex

def run_sha_on_text(text):
    with open(TEMP_TX_FILE, "w") as f:
        f.write(text + "\n")  
    result = subprocess.run([SHA_PROGRAM, TEMP_TX_FILE], capture_output=True, text=True)
    return result.stdout.strip()


def decrypt_signature(sig_hex, pubkey_path):
    with open(TEMP_SIG_BIN, "wb") as f:
        f.write(bytes.fromhex(sig_hex))

    subprocess.run([RSA_PROGRAM, "-e", TEMP_SIG_BIN, TEMP_DECRYPTED_BIN, pubkey_path], check=True)

    with open(TEMP_DECRYPTED_BIN, "rb") as f:
        return f.read().hex()

def main():
    if len(sys.argv) != 3:
        print("Usage: python3 verify_transaction.py <block_transaction.txt> <bear|shadow>")
        sys.exit(1)

    tx_path = Path(sys.argv[1])
    signer = sys.argv[2]
    if signer not in PUB_KEYS:
        print(f"❌ Unknown signer: {signer}")
        sys.exit(1)

    pubkey_path = PUB_KEYS[signer]

    try:
        text, sig_hex = parse_transaction_file(tx_path)
        recomputed_hash = run_sha_on_text(text)
        decrypted_hex = decrypt_signature(sig_hex, pubkey_path)

        print("--- Debug Info ---")
        print(f"📝 Transaction Text: {text}")
        print(f"🔒 Recomputed Hash:  {recomputed_hash}")
        print(f"🔓 Decrypted Hash:   {decrypted_hex[-64:]}")  # Only show trailing 32 bytes

        if decrypted_hex.endswith(recomputed_hash):
            print("✅ Signature is VALID")
        else:
            print("❌ Signature is INVALID")

    finally:
        for temp in [TEMP_TX_FILE, TEMP_SIG_BIN, TEMP_DECRYPTED_BIN]:
            Path(temp).unlink(missing_ok=True)

if __name__ == "__main__":
    main()
