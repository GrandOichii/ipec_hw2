import matplotlib.pyplot as plt

data = open('results.txt', 'r').read()
split = data.split('\n===\n')
nums = []
times = []
for s in split:
    if s == '': continue
    ss = s.split('\n')
    num = int(ss[0])
    nums += [num]
    # print(ss[0])
    ts = ss[2][5:-1]
    m, s = ts.split('m')
    time = float(m * 60 + s)
    times += [time]
    
plt.plot(nums, times)
plt.ylabel('Time')
plt.xlabel('# of threads')
plt.scatter(nums, times)
plt.show()