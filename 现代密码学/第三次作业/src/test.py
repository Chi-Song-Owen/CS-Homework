R = [0b11100001]
for _ in range(15):
    R.append(0)
# print(R)

def getbit(byte: int, index: int) -> bool:
    a = 1 << (7 - index)
    # print(a)
    return byte & a == a

def xor(X: list, Y: list, len: int) -> list:
    Z = []
    for i in range(len):
        Z.append(X[i] ^ Y[i])
    return Z

def sr(X: list) -> list:
    Z = []
    Z.append(X[0] >> 1)
    for i in range(1, 16):
        if getbit(X[i - 1], 7):
            Z.append((X[i] >> 1) + 0b10000000)
        else:
            Z.append(X[i] >> 1)
    return Z


def multi(X: list, Y: list) -> list:
    print(X)
    # print(Y)
    Z = []
    for _ in range(16):
        Z.append(0)
    V = Y
    print(Z)
    print(V)
    for i in range(128):
        a = False

        if (getbit(X[i >> 3], i % 8)):
            Z = xor(Z, V, 16)
        if getbit(V[15], 7):
            a = True
            V = xor(sr(V), R, 16)
        else:
            V = sr(V)
        if (i < 30):
            print(i)
            print(a)
            print(Z)
            print(V)
    return Z
    # print(Z)

def ghash(X: list, H: list, len: int) -> list:
    Y = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
    for i in range(len):
        # print(X[i * 16: i * 16 + 16])
        # print(Y)
        temp = xor(Y, X[i * 16: i * 16 + 16], 16)
        # print(temp)
        Y = multi(temp, H)
    return Y

# print(xor(R, [2,3,5], 3))
# multi(R,R)
X = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0b00000001]
Y = [0b10000000,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]

H = [0xB8, 0x3B, 0x53, 0x37, 0x08, 0xBF, 0x53, 0x5D, 0x0A, 0xA6, 0xE5, 0x29, 0x80, 0xD5, 0x3B, 0x78]

C = [0x42, 0x83, 0x1E, 0xC2, 0x21, 0x77, 0x74, 0x24, 0x4B, 0x72, 0x21, 0xB7, 0x84, 0xD0, 0xD4, 0x9C, 
0xE3, 0xAA, 0x21, 0x2F, 0x2C, 0x02, 0xA4, 0xE0, 0x35, 0xC1, 0x7E, 0x23, 0x29, 0xAC, 0xA1, 0x2E, 
0x21, 0xD5, 0x14, 0xB2, 0x54, 0x66, 0x93, 0x1C, 0x7D, 0x8F, 0x6A, 0x5A, 0xAC, 0x84, 0xAA, 0x05, 
0x1B, 0xA3, 0x0B, 0x39, 0x6A, 0x0A, 0xAC, 0x97, 0x3D, 0x58, 0xE0, 0x91, 0x47, 0x3F, 0x59, 0x85]

C1 = [0x42, 0x83, 0x1E, 0xC2, 0x21, 0x77, 0x74, 0x24, 0x4B, 0x72, 0x21, 0xB7, 0x84, 0xD0, 0xD4, 0x9C, 
0xE3, 0xAA, 0x21, 0x2F, 0x2C, 0x02, 0xA4, 0xE0, 0x35, 0xC1, 0x7E, 0x23, 0x29, 0xAC, 0xA1, 0x2E, 
0x21, 0xD5, 0x14, 0xB2, 0x54, 0x66, 0x93, 0x1C, 0x7D, 0x8F, 0x6A, 0x5A, 0xAC, 0x84, 0xAA, 0x05, 
0x1B, 0xA3, 0x0B, 0x39, 0x6A, 0x0A, 0xAC, 0x97, 0x3D, 0x58, 0xE0, 0x91, 0x47, 0x3F, 0x59, 0x85,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0]

target = [0x7F, 0x1B, 0x32, 0xB8, 0x1B, 0x82, 0x0D, 0x02, 0x61, 0x4F, 0x88, 0x95, 0xAC, 0x1D, 0x4E, 0xAC]
# print(sr(R))
# print(multi(X,X))
# print(multi(Y,Y))
# print(multi(X,Y))
# print(target)
print(ghash(C1, H, 5))
# for i in range(8):
#     print(getbit(0b11100001, i))