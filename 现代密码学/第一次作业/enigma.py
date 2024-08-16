import math

rotor_order = list(input("rotor order: "))

ring_setting = list(input("ring setting: "))
ring_setting = [ord(char) - 65 for char in ring_setting]

initial_position = list(input("initial position: "))
initial_position = [ord(char) - 65 for char in initial_position]
current_position = initial_position

turn_on_plug_board = input("turn on plug board? (y/n): ")
plug_board = {'B': 'X', 'X': 'B', 'G': 'K', 'K': 'G', 'W': 'Y', 'Y': 'W', 'E': 'F', 'F': 'E', 'P': 'Q', 'Q': 'P', 'S': 'N', 'N': 'S'}

carry_over = ['Q', 'E', 'V']

reflector = ['Y', 'R', 'U', 'H', 'Q', 'S', 'L', 'D', 'P', 'X', 'N', 'G', 'O', 'K', 'M', 'I', 'E', 'B', 'F', 'Z', 'C', 'W', 'V', 'J', 'A', 'T']
rotor_list = [
    ['E', 'K', 'M', 'F', 'L', 'G', 'D', 'Q', 'V', 'Z', 'N', 'T', 'O', 'W', 'Y', 'H', 'X', 'U', 'S', 'P', 'A', 'I', 'B', 'R', 'C', 'J'],
    ['A', 'J', 'D', 'K', 'S', 'I', 'R', 'U', 'X', 'B', 'L', 'H', 'W', 'T', 'M', 'C', 'Q', 'G', 'Z', 'N', 'P', 'Y', 'F', 'V', 'O', 'E'],
    ['B', 'D', 'F', 'H', 'J', 'L', 'C', 'P', 'R', 'T', 'X', 'V', 'Z', 'N', 'Y', 'E', 'I', 'W', 'G', 'A', 'K', 'M', 'U', 'S', 'Q', 'O']
]

low_level_rotor = int(rotor_order[2]) - 1
mid_level_rotor = int(rotor_order[1]) - 1
high_level_rotor = int(rotor_order[0]) - 1

def through_plug_board(text):
    content = []
    for char in text:
        if char in plug_board:
            char = plug_board[char]
        content.append(char)
    return content

def rotor_change(current_position, carry_over, low_level_rotor, mid_level_rotor):
    if current_position[1] == ord(carry_over[mid_level_rotor]) - 65:
        current_position[0] = (current_position[0] + 1) % 26
        current_position[1] = (current_position[1] + 1) % 26
    if current_position[2] == ord(carry_over[low_level_rotor]) - 65:
        current_position[1] = (current_position[1] + 1) % 26
    current_position[2] = (current_position[2] + 1) % 26

def rotor(char, rotor_order=rotor_order, ring_setting=ring_setting, current_position=current_position, reflector=reflector, rotor_list=rotor_list):
    for i in range(2, -1, -1):
        rotor_num = int(rotor_order[i]) - 1
        char = rotor_list[rotor_num][(ord(char) - 65 + current_position[i] - ring_setting[i]) % 26]
        char = chr((ord(char) - 65 - current_position[i] + ring_setting[i]) % 26 + 65)
    char = reflector[(ord(char) - 65) % 26]
    for i in range(0, 3, 1):
        rotor_num = int(rotor_order[i]) - 1
        char = rotor_list[rotor_num].index(chr((ord(char) - 65 + current_position[i] - ring_setting[i]) % 26 + 65))
        char = chr((char - current_position[i] + ring_setting[i]) % 26 + 65)
    return char

text = list(input("text: "))

def encrypt(text, rotor_order=rotor_order, ring_setting=ring_setting, current_position=current_position, turn_on_plug_board=turn_on_plug_board, plug_board=plug_board, carry_over=carry_over, reflector=reflector, rotor_list=rotor_list, low_level_rotor=low_level_rotor, mid_level_rotor=mid_level_rotor, high_level_rotor=high_level_rotor):

    if turn_on_plug_board == 'y' or turn_on_plug_board == 'Y':
        text = through_plug_board(text)

    cipher = ''
    for char in text:
        rotor_change(current_position, carry_over, low_level_rotor, mid_level_rotor)
        print(current_position)
        cipher += rotor(char, rotor_order, ring_setting, current_position, reflector, rotor_list)
    
    if turn_on_plug_board == 'y' or turn_on_plug_board == 'Y':
        cipher = ''.join(through_plug_board(cipher))

    return cipher

