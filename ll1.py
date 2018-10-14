TABLE = {}
FILE = open("gramatica", "r")

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
            TABLE[lhs].append(attr)

def first_rule(key):
	size = len(key)
	for element in TABLE[key]:
		if(key == element[0:size]):
			print("found one")

def second_rule(key):
	print("TODO")	

def factorizantion(key):
	print("TODO")

def main():
    parse_text()
    for key in TABLE:
        first_rule(key)

main()