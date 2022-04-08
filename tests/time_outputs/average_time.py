import sys


text_files = sys.argv[1:]
total_time = 0
for text_file in text_files:
    f = open(text_file, 'r')
    for line in f:
        s = line.split()
        if(len(s) > 0 and s[0] == 'real'):
            minuto = ''
            pont = 0
            while s[1][pont] != 'm':
                minuto += s[1][pont]
                pont += 1
            segundo = ''
            pont = pont + 1
            while s[1][pont] != 's':
                segundo += s[1][pont]
                if segundo[-1] == ',':
                    segundo = segundo[:-1]
                    segundo += '.'
                pont += 1
            total_time += float(minuto) * 60 + float(segundo)
            break
average_time = total_time / len(text_files)
print(average_time)