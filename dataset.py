import string
import random

numDataPoints = 1000
maxDistance = 50
letters = string.ascii_letters + ' '
cuisines = ['American', 'French', 'Mexican', 'Italian', 'Japanese', 'Indian', 'Chinese', 'African', 'Lebanese', 'Kosher', 'Thai', 'Spanish', 'Cuban']
f = open('dataset.txt', 'w')

for i in range(0, numDataPoints):
    s = "'" + ''.join(random.choice(letters) for i in range(10)) + "'"
    s = s + ' ' + str(round(random.uniform(0, maxDistance), 2)) + ' ' + str(random.randint(0, 5)) + ' ' + str(random.choice(cuisines)) + "\n"
    f.write(s)
