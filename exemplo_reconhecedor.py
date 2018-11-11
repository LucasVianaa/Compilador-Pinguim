#funcao auxiliar para eliminar espacos no comeco e fim de uma string
SPACES = [' ', '\n', '\t']
def trim(txt, line, column):
    i = 0
    l = line
    c = column
    while i < len(txt) and txt[i] in SPACES:
        c += 1
        if txt[i] == '\n':
            l += 1
            c = 0
        i += 1
    j = len(txt) - 1
    if i <= j:
        return [txt[i: j + 1], l, c]
    return ['', l, c]

def isLetter(s):
    return s >= 'a' and s <= 'z'

#baseado na derivacao de <Program> se faz uma funcao
def program(txt_original, line, column):
    trimmed = trim(txt_original, line, column)
    txt = trimmed[0]
    l = trimmed[1]
    c = trimmed[2]
    if len(txt) > 2 and txt[0:2] == 'if' and not isLetter(txt[2]): #caso if
        result = exp(txt[2:], l, c + 2)
        if not result[0]:
            return result
        trimmed = trim(result[1], result[2], result[3])
        txt = trimmed[0]
        l = trimmed[1]
        c = trimmed[2]
        if len(txt) >= 4 and txt[0:4] == 'then': #precisa then
            result = program(txt[4:], l, c + 4)
            if not result[0]:
                return result
            trimmed = trim(result[1], result[2], result[3])
            txt = trimmed[0]
            l = trimmed[1]
            c = trimmed[2]
            if len(txt) >= 4 and txt[:4] == 'else': #caso else
                result = program(txt[4:], l, c + 4)
                if not result[0]:
                    return result
                trimmed = trim(result[1], result[2], result[3])
                txt = trimmed[0]
                l = trimmed[1]
                c = trimmed[2]
            return [True, txt, l, c]
        return [False, txt, l, c, 'Missing "then"!']#caso nao tenha o then apos o if
    elif len(txt) > 0 and isLetter(txt[0]) and not (len(txt) > 4 and txt[:4] == 'else' and not isLetter(txt[4])):#caso seja algo comecando por a ou z
        result = barId(txt[1:], l, c + 4)
        if not result[0]:
            return result
        result = posId(result[1], result[2], result[3])
        if not result[0]:
            return result
        return program(result[1], result[2], result[3])
    return [True, txt, l, c] #caso vazio

#baseado na derivacao de <Id(com barra)> se faz uma funcao
def barId(txt, l, c):
    if len(txt) > 0 and isLetter(txt[0]):#caso comece com uma letra
        return barId(txt[1:], l, c + 1)
    return [True, txt, l, c] #caso vazio

#baseado na derivacao de <Pos Id> se faz uma funcao
def posId(txt_original, line, column):
    trimmed = trim(txt_original, line, column)
    txt = trimmed[0]
    l = trimmed[1]
    c = trimmed[2]
    if len(txt) > 0:
        if txt[0] == '=':#caso comeca com igual
            return exp(txt[1:], l, c + 1)
        elif txt[0] == '(':#caso comeca com parentesis aberto
            return fun(txt[1:], l, c + 1)
    return [False, txt, l, c, 'Invalid command: expected an attribution a function definition or a function call!']

#baseado na derivacao de <Exp.> se faz uma funcao
def exp(txt_original, line, column):
    trimmed = trim(txt_original, line, column)
    txt = trimmed[0]
    l = trimmed[1]
    c = trimmed[2]
    if len(txt) > 0:
        if txt[0] == '(': #caso parentesis aberto
            result = exp(txt[1:], l, c + 1)
            if not result[0]:
                return result
            trimmed = trim(result[1], result[2], result[3])
            txt = trimmed[0]
            l = trimmed[1]
            c = trimmed[2]
            if len(txt) > 0 and txt[0] == ')':#precisa fechar o parentesis
                return barExp(txt[1:], l, c + 1)
            return [False, txt, l, c, 'Missing ")"!']
        elif isLetter(txt[0]):#caso comece com letra
            result = barId(txt[1:], l, c + 1)
            if not result[0]:
                return result
            return expId(result[1], result[2], result[3])
        elif txt[0] == '0':#caso comece com zero
            return barExp(txt[1:], l , c + 1)
        elif txt[0] >= '1' and txt[0] <= '9': #caso comece com numero > 0
            result = barNum(txt[1:], l, c + 1)
            if not result[0]:
                return result
            return barExp(result[1], result[2], result[3])
    return [False, txt, l, c, 'Invalid expression!']
            
            

