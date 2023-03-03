import json
from random import shuffle

file = open("count.json", 'r')
count = json.load(file)
file.close()

count = count["lower"]

max_value = 0
content = ""

FILENAME = "fill_in_text.txt"


def shuffle_str(string):
    string = list(string)
    shuffle(string)
    return ''.join(string)


for i in count.values():
    max_value = max(max_value, i)

for i in count:
    count[i] = abs(max_value-count[i])

while sum(count.values()) > 0:
    str_temp = ""
    for i in count:
        if count[i] > 0:
            str_temp += i
            count[i] -= 1
    content += shuffle_str(str_temp)+'\n'

print(content)
file = open(FILENAME, 'w')
file.write(content)
file.close()
