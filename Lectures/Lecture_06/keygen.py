import random

def find_large_prime():
    is_prime = lambda n: all(n % i != 0 for i in range(2, int(n ** 0.5) + 1))
    candidate = 6 * random.randint(5460,10000) + 1
    while not is_prime(candidate):
        candidate += 6
    return candidate

def egcd(a, b):
    if a == 0:
        return (b, 0, 1)
    else:
        g, y, x = egcd(b % a, a)
        return (g, x - (b // a) * y, y)
    
lcm = lambda a, b: a * b // egcd(a, b)[0]
p, q = find_large_prime(), find_large_prime()
n = p * q
e = 65537
lmdb = lcm(p - 1,  q - 1)
d = egcd(e, lmdb)[1]
while d < 0: # fix sign problem
    d += lcm(p - 1,  q - 1)
hd = "-----BEGIN"
ft = "-----END"
tl = " UNSAFE PRIVATE KEY-----\n"
open("unsafe.bad", "w").write(f"{hd}{tl}{n:x}\n{e:x}\n{d:x}\n{ft}{tl}")
tl = " UNSAFE PUBLIC KEY-----\n"
open("unsafe.pub", "w").write(f"{hd}{tl}{n:x}\n{e:x}\n{ft}{tl}")
