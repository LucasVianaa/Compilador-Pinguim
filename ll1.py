import re

TABLE = {}
FILE = open("testegram", "r")
NEWFILE = open("output", "w")


def parse_text():
    lines = FILE.readlines()
    currentLeftSide = 0
    for i, line in enumerate(lines):
        line.replace(" ", "")
        newLine = line.split("::= ")
        lhs = newLine[0].replace(" ", "")
        rhs = newLine[1].replace("\n", "")

        TABLE[lhs] = []
        for attr in rhs.split(" | "):
            TABLE[lhs].append(attr.replace(' ', ''))


def first_rule(key):
    size = len(key)
    for element in TABLE[key]:
        if(key == element[0:size]):
            print("what")

def check_for_terminal(key):
    for element in TABLE[key]:
        if(element[0] == '<' and (element[1] != "=" and  element[1] != "<" and element[1] != ' ')):
            return False
    return True

def second_rule(key):
    change = False
    size = len(key)

    toRemove = []
    toAppend = []

    for element in TABLE[key]:
        if(element[0] == '<' and (element[1] != "=" and  element[1] != "<" and element[1] != ' ')):
            change = True
            nextKey = ""
            for char in element:
                nextKey += char
                if char == '>':
                    break
            if check_for_terminal(nextKey):
                toRemove.append(element)
                for appended in TABLE[nextKey]:
                    toAppend.append(appended + element[element.find(">")+1:])

    for item in toRemove:
        TABLE[key].remove(item)
    for item in toAppend:
        TABLE[key].append(item)
    return change

        
def factorizantion(key):
    repeat = {}
    for element in TABLE[key]:
        texto = ""
        for letter in element:
            if letter == '<':
                break
            texto += letter
        if texto != "":
            if texto in repeat:
                repeat[texto] += 1
            else:
                repeat[texto] = 1

    #for ele in repeat:
    #    print(key + " -> " + ele + " : " + str(repeat[ele]))


def main():
    parse_text()
    stop = True
    while stop == True:
        for key in TABLE:
            first_rule(key)
        #for key in TABLE:
            #factorizantion(key)
        for key in TABLE:
            if not second_rule(key):
                stop = False
            else:
                stop = True
        break
    for key in TABLE:
        NEWFILE.write(key + " ::= " + " | ".join(x for x in TABLE[key]) + "\n")
        

main()