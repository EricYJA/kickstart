# All kinds of Package
import math
from functools import reduce, partial
from vector import vector_mean, distance, vector_and, vector_or
import random
import time 
from collections import Counter

# The route of Data
Data_Training = 'digit-training.txt'
Data_Testing = 'digit-testing.txt'
Data_Predicting = 'digit-predict.txt'

#########################
######__LOAD_DATA__######
#########################

def get_data(route):

    train_data = open(route)
    train_set = train_data.readlines()
    train_data.close()
    train_set = [i.replace('\n', '') for i in train_set]

    pointer = 0
    form_list = []
    mid_list = []
    while pointer < len(train_set):
        if len(train_set[pointer]) == 2:
            midset = ''.join(mid_list)
            del(mid_list[:])
            form_list.append(midset)
            form_list.append(train_set[pointer])
        else:
            mid_list.append(train_set[pointer])

        pointer += 1

    return form_list

def get_all(p_dataset):
    train_dict = dict()
    for i in range(len(p_dataset)):
        if len(p_dataset[i]) == 2 and train_dict.get(p_dataset[i], 0) == 0:
            train_dict[p_dataset[i]] = []
            train_dict[p_dataset[i]].append([int(i) for i in list(p_dataset[i-1])])
        elif len(p_dataset[i]) == 2 and train_dict.get(p_dataset[i], 0) != 0:
            train_dict[p_dataset[i]].append([int(i) for i in list(p_dataset[i-1])])

    return train_dict

train_dataset = get_all(get_data(Data_Training))
test_dataset = get_all(get_data(Data_Testing))
predict_dataset = get_all(get_data(Data_Predicting))


#########################
########__MODLE__########
#########################

def mean_model(p_dataset):
    keys_list = list(p_dataset.keys())
    for num in keys_list:
        p_dataset[num] = [vector_mean(p_dataset[num])]
    
    return p_dataset


def slowest_model(p_dataset):
    return p_dataset

def ran_model(p_dataset, ran_num):
    keys_list = list(p_dataset.keys())
    for num in keys_list:
        p_dataset[num] = random.sample(p_dataset[num], ran_num)
    
    return p_dataset

def and_model(p_dataset):
    keys_list = list(p_dataset.keys())
    for num in keys_list:
        p_dataset[num] = [reduce(vector_and, p_dataset[num])]

    return p_dataset

def or_model(p_dataset):
    keys_list = list(p_dataset.keys())
    for num in keys_list:
        p_dataset[num] = [reduce(vector_or, p_dataset[num])]

    return p_dataset


#########################
#########__KNN__#########
#########################

def K_NN(p_dataset, test_vector, n, flag):
    data_list = sorted(list(p_dataset.items()))
    judge_list = []
    for i in data_list:
        for j in i[1]:
            vec_dis = distance(j, test_vector)
            judge_list.append((vec_dis, i[0]))
    
    judge_list.sort()
    # For testing
    # print(len(judge_list))

    if flag == 'f':
        answer_list = []
        for m in range(n):
            answer_list.append(judge_list[m][1])

        return answer_list[0]
    elif flag == 'm':
        answer_list = []
        for m in range(n):
            answer_list.append(judge_list[m][1])

        con_list = Counter(answer_list)
        return con_list.most_common()[0][0]
    elif flag == 'a':
        answer_list = []
        answer_dict = {}
        for m in range(n):
            answer_list.append(judge_list[m])
        
        for a in answer_list:
            if answer_dict.get(a[1], 0) == 0:
                answer_dict[a[1]] = [a[0]]
            else:
                answer_dict[a[1]].append(a[0])
        
        answer_list = [(sum(answer_dict[k])/len(answer_dict[k]), k) for k in sorted(answer_dict.keys())]
        answer_list.sort()
        return answer_list[0][1]
        


#########################
######__SHOW_DATA__######
#########################

def show_train(p_dataset):
    data_list = [(k,len(p_dataset[k])) for k in sorted(p_dataset.keys())]
    print('Begining of Training @', \
    time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()))
    print('-'*40)
    print('Training Info'.center(40, ' '))
    print('-'*40)

    count_list = []
    for i in data_list:
        if i[1] < 100:
            print((str(i[0]) + ' =  ' + str(i[1])).rjust(22, ' '))
        else:
            print((str(i[0]) + ' = ' + str(i[1])).rjust(22, ' '))
        count_list.append(i[1])
        
    print('-'*40)
    print('  Total Samples =', sum(count_list))
    print('-'*40)
    # print('Ending of Training @', time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()))


