TABLE = {}
FILE = open("gramatica", "r")

def parse_text():
    lines = FILE.readlines()
    currentLeftSide = 0
    for i, line in enumerate(lines):
        line = line.replace("<Tipo> ", "<Tipo>`SP`")
        line.replace(" ", "")
        newLine = line.split(" ::= ")
        lhs = newLine[0]
        rhs = newLine[1].replace("\n", "")
        if rhs[0:9] = "<Tipo> <I"
        TABLE[lhs] = []
        for attr in rhs.split(" | "):
            TABLE[lhs].append(attr)

def main():
    parse_text()
    for key in TABLE:
        print(TABLE[key])

main()