print(encrypt(text, rotor_order, ring_setting, current_position, turn_on_plug_board, plug_board, carry_over, reflector, rotor_list, low_level_rotor, mid_level_rotor, high_level_rotor))

def reset(initial):
    current = initial.copy()
    return current

def decrypt():
    ring_setting = [3, 4, 18]
    rotor_order = [[1, 2, 3], [1, 3, 2], [2, 1, 3], [2, 3, 1], [3, 1, 2], [3, 2, 1]]
    loops = {}
    for i in range(6):
        low_level_rotor = rotor_order[i][2] - 1
        mid_level_rotor = rotor_order[i][1] - 1
        high_level_rotor = rotor_order[i][0] - 1
        for j in range(26):
            for k in range(26):
                for l in range(26):
                    initial = [j, k, l]
                    current = initial.copy()
                    dictions = [{}, {}, {}]
                    for char in range(26):
                        text = chr(char + 65) * 6
                        cipher = encrypt(text, rotor_order[i], ring_setting, current, 'n', plug_board, carry_over, reflector, rotor_list, low_level_rotor, mid_level_rotor, high_level_rotor)
                        cipher = list(cipher)
                        current = reset(initial)
                        for m in range(3):
                            dictions[m][cipher[m]] = cipher[m + 3]
                    three_loop = []
                    for m in range(3):
                        one_loop = []
                        letter_in_loop = []
                        loop = []
                        for _ in range(26):
                            char = chr(_ + 65)
                            if char in letter_in_loop:
                                continue
                            letter_in_loop.append(char)
                            loop.append(char)
                            init = char
                            char = dictions[m][char]
                            while init != char:
                                loop.append(char)
                                letter_in_loop.append(char)
                                char = dictions[m][char]
                            length = len(loop)
                            one_loop.append(length)
                            loop = []
                        one_loop.sort()
                        three_loop.append(one_loop)
                    three_loop = tuple([tuple(sublist) for sublist in three_loop])
                    if tuple(three_loop) in loops:
                        loops[tuple(three_loop)].append((rotor_order[i].copy(), initial.copy()))
                    else:
                        loops[tuple(three_loop)] = [(rotor_order[i].copy(), initial.copy())]
    list1 = ['E', 'L', 'C', 'O', 'N', 'W', 'D', 'I', 'A', 'P', 'K', 'S', 'Z', 'H', 'F', 'B', 'Q', 'T', 'J', 'Y', 'R', 'G', 'V', 'X', 'M', 'U']
    list2 = ['M', 'R', 'W', 'J', 'F', 'D', 'V', 'S', 'Q', 'E', 'X', 'U', 'C', 'O', 'N', 'H', 'B', 'I', 'P', 'L', 'T', 'G', 'A', 'Y', 'Z', 'K']
    list3 = ['W', 'A', 'D', 'F', 'R', 'P', 'O', 'L', 'N', 'T', 'V', 'C', 'H', 'M', 'Y', 'B', 'J', 'Q', 'I', 'G', 'E', 'U', 'S', 'K', 'Z', 'X']
    dictions = [{}, {}, {}]
    for i in range(26):
        char = chr(i + 65)
        dictions[0][char] = list1[i]
        dictions[1][char] = list2[i]
        dictions[2][char] = list3[i]
    three_loop = []
    for m in range(3):
        one_loop = []
        letter_in_loop = []
        loop = []
        for _ in range(26):
            char = chr(_ + 65)
            if char in letter_in_loop:
                continue
            letter_in_loop.append(char)
            loop.append(char)
            init = char
            char = dictions[m][char]
            while init != char:
                loop.append(char)
                letter_in_loop.append(char)
                char = dictions[m][char]
            length = len(loop)
            one_loop.append(length)
            loop = []
        one_loop.sort()
        three_loop.append(one_loop)
    three_loop = tuple([tuple(sublist) for sublist in three_loop])
    print(loops[three_loop])
decrypt()

ring_setting = [[1, 0, 2], [1, 2, 0], [1, 2, 0]]
initial_position = [[16, 6, 19], [22, 21, 8], [23, 22, 8]]