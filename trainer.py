import pygame
import random
from string import ascii_lowercase

pygame.init()
WIDTH = 1200
HEIGHT = 700
win = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Glove Trainer")

clock = pygame.time.Clock()


def write(txt, x, y, font="arial", color=(0, 0, 0), size=30, aa=True, angle=0):
    temp = pygame.font.SysFont(font, size, True)
    temp = temp.render(txt, aa, color)
    temp = pygame.transform.rotate(temp, angle)
    win.blit(temp, (x, y))


def button(msg, x, y, w, h, ic, ac, font="arial", fontSize=30, tcolor=(0, 0, 0), action=None, args=None):
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

    font = pygame.font.SysFont(font, fontSize, True)
    screen_text = font.render(msg, True, tcolor)
    win.blit(screen_text, (x - screen_text.get_rect().width / 2 + w / 2, y - screen_text.get_rect().height / 2 + h / 2))


def load_pangrams(types, loop=250):
	if types == "other":
		special_char = list("!@#$%^&*()_-+=/?<>\"',.;:1234567890[]{}`~")
		for i in range(250):
			shuffle(special_char)
			yield ''.join(special_char)
	else:
		for i in open("pangrams.txt", 'r'):
			yield i


def train(train_type):
	if train_type != "other":
		data = load_pangrams(train_type)
		if train_type == "upper":
			data = data.upper()
		data = data[:-1].split('\n')
	#else:
		#TODO add randomized data

	while True:
		win.fill((255, 255, 255))
		for event in pygame.event.get():
			if event.type == pygame.QUIT:
				pygame.quit()
				quit()
			if event.type == pygame.KEYDOWN:
				if event.key == pygame.K_ESCAPE:
					return
		
		if (len(data) > 0):
			sentence = random.choice(data)
			data.remove(sentence)
		
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
		
		button("Lowercase", 0, 100, WIDTH//2, HEIGHT//4*3-100, (255,140,0), (255, 165, 0), action=train, args="lower")
		button("Uppercase", WIDTH//2, 100, WIDTH//2, HEIGHT//4*3-100, (90,153,155), (100,163,165), action=train, args="upper")
		button("Symbols and Numbers", 0, HEIGHT//4*3, WIDTH, HEIGHT//4, (128, 128, 128), (192, 192, 192), action=train, args="other")
		
		clock.tick(25)
		pygame.display.update()


menu()