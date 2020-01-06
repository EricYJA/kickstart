def Get_words(route): 
    sen_file = open(route,'r')
    line = sen_file.readlines()
    sen_file.close()

    lines = [x.replace('\n', '') for x in line]
    for i in range(len(lines)):
        try:
            int(lines[i])
            lines.append(lines[i][:-3]+','+lines[i][-3:])
        except:
            lines[i] = lines[i]
    return lines

def Get_sentence(route):
    ori_file = open(route,'r')
    sentence_s = ori_file.readlines()
    p_sen = ''.join([i.replace('\n', '') for i in sentence_s])
    ori_file.close()
    p_sen = [i+'.\n' for i in p_sen.split('.')]
    p_sen.pop()
    return p_sen

def Change_sentence(ori_str):
    lines = Get_words('word.txt')
    symbol = ['"', '(', ')', '-', '\'', '.', ',', '!', '?']

    i = 0
    final_str = []
    while 0 <= i < len(ori_str):
        for j in range(i, len(ori_str))[::-1]:
            if ori_str[i:j].lower() in lines:
                final_str.append(ori_str[i:j])
                final_str.append(' ')
                break
            elif ori_str[i:j] in symbol:
                final_str.pop()
                final_str.append(ori_str[i:j])
                final_str.append(' ')
                break                
        i += len(final_str[-2])
    final = ''.join(final_str)
    if '"' in final: 
        final = final.replace('" ', '"')
    if '-' in final:
        final = final.replace('- ', '-')

    re_str = ori_str.replace('\n', '')
    m = len(re_str)
    final_str_re = []
    while 0 < m <= len(re_str):
        for n in range(0, m):
            if re_str[n:m].lower() in lines:
                final_str_re.append(re_str[n:m])
                final_str_re.append(' ')
                break
            elif re_str[n:m] in symbol:
                final_str_re.append(' ')
                final_str_re.append(re_str[n:m])
                break              
        m -= len(final_str_re[-2])
    final_str_re.pop()
    final_str_re.reverse()
    final_re = ''.join(final_str_re)
    if '"' in final_re: 
        final_re = final_re.replace('"  ', '"')
    if '-' in final_re:
        final_re = final_re.replace('-  ', '-')

    if len(final.replace(' ', '')) == len(ori_str.replace('\n', '')):
        return (final + '\n')
    elif len(final_re.replace(' ', '')) == len(ori_str.replace('\n', '')):
        return (final_re + '\n')
    else:
        return ori_str

def main():
    my_file = open('Sample_Output.txt','w+')
    for i in Get_sentence('sentence.txt'):
        my_file.write(Change_sentence(i))
        #this line is to print the answer in terminal
        # print(Change_sentence(i))
    my_file.close()

main()