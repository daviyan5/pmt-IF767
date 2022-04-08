import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np
import sys

mapa_sf = {}
th_name = []
th_val = []
test_type = sys.argv[1]
m_dist = 0
textfiles = []
if test_type == 'aprox':
    m_dist = int(sys.argv[2])
    textfiles = sys.argv[3:]
else:  
    textfiles = sys.argv[2:]
test_name = ''
test_f = False
for textfile in textfiles:
    f = open(textfile,'r')
    name = ""
    pointer = 0
    temp = ''
    while textfile[pointer] != '_':
        temp += textfile[pointer]
        pointer +=  1
    if not test_f:
        test_f = True
        test_name = temp
    pointer += 1
    th = False
    while (textfile[pointer] < '0' or textfile[pointer] > '9') and textfile[pointer] != '.':
        name += textfile[pointer]
        if name == "threads_":
            name = ""
            th = True
        pointer += 1

    if not th:
        if not name in mapa_sf:
            mapa_sf[name] = []
        mapa_sf[name].append(float(f.readline()))
    else:
        th_name.append(name)
        th_val.append(float(f.readline()))

if test_type == 'aprox':
    values = [i for i in range(m_dist+1)]
    i = 3
    fig = plt.figure()
    ax = fig.add_subplot(111)
    for name in mapa_sf:
        ax.plot(values,mapa_sf[name],label=name,marker = (i,0,0))
        i+=1
    ax.grid()
    ax.legend()
    ax.yaxis.set_major_formatter(ticker.FormatStrFormatter('%.1fs'))
    fig.savefig(test_name + '_aprox.png')

elif test_type == 'ext':
    values = ["50MB",'100MB','200MB','1GB','2GB']
    i = 3
    fig = plt.figure()
    ax = fig.add_subplot(111)
    for name in mapa_sf:
        ax.plot(values,mapa_sf[name],label=name,marker = (i,0,0))
        i+=1
    ax.grid()
    ax.legend()
    ax.yaxis.set_major_formatter(ticker.FormatStrFormatter('%.1fs'))
    fig.savefig(test_name + '_sg.png')

    hist = plt.figure()
    ax = hist.add_subplot(111)
    x_coordinates = np.arange(len(th_name))
    ax.bar(th_name, th_val, align='center')
    ax.xaxis.set_major_locator(plt.FixedLocator(x_coordinates))
    ax.xaxis.set_major_formatter(plt.FixedFormatter(th_name))
    ax.yaxis.set_major_formatter(ticker.FormatStrFormatter('%.1fs'))

    ax.grid()
    hist.savefig(test_name + '_th.png')
else:
    values = [1,5,25,125,250,500]
    i = 3
    fig = plt.figure()
    ax = fig.add_subplot(111)
    for name in mapa_sf:
        ax.plot(values,mapa_sf[name],label=name,marker = (i,0,0))
        i+=1
    ax.grid()
    ax.legend()
    ax.yaxis.set_major_formatter(ticker.FormatStrFormatter('%.1fs'))
    fig.savefig(test_name + '_aprox.png')