#baseado na derivacao de <Num.(com barra)> se faz uma funcao
def barNum(txt, l, c):
    if len(txt) > 0 and txt[0] >= '0' and txt[0] <= '9': #caso ainda seja um numero
        return barNum(txt[1:], l, c + 1)
    return [True, txt, l, c]#caso vazio

#baseado na derivacao de <Exp.(com barra)> se faz uma funcao
def barExp(txt_original, line, column):
    trimmed = trim(txt_original, line, column)
    txt = trimmed[0]
    l = trimmed[1]
    c = trimmed[2]
    if len(txt) > 0 and txt[0] in '+-*/':#caso seja uma operacao
        result = exp(txt[1:], l, c + 1)
        if not result[0]:
            return result
        return barExp(result[1], result[2], result[3])
    return [True, txt, l, c]#caso vazio

#baseado na derivacao de <Exp.Id> se faz uma funcao
def expId(txt_original, line, column):
    trimmed = trim(txt_original, line, column)
    txt = trimmed[0]
    l = trimmed[1]
    c = trimmed[2]
    if len(txt) > 0:
        if txt[0] in '+-*/':#caso seja uma operacao
            result = exp(txt[1:], l, c + 1)
            if not result[0]:
                return result
            return barExp(result[1], result[2], result[3])
        elif txt[0] == '(':#caso seja parentesis aberto
            result = lExp(txt[1:], l, c + 1)
            if not result[0]:
                return result
            trimmed = trim(result[1], result[2], result[3])
            txt = trimmed[0]
            l = trimmed[1]
            c = trimmed[2]
            if len(txt) > 0 and txt[0] == ')':#precisa fechar o parentesis
                return barExp(txt[1:], l, c + 1)
            return [False, txt, l, c, 'Missing ")"!']
    return [True, txt, l, c]#caso vazio


#baseado na derivacao de <L.Exp> se faz uma funcao
def lExp(txt_original, line, column):
    trimmed = trim(txt_original, line, column)
    txt = trimmed[0]
    l = trimmed[1]
    c = trimmed[2]
    if len(txt) > 0:
        if txt[0] == '(':#caso comece com parentesis aberto
            result = exp(txt[1:], l, c + 1)
            if not result[0]:
                return result
            trimmed = trim(result[1], result[2], result[3])
            txt = trimmed[0]
            l = trimmed[1]
            c = trimmed[2]
            if len(txt) > 0 and txt[0] == ')':#precisa fechar o parentesis
                result = barExp(txt[1:], l, c + 1)
                if not result[0]:
                    return result
                return barLExp(result[1], result[2], result[3])
            return [False, txt, l, c, 'Missing ")"!']
        elif isLetter(txt[0]):#caso comece com letra
            result = barId(txt[1:], l, c + 1)
            if not result[0]:
                return result
            result = expId(result[1], result[2], result[3])
            if not result[0]:
                return result
            return barLExp(result[1], result[2], result[3])
        elif txt[0] == '0':#caso comece com zero
            result = barExp(txt[1:], l, c + 1)
            if not result[0]:
                return result
            return barLExp(result[1], result[2], result[3])
        elif txt[0] >= '1' and txt[0] <= '9':#caso seja um numero >0
            result = barNum(txt[1:], l, c + 1)
            if not result[0]:
                return result
            result = barExp(result[1], result[2], result[3])
            if not result[0]:
                return result
            return barLExp(result[1], result[2], result[3])
    return [False, txt, l, c]

    
