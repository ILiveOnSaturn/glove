import threading
import pygame
import random
import json
from string import ascii_lowercase, ascii_uppercase
import serial

pygame.init()
WIDTH = 1200
HEIGHT = 700
win = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Glove Trainer")

clock = pygame.time.Clock()
SPECIAL_CHARS = "!@#$%^&*()_-+=/?<>\"', .;:1234567890[]{}`~↑↓→←↩"

term = serial.Serial("/dev/ttyACM0", 115200)
received_letter = False

glove_data = []
session_data = []

savefile_path = "data.json"
gthread = None


def save_data():
    global session_data
    new_data = [session_data[0]]
    print(session_data)
    for i in session_data[1:]:
        new_data.append([float(j) for j in i.split(',')])
    print(new_data)
    session_data = []
    file = open(savefile_path, 'a')
    file.write(json.dumps(new_data)+', ')
    file.close()


def read_term_thread():
    global received_letter
    global glove_data
    while True:
        term.read_until(b'<')
        data = term.read_until(b'>').decode()
        print(data)
        if data[-2] == '-':
            print("done")
            break
        glove_data = data[:-2].split('|')
        received_letter = True


def write(txt, x, y, font="arial", color=(0, 0, 0), size=30, aa=True, angle=0):
    temp = pygame.font.SysFont(font, size, True)
    temp = temp.render(txt, aa, color)
    temp = pygame.transform.rotate(temp, angle)
    win.blit(temp, (x, y))


def button(msg, x, y, w, h, ic, ac, font="arial", font_size=30, tcolor=(0, 0, 0), action=None, args=None):
    mouse = pygame.mouse.get_pos()
    click = pygame.mouse.get_pressed()
    if x + w > mouse[0] > x and y + h > mouse[1] > y:
        pygame.draw.rect(win, ac, (x, y, w, h))
        if click[0] == 1 and action is not None:
            if args is not None:
                action(args)
            else:
                action()
    else:
        pygame.draw.rect(win, ic, (x, y, w, h))

    font = pygame.font.SysFont(font, font_size, True)
    screen_text = font.render(msg, True, tcolor)
    win.blit(screen_text, (x - screen_text.get_rect().width / 2 + w / 2, y - screen_text.get_rect().height / 2 + h / 2))


def load_char_info():
    json_data = {"lower": dict.fromkeys(ascii_lowercase, 0), "upper": dict.fromkeys(ascii_uppercase, 0),
                 "other": dict.fromkeys(SPECIAL_CHARS, 0)}
    try:
        json_data = json.load(open("count.json", 'r'))
    except FileNotFoundError:
        outfile = open("count.json", 'x')
        json.dump(json_data, outfile)
    finally:
        return json_data


def save_char_info(info):
    file = open("count.json", 'w')
    json.dump(info, file)
    file.close()


def delete_last_entry():
    file = open(savefile_path, 'r')
    data = file.read()
    file.close()
    file = open(savefile_path, 'w')
    file.write(data[:data.rfind("\"")-3])
    file.close()


def load_pangrams(types, skip, loop=250):
    if types == "other":
        special_char = list(SPECIAL_CHARS)
        for i in range(loop-(skip//len(special_char))):
            random.shuffle(special_char)
            yield ''.join(special_char)
    else:
        j = skip
        for i in open("pangrams.txt", 'r'):
            if j >= len(i):  # NOTE: length of the sentence may be needed to be decremented by 1 (because of \n)
                j -= len(i)
                continue
            elif j > 0:
                yield i[j:]
                j = 0
                continue
            yield i


def train(train_type):
    global glove_data
    global received_letter
    global gthread
    global session_data
    char_count = load_char_info()
    data = load_pangrams(train_type, sum(char_count[train_type].values()))
    ch = 0
    sentence = next(data, None)[:-1]
    if sentence is None:
        print("Done")
        return
    if train_type == "upper":
        sentence = sentence.upper()
    elif train_type == "lower":
        sentence = sentence.lower()
    
    if gthread is None or not gthread.is_alive():
        gthread = threading.Thread(target=read_term_thread)
        gthread.start()
    
    while True:
        win.fill((255, 255, 255))
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                quit()
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:
                    return
                if event.key == pygame.K_BACKSPACE:
                    ch -= 1
                    if (train_type != "other") and (sentence[ch] in SPECIAL_CHARS):
                        char_count["other"][sentence[ch]] -= 1
                    else:
                        char_count[train_type][sentence[ch]] -= 1
                    delete_last_entry()

        if received_letter:
            session_data = [sentence[ch]]+glove_data
            print("saving")
            save_data()
            if (train_type != "other") and (sentence[ch] in SPECIAL_CHARS):
                char_count["other"][sentence[ch]] += 1
            else:
                char_count[train_type][sentence[ch]] += 1
            save_char_info(char_count)
            ch += 1
            received_letter = False
            if ch > len(sentence) - 1:
                ch = 0
                sentence = next(data, None)[:-1]
                if sentence is None:
                    print("Done")
                    break
                if train_type == "upper":
                    sentence = sentence.upper()
                elif train_type == "lower":
                    sentence = sentence.lower()

        if not gthread.is_alive():
            print("glove disconnected")
            return

        write(train_type, 10, 10, size=50)
        write(sentence[ch:], 10, 100, color=(100, 100, 100))

        clock.tick(25)
        pygame.display.update()


def menu():
    while True:
        win.fill((255, 255, 255))
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                quit()

        write("Select data type: ", 10, 10)

        button("Lowercase", 0, 100, WIDTH // 2, HEIGHT // 4 * 3 - 100, (255, 140, 0), (255, 165, 0), action=train,
               args="lower")
        button("Uppercase", WIDTH // 2, 100, WIDTH // 2, HEIGHT // 4 * 3 - 100, (90, 153, 155), (100, 163, 165),
               action=train, args="upper")
        button("Symbols and Numbers", 0, HEIGHT // 4 * 3, WIDTH, HEIGHT // 4, (128, 128, 128), (192, 192, 192),
               action=train, args="other")

        clock.tick(25)
        pygame.display.update()


menu()
