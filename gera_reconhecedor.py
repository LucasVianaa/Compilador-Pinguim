TABLE = {}
FILE = open("Gramatica", "r")
NEWFILE = open("output_recon", "w")
CODE = ""

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

def number_of_chars_terminal(element):
    count = 0
    if element[0:5] == "List<":
        return 5
    elif element[0:2] == "<<":
        return 1
    elif element[0:2] == "<=":
        return 2
    else:
        for char in element:
            if char != "<":
                count += 1
            else:
                break
    return count

#Template para método 
'''
resultType nome_derivação(string original_text, int line, int column){
    
    ResultType aux = trim(original_text, line, column);
    string text = aux.getText(); int line = aux.getLine(); int column = aux.getColumn();

    //corpo

    return resultType(Parametros); // se vazio true e se falso invalid expression
}

'''
def find_next_non_terminal(text):
    result = ""
    for x in text:
        if x == '>':
            break
        elif x != '<':
            result += x
    return result

def generate_element(value):
    num_term = number_of_chars_terminal(value)
    print(find_next_non_terminal(value[num_term:]))
    
    return ""



def generate(key):
    global CODE 
    CODE += "ResultType " + key.replace("-", "_").replace("<", "").replace(">","").lower() + "(string text_original, int line, int column){\n"
    for value in TABLE[key]:
        CODE += generate_element(value)
    CODE += "return ResultType(true, text, line, column)\n"
    CODE += "}\n"

def main():
    parse_text()
    for key in TABLE:
        generate(key)
main()