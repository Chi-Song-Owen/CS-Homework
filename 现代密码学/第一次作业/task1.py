import math

alphabet = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
text = 'QGMEMKLTWLZWUZSFYWQGMOAKZLGKWWAFLZWOGJDV'

def decrypt(text, key):
    decrypted = ''
    for letter in text:
        if letter in alphabet:
            decrypted += alphabet[(alphabet.index(letter) - key) % 26]
        else:
            decrypted += letter
    print(key, decrypted)

for i in range(0, 26):
    decrypt(text, i)