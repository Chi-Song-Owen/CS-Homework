import math

alphabet = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
text = 'DEAXTLICPUVRASMMSTPPJHTCEPNTHTPDTDEHMTHYUDTACJTGDLPCRDKTEGRCLVTGWWRDSSJTPCBWITRCVFXCDVRBPRPOCXQHTDJPMICVCYTXEQAHXHGZSIXKBLPIGUGPCCEPADQPOCDDKAIPALNITPZWWTPLTWTFHRSLYYEIWYAMPZCZAAAROIHEMZSXQJLOETPHTTHROEHTALNITPZDGPUHLBDQAUCXKHGXCYILTPKVUCIQVFEDULRIWCWOLTPKEBPLKIHCRNOXCEAOHAMDACNRPMTHMVNIWCHDKTLAOURPFPIDABRGTLJYPCBNECTPHTXKCHIXHAYEPIGUGPCCEPDCCUTXPJYIHTGUDTBYUDUDPTOGTBHTPRCUTTGQHNSBMYETACJTGXAPTNLCYELXRUEHHGUGPGCCOAJRPOCXLLLTRRYIRRYYSIGSJKHPLKAEEJPACRCZEATAARXRAVMEPLPEHLGSLWPTLTDBCLTIWCKEBPLKWWXJLMDKGUGJHRVWPGBHCPGZVNCTSARPAUVRAS'
frequency = [0.082, 0.015, 0.028, 0.043, 0.127, 0.022, 0.020, 0.061, 0.070, 0.002, 0.008, 0.040, 0.024, 0.067, 0.075, 0.019, 0.001, 0.060, 0.063, 0.091, 0.028, 0.010, 0.023, 0.001, 0.020, 0.001]

def triple_frequency_count(text):
    triple_frequency = {}

    for i in range(len(text) - 3):
        triple = text[i] + text[i + 1] + text[i + 2]
        if triple in triple_frequency:
            triple_frequency[triple] += 1
        else:
            triple_frequency[triple] = 1

    sorted_result = sorted(triple_frequency.items(), key=lambda x: x[1])

    for triple in sorted_result:
        print(triple[0], triple[1])

triple_frequency_count(text)

for i in range(len(text) - 3):
    triple = text[i] + text[i + 1] + text[i + 2]
    if triple == 'GUG':
        print(i)

key_length = 5
y1 = []
y2 = []
y3 = []
y4 = []
y5 = []

for i in range(len(text)):
    if i % key_length == 0:
        y1.append(text[i])
    elif i % key_length == 1:
        y2.append(text[i])
    elif i % key_length == 2:
        y3.append(text[i])
    elif i % key_length == 3:
        y4.append(text[i])
    elif i % key_length == 4:
        y5.append(text[i])

def frequency_count(text):
    frequency = {}

    for i in range(len(text)):
        if text[i] in frequency:
            frequency[text[i]] += 1
        else:
            frequency[text[i]] = 1

    sorted_result = dict(sorted(frequency.items(), key=lambda x: x[1]))
    return sorted_result

def cal_mg(yi):
    mg = [0] * 26

    sorted_result = frequency_count(yi)
    alphabet_frequency = []

    for i in range(26):
        if str(alphabet[i]) in sorted_result:
            alphabet_frequency.append(sorted_result[str(alphabet[i])])
        else:
            alphabet_frequency.append(0)

    for g in range(26):
        sum = 0.0
        for j in range(26):
            sum += frequency[j] * (alphabet_frequency[(j + g) % 26] / len(yi))
        mg[g] = round(sum, 4)

    print(mg)

cal_mg(y1)
cal_mg(y2)
cal_mg(y3)
cal_mg(y4)
cal_mg(y5)

key = 'HAPPY'

def decrypt(text, key):
    result = ''
    for i in range(len(text)):
        result += alphabet[(alphabet.index(text[i]) - alphabet.index(key[i % len(key)])) % 26]
    print(result.swapcase())

decrypt(text, key)