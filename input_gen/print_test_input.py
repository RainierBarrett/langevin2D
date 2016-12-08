import numpy as np
x = np.arange(-5,5.1,0.1)
y = np.arange(-5,5.1,0.1)
f = open('./test_input.txt', 'w+')
for i in range(len(x)):
    for j in range(len(y)):
        value = "{} \t {} \t 0 \t 0\n".format(x[i] if abs(x[i]) > 0.0001 else 0, y[j] if abs(y[j]) > 0.0001 else 0)
        f.write(value)