def show_test(p_dataset, p_trainset, flag, k):
    # print('Begining of Testing @', time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()))
    print('-'*40)
    print('Testing Info'.center(40, ' '))
    print('-'*40)

    keys_list = list(p_dataset.keys())
    correct_list = []
    wrong_list = []
    total_list = []

    for num in keys_list:
        correct = 0
        wrong = 0
        for vec in p_dataset[num]:
            if K_NN(p_trainset, vec, k, flag) == num:
                correct += 1
            else:
                wrong += 1
        
        total_list.append(len(p_dataset[num]))
        correct_list.append(correct)
        wrong_list.append(wrong)

    combine_list = list(zip(keys_list, correct_list, wrong_list, total_list))
    combine_list.sort()

    for i in combine_list:
        num_acc = int(round(100*i[1]/i[3],0))
        print(str(i[0]).rjust(7) + ' = ' + str(i[1]).rjust(3) +\
         ', ' + str(i[2]).rjust(3) + ', ' + str(num_acc).rjust(4)+'%')

    print('-'*40)

    total_cor = sum([i[1] for i in combine_list])
    total_data = sum([i[3] for i in combine_list])
    total_acc = round(100*total_cor/total_data, 2)

    print('Accuracy'.rjust(18) + ' = ' + str(total_acc) + '%')
    print('Correct/Total'.rjust(18) + ' = ' + \
    str(total_cor) + '/' + str(total_data))

    print('-'*40)
    print('Ending of Training @', \
    time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()))


def predict_original(p_dataset, flag, k):
    print('The predictions are: ')
    for i in p_dataset[' 0']:
        print(K_NN(slowest_model(train_dataset), i, k, flag))

def predict_mean(p_dataset, flag, k):
    print('The predictions are: ')
    for i in p_dataset[' 0']:
        print(K_NN(mean_model(train_dataset), i, k, flag))
    
def predict_random(p_dataset, flag, k, ran_num):
    print('The predictions are: ')
    for i in p_dataset[' 0']:
        print(K_NN(ran_model(train_dataset, ran_num), i, k, flag))
    
def predict_and(p_dataset, flag, k):
    print('The predictions are: ')
    for i in p_dataset[' 0']:
        print(K_NN(and_model(train_dataset), i, k, flag))

def predict_or(p_dataset, flag, k):
    print('The predictions are: ')
    for i in p_dataset[' 0']:
        print(K_NN(or_model(train_dataset), i, k, flag))



##########################
#This is only for testing#
##########################

if __name__ == '__main__':
    print('''
This is a program based on KNN modle!!!
You have three options (original/mean/random/and/or). You can choose whatever you want among the three. Please note that there are different requiremens for different measure. 
About flag: This is a way of taking nearby points. It can only be m, f, a. (m: majority/f: first/a: average)
if you want to use original. Note that k must smaller than the total number.
if you want to use mean. Good for you. You choose the simplest one.
if you want to use random. Note that k must smaller than the total random sample number.
if you want to use (or) or (and). Good luck. They are really unreliable.
Start the exciting journey~~~
    ''')

    model = input('Please choose a modle: ')

    if model == 'original':
        k = int(input('Please choose how many nearest points you want to choose: '))
        flag = input('Please choose a flag (m/f/a): ')
        show_train(train_dataset)
        show_test(test_dataset, slowest_model(train_dataset), flag, k)
        predict_original(predict_dataset, flag, k)
    elif model == 'mean':
        k = 1
        flag = 'f'
        show_train(train_dataset)
        show_test(test_dataset, mean_model(train_dataset), flag, k)
        predict_mean(predict_dataset, flag, k)
    elif model == 'random':
        k = int(input('Please choose how many nearest points you want to choose: '))
        flag = input('Please choose a flag (m/f/a): ')
        ran_num = int(input('Please input how many random sample you want: '))
        show_train(train_dataset)
        show_test(test_dataset, ran_model(train_dataset, ran_num), flag, k)
        predict_random(predict_dataset, flag, k, ran_num)
    elif model == 'and':
        k = 1
        flag = 'f'
        show_train(train_dataset)
        show_test(test_dataset, and_model(train_dataset), flag, k)
        predict_and(predict_dataset, flag, k)
    elif model == 'or':
        k = 1
        flag = 'f'
        show_train(train_dataset)
        show_test(test_dataset, or_model(train_dataset), flag, k)
        predict_or(predict_dataset, flag, k)