TABLE = {}
FILE = open("Gramatica", "r")
NEWFILE = open("why", "w")
CODE = ""
COUNTIF = 0

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

def number_of_chars_terminal(element):
    count = 0
    lastChar = ''
    if element[0:5] == "List<":
        return 5
    elif element[0:2] == "< ":
        return 1
    elif element[0:2] == "<=":
        return 2
    elif element[0:4] == " || " or element[0:4] == " && ":
        return 4
    elif element[0:19] == "[A-z 0-9?!+-/*=:[]]":
        return 19
    else:
        for char in element:
            if char != "<":
                count += 1
                #if char == ' ':
                #    print(junto)
                #    print(count)
                if char == ' ' and lastChar != 'z':
                    break
                lastChar = char
            else:
                break
    return count


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
    global COUNTIF
    COUNTIF = 0
    old = 0
    num_term = number_of_chars_terminal(value.replace("`SP`", " "))
    to_insert = ""
    while value != "":
        num_term = number_of_chars_terminal(value.replace("`SP`", " "))
        next_non_terminal = ""
        if ' ' in value.replace("`SP`", " ")[0:num_term] and num_term != 19:
            old = num_term
            num_term = value.replace("`SP`", " ").find(' ')
        if value != "∅":
            if num_term == 0:
                to_insert += "trimmed = trim(result.getText(), result.getLine(), result.getColumn());\n\
                text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();\n"
            next_non_terminal = find_next_non_terminal(value.replace("`SP`", " ").replace("-", "_")[num_term:].lower())
            if(num_term > 0):            
                COUNTIF += 1
                if value.replace("`SP`", " ")[0:num_term] == "[A-z 0-9?!+-/*=:[]]":
                    to_insert += "  result.setError(\"Invalid character\"); \n";
                elif value.replace("`SP`", " ")[0:num_term] == "[A-z]":
                    to_insert += "  result.setError(\"Expected letter\"); \n";
                elif value.replace("`SP`", " ")[0:num_term] == "[0-9]":
                    to_insert += "  result.setError(\"Expected number\"); \n";
                elif value.replace("`SP`", " ")[0:num_term] == "\"":
                    to_insert += "  result.setError(\"Expected \\"+ value.replace("`SP`", " ")[0:num_term] + "\"); \n";
                else:
                    to_insert += "  result.setError(\"Expected "+ value.replace("`SP`", " ")[0:num_term] + "\"); \n";
                
                if len(value) == num_term:
                    to_insert += "    if(text.length() >= "
                else:
                    to_insert += "    if(text.length() > "
                if value.replace("`SP`", " ")[0:num_term] == "[A-z 0-9?!+-/*=:[]]":
                    to_insert += "1"  +" && istext(text[0])){\n"
                elif value.replace("`SP`", " ")[0:num_term] == "[A-z]":
                    to_insert += "1"  + " && isalpha(text[0])){\n"
                elif value.replace("`SP`", " ")[0:num_term] == "[0-9]":
                    to_insert += "1"  + " && isdigit(text[0])){\n"
                else:
                    if value.replace("`SP`", " ")[0:num_term] == "\"":
                        to_insert += str(num_term)  +" && text.substr(0, "+str(num_term)+") == \"\\\"\"){\n"
                    else:
                        to_insert += str(num_term)  +" && text.substr(0, "+str(num_term)+") == \""+value.replace("`SP`", " ")[0:num_term]+"\"){\n"
            if ' ' not in value.replace("`SP`", " ")[0:old]:
                if next_non_terminal != "":
                    if value.replace("`SP`", " ")[0:num_term] == "[A-z]" or value.replace("`SP`", " ")[0:num_term] == "[0-9]" or value.replace("`SP`", " ")[0:num_term] == "[A-z 0-9?!+-/*=:[]]":
                        to_insert += "        result = " + next_non_terminal + "(text.substr(1), line, column);\n"
                    else:
                        to_insert += "        result = " + next_non_terminal + "(text.substr("+str(num_term)+"), line, column + " + str(num_term) + ");\n"
                    to_insert += "        if(!result.getAccept()){\n"
                    to_insert += "            return result;\n"
                    to_insert += "        }\n"
                else:
                    to_insert += "          result.setText(text.substr("+str(num_term)+"));\n"
                    to_insert += "          result.setColumn("+str(num_term)+"+result.getColumn());\n"
                    to_insert += "          return result;\n"
            else:
                to_insert += "          result.setText(text.substr("+str(num_term)+"));\n"
                to_insert += "          result.setColumn("+str(num_term)+"+result.getColumn());\n"
            if(value.replace("`SP`", " ")[(num_term + len(next_non_terminal) + 2):] != ""):
                to_insert += "        trimmed = trim(result.getText(), result.getLine(), result.getColumn());\n\
            text = trimmed.getText(); line = trimmed.getLine(); column = trimmed.getColumn();\n"
        else:
            print("")
            #to_insert += "    return result; \n"
        if ' ' in value.replace("`SP`", " ")[0:old]:
            value = value.replace("`SP`", " ")[(num_term + 1):]
        else:
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
    global COUNTIF
    CODE += "ResultType " + key.replace("-", "_").replace("<", "").replace(">","").lower() + "(string text_original, int l, int c){\n"
    CODE += generate_layout()
    for value in TABLE[key]:
        CODE += generate_element(value)
        for x in range(0,COUNTIF):
            if x < COUNTIF-1:
                CODE += "}else{\n"
                CODE += "result.setAccept(false);\n"
                CODE += "}\n"
            else:
                CODE += "return result; \n"
                CODE += "}\n"
    #if "∅" not in TABLE[key]:
    #    CODE += "result.setAccept(false);\n"
    #    CODE += "return result;\n"
    CODE += "}\n"

def generate_header(key):
    global CODE 
    CODE += "ResultType " + key.replace("-", "_").replace("<", "").replace(">","").lower() + "(string text_original, int l, int c);\n"

def main():
    parse_text()
    for key in TABLE:
        generate_header(key)
    for key in TABLE:
        generate(key)
    #print(CODE)
    NEWFILE.write(CODE)
main()