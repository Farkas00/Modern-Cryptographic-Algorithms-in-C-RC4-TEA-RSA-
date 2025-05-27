#  Security and Cryptography Project – RC4, TEA, RSA (C)

This C project implements three cryptographic algorithms from scratch, without using any cryptographic libraries.

##  Features

- **RC4** – Stream cipher (byte-by-byte encryption)
- **TEA (Tiny Encryption Algorithm)** – Symmetric 64-bit block cipher with a 128-bit key
- **RSA** – Asymmetric encryption for small files using modular exponentiation

Each algorithm supports:
- File-based encryption and decryption
- Command-line usage
- Manual key file input
- Output file generation

---

## 🧪 Tested With

-  Files up to 3.4 GB (RC4, TEA)
-  RSA tested with small text files
-  RSA is not used for large files (by design)




# ---------------------------------------------------------
# RC4 – Stream cipher (works with ANY size, tested up to 3.4GB)
# ---------------------------------------------------------

# Encrypt the 3.4GB video file using RC4
./crypto rc4 -e test_3GB.mp4 -k key.bin -o rc4_3gb_enc.bin
# Decrypt it back
./crypto rc4 -d rc4_3gb_enc.bin -k key.bin -o rc4_3gb_dec.mp4
# Check if original and decrypted are identical
diff test_3GB.mp4 rc4_3gb_dec.mp4

# Repeat for 2GB file
./crypto rc4 -e test2.mp4 -k key.bin -o rc4_2gb_enc.bin
./crypto rc4 -d rc4_2gb_enc.bin -k key.bin -o rc4_2gb_dec.mp4
diff test2.mp4 rc4_2gb_dec.mp4

# Repeat for 800MB file
./crypto rc4 -e f800.mp4 -k key.bin -o rc4_800mb_enc.bin
./crypto rc4 -d rc4_800mb_enc.bin -k key.bin -o rc4_800mb_dec.mp4
diff f800.mp4 rc4_800mb_dec.mp4

# Test RC4 on small text file
./crypto rc4 -e message.txt -k key.bin -o rc4_msg_enc.bin
./crypto rc4 -d rc4_msg_enc.bin -k key.bin -o rc4_msg_dec.txt
diff message.txt rc4_msg_dec.txt


# ---------------------------------------------------------
# TEA – Block cipher (64-bit blocks, now handles any size, up to 4GB)
# ---------------------------------------------------------

# Encrypt the 3.4GB video file using TEA
./crypto tea -e test_3GB.mp4 -k key.bin -o tea_3gb_enc.bin
# Decrypt it back
./crypto tea -d tea_3gb_enc.bin -k key.bin -o tea_3gb_dec.mp4
# Verify match
diff test_3GB.mp4 tea_3gb_dec.mp4

# Repeat for 2GB file
./crypto tea -e test2.mp4 -k key.bin -o tea_2gb_enc.bin
./crypto tea -d tea_2gb_enc.bin -k key.bin -o tea_2gb_dec.mp4
diff test2.mp4 tea_2gb_dec.mp4

# Repeat for 800MB file
./crypto tea -e f800.mp4 -k key.bin -o tea_800mb_enc.bin
./crypto tea -d tea_800mb_enc.bin -k key.bin -o tea_800mb_dec.mp4
diff f800.mp4 tea_800mb_dec.mp4

# Test TEA on small text file
./crypto tea -e message_tea.txt -k key.bin -o tea_msg_enc.bin
./crypto tea -d tea_msg_enc.bin -k key.bin -o tea_msg_dec.txt
diff message_tea.txt tea_msg_dec.txt


# ---------------------------------------------------------
# RSA – Only for SMALL files (less than ~1MB), asymmetric encryption
# ---------------------------------------------------------

# Encrypt a small text file with RSA
./crypto rsa -e message_rsa.txt -k rsa_key.txt -o rsa_enc.bin
# Decrypt it back
./crypto rsa -d rsa_enc.bin -k rsa_key.txt -o rsa_dec.txt
# Verify
diff message_rsa.txt rsa_dec.txt

# Try another small file (message.txt)
./crypto rsa -e message.txt -k rsa_key.txt -o rsa_msg_enc.bin
./crypto rsa -d rsa_msg_enc.bin -k rsa_key.txt -o rsa_msg_dec.txt
diff message.txt rsa_msg_dec.txt

# DO NOT USE RSA for large files like test_3GB.mp4 — explain why in your presentation
