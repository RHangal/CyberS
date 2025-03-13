def modexp(m, e, n):
    if e == 0:
        return 1
    if e == 1:
        return m % n
    if e % 2:
        return (m * modexp(m*m % n, e//2, n)) % n
    return  modexp(m*m % n, e//2, n) % n

make_encrypt = lambda e, n : lambda m : modexp(m,e,n)

lines = open("unsafe.bad").readlines()
n = int(lines[1], 16) 
e = int(lines[2], 16) 
d = int(lines[3], 16) 

lines = open("unsafe.pub").readlines()
if n != int(lines[1], 16) or e != int(lines[2], 16):
    print("Public key does not match private key.")
    exit()

encrypt = make_encrypt(e,n)
decrypt = make_encrypt(d,n)

if all([i == decrypt(encrypt(i)) for i in range(0x100)]):
    print("Keys work for small values.")
else:
    print("Keys failed on small values.")

if all([i == decrypt(encrypt(i)) for i in range(0,1 << 31, 1 << 29)]):
    print("Keys work for larger values.")
else:
    print("Keys failed on larger values.")
