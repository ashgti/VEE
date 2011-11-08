import random
import numpy as np
import matplotlib.pyplot as plt

fig = plt.figure()

max_a = 3.4
duration = 1
phase = 5.0

x = np.arange(0, 100, 0.1)
y = np.copy(x)

i = 0
last_start = 0
cur_period = random.expovariate(1.0 / phase)
print 'cur:', cur_period
for i, v in enumerate(x):
    if v > last_start + cur_period + duration:
        last_start = v
        cur_period = random.expovariate(1.0 / phase)
        print 'cur:', cur_period

    if v > last_start + cur_period:
        y[i] = max_a
    else:
        y[i] = np.sin((1.0/cur_period)/2.0 * np.pi * (v - last_start)) * max_a

ax = fig.add_subplot(111)
ax.plot(x, y)
ax.set_xlim(-0.5, 10)
ax.set_ylim(-0.25, 4)

plt.show()