#baseado na derivacao de <L.Exp (com barra)> se faz uma funcao
def barLExp(txt_original, line, column):
    trimmed = trim(txt_original, line, column)
    txt = trimmed[0]
    l = trimmed[1]
    c = trimmed[2]
    if len(txt) > 0 and txt[0] == ',':#caso comece com virgula
        result = exp(txt[1:], l, c + 1)
        if not result[0]:
            return result
        return barLExp(result[1], result[2], result[3])
    return [True, txt, l, c]#caso vazio

#baseado na derivacao de <Fun.> se faz uma funcao
def fun(txt_original, line, column):
    trimmed = trim(txt_original, line, column)
    txt = trimmed[0]
    l = trimmed[1]
    c = trimmed[2]
    if len(txt) > 0:
        if txt[0] == ')':#caso feche parentesis
            return emptyFun(txt[1:], l , c + 1)
        elif txt[0] == '(':#caso abra parentesis
            result = exp(txt[1:], l, c + 1)
            if not result[0]:
                return result
            trimmed = trim(result[1], result[2], result[3])
            txt = trimmed[0]
            l = trimmed[1]
            c = trimmed[2]
            if len(txt) > 0 and txt[0] == ')':#precisa fechar o parentesis
                result = barExp(txt, l, c + 1)
                if not result[0]:
                    return result
                result = barLExp(result[1], result[2], result[3])
                if not result[0]:
                    return result
                trimmed = trim(result[1], result[2], result[3])
                txt = trimmed[0]
                l = trimmed[1]
                c = trimmed[2]
                if len(txt) > 0 and txt[0] == ')':
                    return [True, txt[1:], l, c + 1]
            return [False, txt, l, c, 'Missing ")"!']
        elif isLetter(txt[0]):#caso comece com letra
            result = barId(txt[1:], l, c + 1)
            if not result[0]:
                return result
            return funId(result[1], result[2], result[3])
        elif txt[0] == '0':#caso comece com zero
            result = barExp(txt[1:], l, c + 1)
            if not result[0]:
                return result
            result = barLExp(result[1], result[2], result[3])
            if not result[0]:
                return result
            trimmed = trim(result[1], result[2], result[3])
            txt = trimmed[0]
            l = trimmed[1]
            c = trimmed[2]
            if len(txt) > 0 and txt[0] == ')':
                return [True, txt[1:], l, c + 1]
            return [False, txt, l, c, 'Missing ")"!']
        elif txt[0] >= '1' and txt[0] <= '9':#comeca com numero >0
            result = barNum(txt[1:], l, c + 1)
            if not result[0]:
                return result
            result = barExp(result[1], result[2], result[3])
            if not result[0]:
                return result
            result = barLExp(result[1], result[2], result[3])
            if not result[0]:
                return result
            trimmed = trim(result[1], result[2], result[3])
            txt = trimmed[0]
            l = trimmed[1]
            c = trimmed[2]
            if len(txt) > 0 and txt[0] == ')':
                return [True, txt[1:], l, c + 1]
            return [False, txt, l, c, 'Missing ")"!']
    return [False, txt, l, c]
            

#baseado na derivacao de <Empty Fun.> se faz uma funcao
def emptyFun(txt_original, line, column):
    trimmed = trim(txt_original, line, column)
    txt = trimmed[0]
    l = trimmed[1]
    c = trimmed[2]
    if len(txt) > 0 and txt[0] == '{':#comeca abrindo chaves
        result = program(txt[1:], l, c + 1)
        if not result[0]:
            return result
        trimmed = trim(result[1], result[2], result[3])
        txt = trimmed[0]
        l = trimmed[1]
        c = trimmed[2]
        if len(txt) > 0 and txt[0] == '}':#precisa fechar a chave
            return [True, txt[1:], l, c + 1]
        return [False, txt, l, c, 'Missing "}"!']
    return [True, txt, l, c]#caso vazio


