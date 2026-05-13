def cardScore(card):
    mp1 = {'h': 3, 's': 2, 'd': 1, 'c': 0}
    bonus = mp1[card[0]]
    mp2 = {'2': 15, 'A': 14, 'K': 13, 'Q': 12, 'J': 11}
    value = mp2[card[1]] if card[1] in mp2 else int(card[1:])
    return value * 4 + bonus

while True:
    try:
        line = input()
    except EOFError:
        break
    cards = line.split()
    cards.sort(key=cardScore, reverse=True)
    print(' '.join(cards))