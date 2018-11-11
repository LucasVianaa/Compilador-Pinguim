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
#∅

def find_next_non_terminal(text):
    result = ""
    for x in text:
        if x == '>':
            break
        elif x != '<':
            result += x
    return result

def generate_element(value):
    num_term = number_of_chars_terminal(value.replace("`SP`", " "))
    to_insert = ""
    countIf = 0
    while value != "":
        num_term = number_of_chars_terminal(value.replace("`SP`", " "))
        next_non_terminal = ""
        if value != "∅":
            next_non_terminal = find_next_non_terminal(value.replace("`SP`", " ").replace("-", "_")[num_term:].lower())
            countIf += 1
            to_insert += "    if(text.length() > "
            if value.replace("`SP`", " ")[0:num_term] == "[A-z]":
                to_insert += "1"  + " && isalpha(str[0])){\n"
            elif value.replace("`SP`", " ")[0:num_term] == "[0-9]":
                to_insert += "1"  + " && isdigit(str[0])){\n"
            elif value.replace("`SP`", " ")[0:num_term] == "[A-z 0-9?!+-/*=:[]]":
                to_insert += "1"  +" && istext(str[0])){\n"
            else:
                to_insert += str(num_term)  +" && str.substr(0, "+str(num_term)+") == \""+value.replace("`SP`", " ")[0:num_term]+"\"){\n"
            to_insert += "        result = " + next_non_terminal + "(text.substr("+str(num_term)+"), line, column);\n"
            to_insert += "        if(!result.getAccept()){\n"
            to_insert += "            return result;\n"
            to_insert += "        }\n"
            if(value.replace("`SP`", " ")[(num_term + len(next_non_terminal) + 2):] != ""):
                to_insert += "        trimmed = trim(result.getText(), result.getLine(), result.getColumn());\n\
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();\n"
            to_insert += "    }\n"
        else:
            to_insert += "    return ResultType(true, text, l, c); \n"
        print(value)
        value = value.replace("`SP`", " ")[(num_term + len(next_non_terminal) + 2):]
        #for i in range(0, countIf):
        #    to_insert += "    }\n"
    #print(to_insert)
    #print(find_next_non_terminal(value[num_term:]))
    return to_insert

def generate_layout():
    to_insert = ""
    to_insert += "    ResultType trimmed = trim(text_original, l, c);\n\
    ResultType result(true, trimmed.getText(), trimmed.getLine(), trimmed.getColumn());\n\
    string text = trimmed.getText(); int line = trimmed.getLine(); int column = trimmed.getColumn();\n"
    return to_insert


def generate(key):
    global CODE 
    CODE += "ResultType " + key.replace("-", "_").replace("<", "").replace(">","").lower() + "(string text_original, int line, int column){\n"
    CODE += generate_layout()
    for value in TABLE[key]:
        CODE += generate_element(value)
    CODE += "}\n"


def main():
    parse_text()
    for key in TABLE:
        generate(key)
    #print(CODE)
    NEWFILE.write(CODE)
main()