#baseado na derivacao de <Fun.Id> se faz uma funcao
def funId(txt_original, line, column):
    trimmed = trim(txt_original, line, column)
    txt = trimmed[0]
    l = trimmed[1]
    c = trimmed[2]
    if len(txt) > 0:
        if txt[0] in '+-*/':#caso seja um operador
            result = exp(txt[1:], l, c + 1)
            if not result[0]:
                return result
            result = barExp(result[1], result[2], result[3])
            if not result[0]:
                return result
            result = barLExp(result[1], result[2], result[3])
            if not result[0]:
                return result
            trimmed = trim(result[1], result[2], result[3])
            txt = trimmed[0]
            l = trimmed[1]
            c = trimmed[2]
            if len(txt) > 0 and txt[0] == ')':
                return [True, txt[1:], l, c + 1]
        elif txt[0] == '(':#caso seja parentesis aberto
            result = lExp(txt[1:], l, c + 1)
            if not result[0]:
                return result
            trimmed = trim(result[1], result[2], result[3])
            txt = trimmed[0]
            l = trimmed[1]
            c = trimmed[2]
            if len(txt) > 0 and txt[0] == ')':#fecha o parentesis
                result = barExp(txt[1:], l, c + 1)
                if not result[0]:
                    return result
                result = barLExp(result[1], result[2], result[3])
                if not result[0]:
                    return result
                trimmed = trim(result[1], result[2], result[3])
                txt = trimmed[0]
                l = trimmed[1]
                c = trimmed[2]
                if len(txt) > 0 and txt[0] == ')':
                    return [True, txt[1:], l, c + 1]
        elif txt[0] == ',':#comeca com virgula
            return list_(txt[1:], l, c + 1)
        elif txt[0] == ')': #fecha parentesis
            return afterParFunId(txt[1:], l, c + 1)
    return [False, txt, l, c]

#baseado na derivacao de <AfterParFunId> se faz a funcao
def afterParFunId(txt_original, line, column):
    trimmed = trim(txt_original, line, column)
    txt = trimmed[0]
    l = trimmed[1]
    c = trimmed[2]
    if len(txt) > 0 and txt[0] == '=': #caso defina funcao
        trimmed = trim(txt[1:], l, c + 1)
        txt = trimmed[0]
        l = trimmed[1]
        c = trimmed[2]
        if len(txt) > 0 and txt[0] == '{':#precisa abrir chave para escrever a funcao
            result = program(txt[1:], l, c + 1)
            if not result[0]:
                return result
            trimmed = trim(result[1], result[2], result[3])
            txt = trimmed[0]
            l = trimmed[1]
            c = trimmed[2]
            if len(txt) > 0 and txt[0] == '}':#fecha a chave
                return [True, txt[1:], l, c + 1]
    return [True, txt, l, c]#caso vazio

#baseado na derivacao de <List> se faz uma funcao
def list_(txt_original, line, column):
    trimmed = trim(txt_original, line, column)
    txt = trimmed[0]
    l = trimmed[1]
    c = trimmed[2]
    if len(txt) > 0:
        if txt[0] == '(':
            result = exp(txt[1:], l, c + 1)
            if not result[0]:
                return result
            trimmed = trim(result[1], result[2], result[3])
            txt = trimmed[0]
            l = trimmed[1]
            c = trimmed[2]
            if len(txt) > 0 and txt[0] == ')':
                result = barExp(txt[1:], l, c + 1)
                if not result[0]:
                    return result
                result = barLExp(result[1], result[2], result[3])
                if not result[0]:
                    return result
                trimmed = trim(result[1], result[2], result[3])
                txt = trimmed[0]
                l = trimmed[1]
                c = trimmed[2]
                if len(txt) > 0 and txt[0] == ')':
                    return [True, txt[1:], l, c + 1]
        elif txt[0] == '0':
            result = barExp(txt[1:], l, c + 1)
            if not result[0]:
                return result
            result = barLExp(result[1], result[2], result[3])
            if not result[0]:
                return result
            trimmed = trim(result[1], result[2], result[3])
            txt = trimmed[0]
            l = trimmed[1]
            c = trimmed[2]
            if len(txt) > 0 and txt[0] == ')':
                return [True, txt[1:], l, c + 1]
        elif txt[0] >= '1' and txt[0] <= '9':
            result = barNum(txt[1:], l, c + 1)
            if not result[0]:
                return result
            result = barExp(result[1], result[2], result[3])
            if not result[0]:
                return result
            result = barLExp(result[1], result[2], result[3])
            if not result[0]:
                return result
            trimmed = trim(result[1], result[2], result[3])
            txt = trimmed[0]
            l = trimmed[1]
            c = trimmed[2]
            if len(txt) > 0 and txt[0] == ')':
                return [True, txt[1:], l, c + 1]
        elif isLetter(txt[0]):
            result = barId(txt[1:], l, c + 1)
            if not result[0]:
                return result
            return listId(result[1], result[2], result[3])
    return [False, txt, l, c]
            

