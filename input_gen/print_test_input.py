x = [i*0.1 for i in range(-50, 50)]
y = [i*0.1 for i in range(-50, 50)]
f = open('./test_input.txt', 'w+')
value = "{} {}\n".format(len(x), len(y))
f.write(value)
for i in range(len(x)):
    for j in range(len(y)):
        value = "{:.2f} \t {:.2f} \t 0 \t 0\n".format(x[i] if abs(x[i]) > 0.0001 else 0, y[j] if abs(y[j]) > 0.0001 else 0)
        f.write(value)
