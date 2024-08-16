import math
from colorama import init, Fore, deinit

text = 'AZLTBSUZWTCLBCDQFDSKASGDFWOGARTCDZFBSTZSBDABFDAZDBFYDUWOBCSQFWOGFSKLDWZFWUSQOBREZDADOBGDAWGDFRBCWFYDDOBCDFBSZLSKMCWBIRUCBYDAWEEDGAEWFFRAWEAZLTBSUZWTCLBCWBRFSKIDBCSGFSKDOAZLTBRSOBCWBQFDTDOWOGTWTDZSZTDZCWTFFRITEDIDACWORAWEWRGFROBCDDWZELBMDOBRDBCADOBQZLBCDROPDOBRSOSKASITEDJIDACWORAWEWOGDEDABZSIDACWORAWEIWACRODFFQACWFBCDDORUIWZSBSZIWACRODTZSPRGDGISZDFSTCRFBRAWBDGWOGDKKRARDOBIDWOFSKDOAZLTBRSOWOGBCDFQYFDVQDOBROBZSGQABRSOSKDEDABZSORAFWOGASITQBROUCWFWEESMDGDEWYSZWBDFACDIDFSKFBREEUZDWBDZASITEDJRBLISFBSKMCRACWZDDOBRZDELQOFQRBDGBSTDOWOGTWTDZBCDGDPDESTIDOBSKAZLTBSUZWTCLCWFYDDOTWZWEEDEDGYLBCDGDPDESTIDOBSKAZLTBWOWELFRFBCWBRFBCDYZDWNROUSKASGDFWOGARTCDZFBCDGRFASPDZLWOGWTTERAWBRSODWZELSOSKKZDVQDOALWOWELFRFBSBCDZDWGROUSKDOAZLTBDGASIIQORAWBRSOFCWFSOSAAWFRSOWEBDZDGBCDASQZFDSKCRFBSZL'
alphabet = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'

connect = {}

def print_connected_text(text):
    for i in text:
        if i in connect:
            print(Fore.RED + connect[i], end='')
        else:
            print(Fore.RESET + i, end='')

def letter_frequency_count(text):
    letter_frequency = {}

    for letter in text:
        if letter in alphabet:
            if letter in letter_frequency:
                letter_frequency[letter] += 1
            else:
                letter_frequency[letter] = 1

    sorted_result = sorted(letter_frequency.items(), key=lambda x: x[1])

    for letter in sorted_result:
        print(letter[0], letter[1])

letter_frequency_count(text)

connect['A'] = 'c'
connect['D'] = 'e'

def double_frequency_count(text):
    double_frequency = {}

    for i in range(len(text) - 2):
        if text[i] in alphabet and text[i + 1] in alphabet and text[i + 2] == 'D':
            double = text[i] + text[i + 1]
            if double in double_frequency:
                double_frequency[double] += 1
            else:
                double_frequency[double] = 1

    sorted_result = sorted(double_frequency.items(), key=lambda x: x[1])

    for double in sorted_result:
        print(double[0], double[1])

double_frequency_count(text)

connect['B'] = 't'
connect['C'] = 'h'

def triple_frequency_count(text):
    triple_frequency = {}

    for i in range(len(text) - 3):
        if text[i] in alphabet and text[i + 1] in alphabet and text[i + 2] in alphabet:
            triple = text[i] + text[i + 1] + text[i + 2]
            if triple in triple_frequency:
                triple_frequency[triple] += 1
            else:
                triple_frequency[triple] = 1

    sorted_result = sorted(triple_frequency.items(), key=lambda x: x[1])

    for triple in sorted_result:
        print(triple[0], triple[1])

triple_frequency_count(text)

connect['W'] = 'a'
connect['O'] = 'n'
connect['G'] = 'd'

connect['Z'] = 'r'
connect['F'] = 's'

connect['S'] = 'o'
connect['Q'] = 'u'
connect['T'] = 'p'

connect['R'] = 'i'
connect['E'] = 'l'

connect['Y'] = 'b'
connect['L'] = 'y'
connect['U'] = 'g'
connect['K'] = 'f'

connect['I'] = 'm'
connect['M'] = 'w'

connect['P'] = 'v'
connect['J'] = 'x'
connect['V'] = 'q'
connect['N'] = 'k'

print_connected_text(text)