#baseado na derivacao de <List Id> se faz uma funcao
def listId(txt_original, line, column):
    trimmed = trim(txt_original, line, column)
    txt = trimmed[0]
    l = trimmed[1]
    c = trimmed[2]
    if len(txt) > 0:
        if txt[0] in '+-/*':
            result = exp(txt[1:], l, c + 1)
            if not result[0]:
                return result
            result = barExp(result[1], result[2], result[3])
            if not result[0]:
                return result
            result = barLExp(result[1], result[2], result[3])
            if not result[0]:
                return result
            trimmed = trim(result[1], result[2], result[3])
            txt = trimmed[0]
            l = trimmed[1]
            c = trimmed[2]
            if len(txt) > 0 and txt[0] == ')':
                return [True, txt[1:], l, c + 1]
        elif txt[0] == '(':
            result = lExp(txt[1:], l, c + 1)
            if not result[0]:
                return result
            trimmed = trim(result[1], result[2], result[3])
            txt = trimmed[0]
            l = trimmed[1]
            c = trimmed[2]
            if len(txt) > 0 and txt[0] == ')':
                result = barExp(txt[1:], l, c + 1)
                if not result[0]:
                    return result
                result = barLExp(result[1], result[2], result[3])
                if not result[0]:
                    return result
                trimmed = trim(result[1], result[2], result[3])
                txt = trimmed[0]
                l = trimmed[1]
                c = trimmed[2]
                if len(txt) > 0 and txt[0] == ')':
                    return [True, txt[1:], l, c + 1]
        elif txt[0] == ',':
            return list_(txt[1:], l, c + 1)
        elif txt[0] == ')':
            trimmed = trim(txt[1:], l, c + 1)
            txt = trimmed[0]
            l = trimmed[1]
            c = trimmed[2]
            if len(txt) > 0 and txt[0] == '=':
                trimmed = trim(txt[1:], l, c + 1)
                txt = trimmed[0]
                l = trimmed[1]
                c = trimmed[2]
                if len(txt) > 0 and txt[0] == '{':
                    result = program(txt[1:], l, c + 1)
                    if not result[0]:
                        return result
                    trimmed = trim(result[1], result[2], result[3])
                    txt = trimmed[0]
                    l = trimmed[1]
                    c = trimmed[2]
                    if len(txt) > 0  and txt[0] == '}':
                        return [True, txt[1:], l, c + 1]            
    return [False, txt, l, c, 'Syntax error!']


#metodo principal, para reconhecer deve consumir a string completa
def recognizes(txt):
    result = program(txt, 1, 0)
    if (result[0] and len(result[1]) == 0):
        print('Recognized!!')
    else:
        if len(result) == 5:
            print('Error at line', result[2], 'and column', str(result[3]) + '.', result[4])
        else:
            print('Error at line', result[2], 'and column', str(result[3]) + '. Syntax error!')
        print('Not processed code:', result[1])