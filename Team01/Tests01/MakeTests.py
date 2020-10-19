
import collections
import random
import string

INDENTATION = "    "

NODE_PROGRAM = 0
NODE_PROCEDURE = 1
NODE_STATEMENT_LIST = 2
NODE_STATEMENT_READ = 3
NODE_STATEMENT_PRINT = 4
NODE_STATEMENT_CALL = 5
NODE_STATEMENT_WHILE = 6
NODE_STATEMENT_IF = 7
NODE_STATEMENT_ASSIGN = 8

CONSTANT_NAME_MIN = 1
CONSTANT_NAME_MAX = 20

PROCEDURES_MIN = 5
PROCEDURES_MAX = 10
PROCEDURE_NAME_MIN = 1
PROCEDURE_NAME_MAX = 5

STMT_LIST_MIN = 1
STMT_LIST_MAX = 10

VARIABLES_MIN = 5
VARIABLES_MAX = 10
VARIABLE_NAME_MIN = 1
VARIABLE_NAME_MAX = 5

def ast_to_lines(ast, indent=0):
    result = []
    if ast[0] == NODE_PROGRAM:
        for i, c in enumerate(ast[1]):
            if i != 0:
                result.append((False, 0, ""))
            result.extend(ast_to_lines(c, indent))
    elif ast[0] == NODE_PROCEDURE:
        result.append((False, 0, "procedure {} {{".format(ast[1][0])))
        result.extend(ast_to_lines(ast[1][1], indent+1))
        result.append((False, 0, "}"))
    elif ast[0] == NODE_STATEMENT_LIST:
        for c in ast[1]:
            result.extend(ast_to_lines(c, indent))
    elif ast[0] == NODE_STATEMENT_READ:
        result.append((True, indent, "read {};".format(ast[1][0])))
    elif ast[0] == NODE_STATEMENT_PRINT:
        result.append((True, indent, "print {};".format(ast[1][0])))
    elif ast[0] == NODE_STATEMENT_CALL:
        result.append((True, indent, "call {};".format(ast[1][0])))
    elif ast[0] == NODE_STATEMENT_WHILE:
        result.append((True, indent, "while ({}) {{".format(ast[1][0])))
        result.extend(ast_to_lines(ast[1][1], indent + 1))
        result.append((False, indent, "}"))
    elif ast[0] == NODE_STATEMENT_IF:
        result.append((True, indent, "if ({}) then {{".format(ast[1][0])))
        result.extend(ast_to_lines(ast[1][1], indent + 1))
        result.append((False, indent, "} else {"))
        result.extend(ast_to_lines(ast[1][2], indent + 1))
        result.append((False, indent, "}"))
    elif ast[0] == NODE_STATEMENT_ASSIGN:
        result.append((True, indent, "{} = {};".format(ast[1][0], ast[1][1])))
    return result

def ast_to_source(ast):
    lines = ast_to_lines(ast)
    result = []
    for b, i, v in lines:
        indent = INDENTATION * i
        result.append("{}{}".format(indent, v))
    return "\n".join(result)

LETTER = string.ascii_uppercase + string.ascii_lowercase
DIGIT = string.digits

def random_choices(a, b, f):
    result = []
    for i in range(random.randint(a, b)):
        result.append(f())
    return result

def random_name(a, b):
    result = []
    result.append(random.choice(LETTER))
    result.extend(random_choices(a - 1, b - 1, lambda: random.choice(LETTER + DIGIT)))
    return "".join(result)

def random_integer(a, b):
    result = collections.deque()
    result.extend(random_choices(a, b, lambda: random.choice(DIGIT)))
    while len(result) > 1:
        if result[0] != "0":
            break
        result.popleft()
    return "".join(result)

def random_constant():
    return random_integer(CONSTANT_NAME_MIN, CONSTANT_NAME_MAX)

def random_procedure_names():
    f = lambda: random_name(PROCEDURE_NAME_MIN, PROCEDURE_NAME_MAX)
    return random_choices(PROCEDURES_MIN, PROCEDURES_MAX, f)

def random_variable_names():
    f = lambda: random_name(VARIABLE_NAME_MIN, VARIABLE_NAME_MAX)
    return random_choices(VARIABLES_MIN, VARIABLES_MAX, f)

def random_stmt_list():
    f = lambda: random.choice([random_read, random_print])()
    children = random_choices(STMT_LIST_MIN, STMT_LIST_MAX, f)
    return (NODE_STATEMENT_LIST, children)

def random_program():
    f = lambda: random_procedure()
    children = random_choices(PROCEDURES_MIN, PROCEDURES_MAX, f)
    return (NODE_PROGRAM, children)

def random_procedure():
    name = random.choice(list(procedure_names))
    procedure_names.remove(name)
    stmt_list = random_stmt_list()
    return (NODE_PROCEDURE, [name, stmt_list])

def random_read():
    name = random.choice(list(variable_names))
    return (NODE_STATEMENT_READ, [name])

def random_print():
    name = random.choice(list(variable_names))
    return (NODE_STATEMENT_PRINT, [name])

procedure_names = set(random_procedure_names())
variable_names = set(random_variable_names())

print(procedure_names)
print(variable_names)

ast = random_program()
source = ast_to_source(ast)
print(source)
