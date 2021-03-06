#
# A friendly script to help with system testing :)
#

import argparse
import collections
import glob
import os
import re
import signal
import subprocess
import sys
import xml.etree.ElementTree as ElementTree



AUTOTESTER_DIRECTORIES = [".", "../Code01/Release", "../Code01/Debug"]
AUTOTESTER_FILE_NAME = "AutoTester.exe"
OUTPUT_DIRECTORY = "./RunTestsOutput"
PUBLISH_OUTPUT_DIRECTORY = os.path.join(OUTPUT_DIRECTORY, "Publish")

QUERIES_EXTENSION = "qry"
SOURCE_EXTENSION = "src"
SOURCE_DEPENDENCIES_EXTENSION = "dep"
LABEL_EXTENSION = "lab"
PUBLISH_QRY_EXTENSION = "_queries.txt"
PUBLISH_SRC_EXTENSION = "_source.txt"

INFO_ACCENT_COLOR = "\033[36m"
WARNING_COLOR = "\033[93m"
ERROR_COLOR = "\033[91m"
END_COLOR = "\033[0m"

LABEL_INDENTATION = "    "

TOKEN_NAME = 0
TOKEN_INTEGER = 1
TOKEN_ZERO = 2
TOKEN_OPEN_BRACKET = 3
TOKEN_CLOSE_BRACKET = 4
TOKEN_OPEN_BRACE = 5
TOKEN_CLOSE_BRACE = 6
TOKEN_SEMICOLON = 7
TOKEN_EXPR = 8
TOKEN_TERM = 9
TOKEN_REL_EXPR = 10
TOKEN_NOT = 11
TOKEN_AND = 12
TOKEN_OR = 13
TOKEN_EQUALS = 14
TOKEN_EOF = 15

NODE_PROGRAM = 0
NODE_PROCEDURE = 1
NODE_STATEMENT_LIST = 2
NODE_STATEMENT_READ = 3
NODE_STATEMENT_PRINT = 4
NODE_STATEMENT_CALL = 5
NODE_STATEMENT_WHILE = 6
NODE_STATEMENT_IF = 7
NODE_STATEMENT_ASSIGN = 8

SUMMARY_STATEMENT = 9
SUMMARY_CONTAINER = 10

TOKENS_PATTERN = {
    "[A-Za-z][A-Za-z\d]*": TOKEN_NAME,
    "[1-9]\d*": TOKEN_INTEGER,
    "0": TOKEN_ZERO,
    "\(": TOKEN_OPEN_BRACKET,
    "\)": TOKEN_CLOSE_BRACKET,
    "{": TOKEN_OPEN_BRACE,
    "}": TOKEN_CLOSE_BRACE,
    ";": TOKEN_SEMICOLON,
    "\+|-": TOKEN_EXPR,
    "\*|\/|%": TOKEN_TERM,
    ">=|<=|==|!=|>|<": TOKEN_REL_EXPR,
    "!": TOKEN_NOT,
    "&&": TOKEN_AND,
    "\|\|": TOKEN_OR,
    "=": TOKEN_EQUALS,
    "\s+": None,
    ".": None
}



def printinfo(s):
    print("Info: {}".format(s))

def printinfoaccent(s):
    print("{}Info: {}{}".format(INFO_ACCENT_COLOR, s, END_COLOR))

def printerr(s):
    print("{}Error: {}{}".format(ERROR_COLOR, s, END_COLOR))
    exit(1)

def printwarn(s):
    print("{}Warning: {}{}".format(WARNING_COLOR, s, END_COLOR))



# Find AutoTester.exe
def find_autotester_path():
    printinfo("Finding '{}'...".format(AUTOTESTER_FILE_NAME))
    autotester_path = None
    for path in AUTOTESTER_DIRECTORIES:
        path = os.path.join(os.getcwd(), path, AUTOTESTER_FILE_NAME)
        if os.path.isfile(path):
            autotester_path = os.path.abspath(path)
            break

    if not autotester_path:
        printerr("'{}' not found! Please check that you have built AutoTester, and it is in any of these directories:\n{}"
            .format(AUTOTESTER_FILE_NAME, "\n".join(map(lambda p : os.path.abspath(p), AUTOTESTER_DIRECTORIES))))

    printinfo("'{}' found! '{}'".format(AUTOTESTER_FILE_NAME, autotester_path))
    return autotester_path

# Find all source files
def get_source_paths():
    printinfo("Finding all source files...")

    # If source_filter is None, match all file names
    # Else, match from start of file name
    if source_filter is None:
        name_regex = ".+"
    else:
        name_regex = "{}.*".format(re.escape(source_filter))

    source_paths = []
    # BFS
    depth = 0
    queue = collections.deque()
    queue.append(tests_dir_path)
    while True:
        # No more directories to search
        if len(queue) == 0:
            return source_paths

        # The maximum depth is not None, and
        # The current depth exceeds the maximum depth
        if max_source_depth is not None and depth > max_source_depth:
            return source_paths

        # Get current path
        root_path = queue.popleft()
        # Find all files and directories in the current path
        for file_name in os.listdir(root_path):
            path = os.path.join(root_path, file_name)
            if os.path.isfile(path):
                regex = "^({})\\.([^.]+)$".format(name_regex)
                match = re.search(regex, file_name)
                if not match:
                    continue
                name = match.group(1)
                ext = match.group(2)
                name_path = os.path.join(root_path, name)
                if ext == SOURCE_EXTENSION:
                    source_paths.append(name_path)
            elif os.path.isdir(path):
                queue.append(path)
        depth += 1

# Get dependencies of source files
def get_deps_paths(source_paths):
    printinfo("Checking source dependencies files...")
    out = []
    for source_path in source_paths:
        dependencies_path = "{}.{}".format(source_path, SOURCE_DEPENDENCIES_EXTENSION)
        if not os.path.isfile(dependencies_path):
            path = "{}.{}".format(source_path, SOURCE_EXTENSION)
            path = os.path.relpath(path)
            printwarn("'{}' is missing its dependency file! Skipping...".format(path))
            continue

        deps_paths = []
        with open(dependencies_path) as d:
            for line in d:
                line = line.strip()
                queries_path = os.path.join(os.path.dirname(source_path), line)
                queries_path = os.path.abspath(queries_path)
                queries_full_path = "{}.{}".format(queries_path, QUERIES_EXTENSION)
                if not os.path.isfile(queries_full_path):
                    d_path = os.path.relpath(dependencies_path)
                    q_path = os.path.relpath(queries_full_path)
                    printwarn("'{}': '{}' query file does not exist! Skipping...".format(d_path, q_path))
                    continue
                deps_paths.append(queries_path)
        out.append((source_path, deps_paths))
    return out

def tokenize_source(content):
    out = collections.deque()
    while True:
        length = len(content)
        if length == 0:
            out.append((TOKEN_EOF, "EOF"))
            return out

        for k, v in TOKENS_PATTERN.items():
            match = re.match(k, content)
            if not match:
                continue

            word = match.group()
            content = content[len(word):]
            if not v == None:
                out.append((v, word))
            break

        if length == len(content):
            raise Exception("Failed to tokenize! :(")

def parse_source(content):
    tokens = tokenize_source(content)
    ast = parse_program(tokens)
    validate_program(ast)
    return ast

def not_none(n, error):
    if n is None:
        raise Exception(error)
    return n

def lookahead(function, tokens):
    tokens_copy = tokens.copy()
    try:
        o = function(tokens_copy)
    except Exception:
        return None
    if o is None:
        return None
    tokens.clear()
    tokens.extend(tokens_copy)
    return o

# program: procedure+
def parse_program(tokens):
    children = []
    while True:
        c = parse_procedure(tokens)
        if c is None:
            break
        children.append(c)
    if len(children) < 1:
        raise Exception("Expected 'PROCEDURE'!")
    if not tokens[0][0] == TOKEN_EOF:
        raise Exception("Expected 'EOF' after 'PROCEDURE'!")
    return (NODE_PROGRAM, children)

# procedure: `procedure` proc_name `{` stmtLst `}`
def parse_procedure(tokens):
    if not (tokens[0][0] == TOKEN_NAME and tokens[0][1] == "procedure"):
        return None
    children = []
    tokens.popleft()
    token = tokens.popleft()
    if not token[0] == TOKEN_NAME:
        raise Exception("Expected 'NAME' after 'procedure'!")
    children.append(token[1])
    if not tokens.popleft()[0] == TOKEN_OPEN_BRACE:
        raise Exception("Expected '{{' after 'procedure {}'!".format(children[0]))
    children.append(not_none(parse_statement_list(tokens), "Expected 'STATEMENT_LIST' after 'procedure {} {{'!".format(children[0])))
    if not tokens.popleft()[0] == TOKEN_CLOSE_BRACE:
        raise Exception("Expected '}}' after 'procedure {} {{ STATEMENT_LIST'!".format(children[0]))
    return (NODE_PROCEDURE, children)

# stmtLst: stmt+
def parse_statement_list(tokens):
    children = []
    while True:
        c = parse_statement(tokens)
        if c is None:
            break
        children.append(c)
    if len(children) < 1:
        return None
    return (NODE_STATEMENT_LIST, children)

# stmt: read | print | call | while | if | assign
def parse_statement(tokens):
    for f in [parse_assign, parse_read, parse_print, parse_call, parse_while, parse_if]:
        o = f(tokens)
        if not o is None:
            return o
    return None

# read: `read` var_name `;`
def parse_read(tokens):
    if not (tokens[0][0] == TOKEN_NAME and tokens[0][1] == "read"):
        return None
    children = []
    tokens.popleft()
    token = tokens.popleft()
    if not token[0] == TOKEN_NAME:
        raise Exception("Expected 'NAME' after 'read'!")
    children.append(token[1])
    if not tokens.popleft()[0] == TOKEN_SEMICOLON:
        raise Exception("Expected ';' after 'read {}'!".format(children[0]))
    return (NODE_STATEMENT_READ, children)

# print: `print` var_name `;`
def parse_print(tokens):
    if not (tokens[0][0] == TOKEN_NAME and tokens[0][1] == "print"):
        return None
    children = []
    tokens.popleft()
    token = tokens.popleft()
    if not token[0] == TOKEN_NAME:
        raise Exception("Expected 'NAME' after 'print'!")
    children.append(token[1])
    if not tokens.popleft()[0] == TOKEN_SEMICOLON:
        raise Exception("Expected ';' after 'print {}'!".format(children[0]))
    return (NODE_STATEMENT_PRINT, children)

# call: `call` proc_name `;`
def parse_call(tokens):
    if not (tokens[0][0] == TOKEN_NAME and tokens[0][1] == "call"):
        return None
    children = []
    tokens.popleft()
    token = tokens.popleft()
    if not token[0] == TOKEN_NAME:
        raise Exception("Expected 'NAME' after 'call'!")
    children.append(token[1])
    if not tokens.popleft()[0] == TOKEN_SEMICOLON:
        raise Exception("Expected ';' after 'call {}'!".format(children[0]))
    return (NODE_STATEMENT_CALL, children)

# while: `while` `(` cond_expr `)` `{` stmtLst `}`
def parse_while(tokens):
    if not (tokens[0][0] == TOKEN_NAME and tokens[0][1] == "while"):
        return None
    children = []
    tokens.popleft()
    if not tokens.popleft()[0] == TOKEN_OPEN_BRACKET:
        raise Exception("Expected '(' after 'while'!")
    children.append(not_none(parse_cond_expr(tokens), "Expected 'COND_EXPR' after 'while ('!"))
    if not tokens.popleft()[0] == TOKEN_CLOSE_BRACKET:
        raise Exception("Expected ')' after 'while ({}'!".format(children[0]))
    if not tokens.popleft()[0] == TOKEN_OPEN_BRACE:
        raise Exception("Expected '{{' after 'while ({})'!".format(children[0]))
    children.append(not_none(parse_statement_list(tokens), "Expected 'STATEMENT_LIST' after 'while ({}) {{'!".format(children[0])))
    if not tokens.popleft()[0] == TOKEN_CLOSE_BRACE:
        raise Exception("Expected '}}' after 'while ({}) {{ STATEMENT_LIST'!".format(children[0]))
    return (NODE_STATEMENT_WHILE, children)

# if: `if` `(` cond_expr `)` `then` `{` stmtLst `}` `else` `{` stmtLst `}`
def parse_if(tokens):
    if not (tokens[0][0] == TOKEN_NAME and tokens[0][1] == "if"):
        return None
    children = []
    tokens.popleft()
    if not tokens.popleft()[0] == TOKEN_OPEN_BRACKET:
        raise Exception("Expected '(' after 'if'!")
    children.append(not_none(parse_cond_expr(tokens), "Expected 'COND_EXPR' after 'if' (!"))
    if not tokens.popleft()[0] == TOKEN_CLOSE_BRACKET:
        raise Exception("Expected ')' after 'if ({}'!".format(children[0]))
    token = tokens.popleft()
    if not (token[0] == TOKEN_NAME and token[1] == "then"):
        raise Exception("Expected 'then' after 'if ({})'!".format(children[0]))
    if not tokens.popleft()[0] == TOKEN_OPEN_BRACE:
        raise Exception("Expected '{{' after 'if ({}) then'!".format(children[0]))
    children.append(not_none(parse_statement_list(tokens), "Expected 'STATEMENT_LIST' after 'if ({}) then {{'!".format(children[0])))
    if not tokens.popleft()[0] == TOKEN_CLOSE_BRACE:
        raise Exception("Expected '}}' after 'if ({}) then {{ STATEMENT_LIST'!".format(children[0]))
    token = tokens.popleft()
    if not (token[0] == TOKEN_NAME and token[1] == "else"):
        raise Exception("Expected 'else' after 'if ({}) then {{ STATEMENT_LIST }}'!".format(children[0]))
    if not tokens.popleft()[0] == TOKEN_OPEN_BRACE:
        raise Exception("Expected '{{' after 'if ({}) then {{ STATEMENT_LIST }} else'!".format(children[0]))
    children.append(not_none(parse_statement_list(tokens), "Expected 'STATEMENT_LIST' after 'if ({}) then {{ STATEMENT_LIST }} else {{'!".format(children[0])))
    if not tokens.popleft()[0] == TOKEN_CLOSE_BRACE:
        raise Exception("Expected '}}' after 'if ({}) then {{ STATEMENT_LIST }} else {{ STATEMENT_LIST'!".format(children[0]))
    return (NODE_STATEMENT_IF, children)

# assign: var_name `=` expr `;`
def parse_assign(tokens):
    if not (tokens[0][0] == TOKEN_NAME and tokens[1][0] == TOKEN_EQUALS):
        return None
    children = []
    children.append(tokens.popleft()[1])
    tokens.popleft()
    children.append(not_none(parse_expr(tokens), "Expected 'EXPR' after '{} ='!".format(children[0])))
    if not tokens.popleft()[0] == TOKEN_SEMICOLON:
        raise Exception("Expected ';' after '{} = {}'!".format(children[0], children[1]))
    return (NODE_STATEMENT_ASSIGN, children)

# cond_expr: rel_expr | `!` `(` cond_expr `)` | `(` cond_expr `)` `&&` `(` cond_expr `)` | `(` cond_expr `)` `||` `(` cond_expr `)`
def parse_cond_expr(tokens):
    o = lookahead(parse_rel_expr, tokens)
    if not o is None:
        return o
    if not (tokens[0][0] == TOKEN_OPEN_BRACKET or tokens[0][0] == TOKEN_NOT):
        raise Exception("Expected '(' or '!' in 'COND_EXPR'!")
    if tokens[0][0] == TOKEN_OPEN_BRACKET:
        o = tokens.popleft()[1]
        o += not_none(parse_cond_expr(tokens), "Expected 'COND_EXPR' after '('!")
        token = tokens.popleft()
        if not token[0] == TOKEN_CLOSE_BRACKET:
            raise Exception("Expected ')' after '{}'!".format(o))
        o += token[1]
        token = tokens.popleft()
        if not (token[0] == TOKEN_AND or token[0] == TOKEN_OR):
            raise Exception("Expected '&&' or '||' after '{}'!".format(o))
        o += " {} ".format(token[1])
        token = tokens.popleft()
        if not token[0] == TOKEN_OPEN_BRACKET:
            raise Exception("Expected '(' after '{}'!".format(o))
        o += token[1]
        o += not_none(parse_cond_expr(tokens), "Expected 'COND_EXPR' after '{}'!".format(o))
        token = tokens.popleft()
        if not token[0] == TOKEN_CLOSE_BRACKET:
            raise Exception("Expected ')' after '{}'!".format(o))
        o += token[1]
        return o
    elif tokens[0][0] == TOKEN_NOT:
        o = tokens.popleft()[1]
        token = tokens.popleft()
        if not token[0] == TOKEN_OPEN_BRACKET:
            raise Exception("Expected '(' after '{}'!".format(o))
        o += token[1]
        o += not_none(parse_cond_expr(tokens), "Expected 'COND_EXPR' after '{}'!".format(o))
        token = tokens.popleft()
        if not token[0] == TOKEN_CLOSE_BRACKET:
            raise Exception("Expected ')' after '{}'!".format(o))
        o += token[1]
        return o

# rel_expr: rel_factor `>` rel_factor | rel_factor `>=` rel_factor | rel_factor `<` rel_factor | rel_factor `<=` rel_factor | rel_factor `==` rel_factor | rel_factor `!=` rel_factor
def parse_rel_expr(tokens):
    o = parse_rel_factor(tokens)
    if o is None:
        return None
    if not tokens[0][0] == TOKEN_REL_EXPR:
        raise Exception("Expected '>=' or '<=' or '==' or '!=' or '>' or '<' after '{}'!".format(o))
    o += " {} ".format(tokens.popleft()[1])
    o += not_none(parse_rel_factor(tokens), "Expected 'REL_FACTOR' after '{}'!".format(o))
    return o

# rel_factor: var_name | const_value | expr
def parse_rel_factor(tokens):
    o = parse_expr(tokens)
    if not o is None:
        return o
    # Constants are sequences of digits. If more than one digit, the first digit cannot be 0
    if tokens[0][0] == TOKEN_NAME or tokens[0][0] == TOKEN_INTEGER or tokens[0][0] == TOKEN_ZERO:
        return tokens.popleft()[1]
    return None

# expr: expr `+` term | expr `-` term | term
def parse_expr(tokens):
    o = parse_term(tokens)
    if o is None:
        return None
    if not tokens[0][0] == TOKEN_EXPR:
        return o
    o += " {} ".format(tokens.popleft()[1])
    o += parse_expr(tokens)
    return o

# term: term `*` factor | term `/` factor | term `%` factor | factor
def parse_term(tokens):
    o = parse_factor(tokens)
    if o is None:
        return None
    if not tokens[0][0] == TOKEN_TERM:
        return o
    o += " {} ".format(tokens.popleft()[1])
    o += parse_term(tokens)
    return o

# factor: var_name | const_value | `(` expr `)`
def parse_factor(tokens):
    # Constants are sequences of digits. If more than one digit, the first digit cannot be 0
    if tokens[0][0] == TOKEN_NAME or tokens[0][0] == TOKEN_INTEGER or tokens[0][0] == TOKEN_ZERO:
        return tokens.popleft()[1]
    if tokens[0][0] == TOKEN_OPEN_BRACKET:
        o = tokens.popleft()[1]
        o += parse_expr(tokens)
        token = tokens.popleft()
        if not token[0] == TOKEN_CLOSE_BRACKET:
            raise Exception("Expected ')' after '{}'!".format(o))
        o += token[1]
        return o
    return None

def validate_program(ast):
    # Two procedures with the same name is considered an error.
    procedure_calls = {}
    for procedure in ast[1]:
        name = procedure[1][0]
        if name in procedure_calls:
            raise Exception("There are two procedures with the same name '{}'!".format(name))
        procedure_calls[name] = set()

    # Call to a non-existing procedure produces an error.
    for procedure in ast[1]:
        p_name = procedure[1][0]
        data = (procedure_calls, p_name)
        validate_program_calls(procedure[1][1][1], data)

    # Recursive and cyclic calls are not allowed. For example, procedure A calls procedure B, procedure B calls C, and C calls A should not be accepted in a correct SIMPLE code.
    visited = set()
    for p_name in procedure_calls.keys():
        path = collections.OrderedDict()
        path[p_name] = 0
        node = (p_name, path)
        data = (procedure_calls, visited)
        validate_program_cycle(node, data)

def validate_program_calls(statement_list, data):
    procedure_calls = data[0]
    p_name = data[1]
    for s in statement_list:
        if s[0] == NODE_STATEMENT_CALL:
            c_name = s[1][0]
            if c_name not in procedure_calls:
                raise Exception("Calling a non-existent procedure '{}'!".format(c_name))
            procedure_calls[p_name].add(c_name)
        elif s[0] == NODE_STATEMENT_IF:
            validate_program_calls(s[1][1][1], data)
            validate_program_calls(s[1][2][1], data)
        elif s[0] == NODE_STATEMENT_WHILE:
            validate_program_calls(s[1][1][1], data)

def validate_program_cycle(node, data):
    p_name = node[0]
    path = node[1]
    procedure_calls = data[0]
    visited = data[1]
    if p_name in visited:
        return
    for c_name in procedure_calls[p_name]:
        # Cycle detected in path!
        if c_name in path:
            index = path[c_name]
            cycle = list(path.keys())[index:]
            cycle.append(c_name)
            raise Exception("A cyclic procedure call was found! {}".format(" -> ".join(cycle)))
        new_path = collections.OrderedDict(path)
        new_path[c_name] = len(new_path)
        new_node = (c_name, new_path)
        validate_program_cycle(new_node, data)
    visited.add(p_name)



# Check that all .src files contain valid grammar
def check():
    source_paths = get_source_paths()
    if len(source_paths) == 0:
        printwarn("No source files were found!")
        return

    printinfo("Checking .src files...")
    for index, source_path in enumerate(source_paths):
        source_full_path = "{}.{}".format(source_path, SOURCE_EXTENSION)

        with open(source_full_path) as f:
            content = f.read()

        path = os.path.relpath(source_full_path)
        printinfoaccent("Checking ({}/{}): '{}'".format(index+1, len(source_paths), path))

        try:
            ast = parse_source(content)
        except Exception as err:
            printwarn("Invalid grammar! :(\nError Message: {}".format(err))
            continue

        printinfo("Valid grammar :)\n")

    printinfoaccent("All checking done! :)\n")

# Generate .lab files from existing .src files
def label():
    source_paths = get_source_paths()
    if len(source_paths) == 0:
        printwarn("No source files were found!")
        return

    printinfo("Generating .lab files...")
    for index, source_path in enumerate(source_paths):
        source_full_path = "{}.{}".format(source_path, SOURCE_EXTENSION)
        label_full_path = "{}.{}".format(source_path, LABEL_EXTENSION)

        with open(source_full_path) as f:
            content = f.read()

        try:
            ast = parse_source(content)
        except:
            continue

        path = os.path.relpath(label_full_path)
        printinfoaccent("Labelling ({}/{}): '{}'".format(index+1, len(source_paths), path))

        label_string = ast_to_label(ast)
        with open(label_full_path, "w") as l:
            l.write("{}\n".format(label_string))

    printinfoaccent("All labelling done! :)\n")

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

def ast_to_label(ast):
    lines = ast_to_lines(ast)
    result = []
    n = 0
    for b, i, v in lines:
        indent = LABEL_INDENTATION * i
        if b:
            n += 1
            result.append("{:03d} {}{}".format(n, indent, v))
        else:
            result.append("--- {}{}".format(indent, v))
    return "\n".join(result)

# Creates compiled system test files for submission!
def publish():
    # Create publish directory
    if not os.path.exists(PUBLISH_OUTPUT_DIRECTORY):
        os.mkdir(PUBLISH_OUTPUT_DIRECTORY)

    # Check publish directory
    if not os.path.isdir(PUBLISH_OUTPUT_DIRECTORY):
        printerr("{} is not a directory! Please delete it.".format(PUBLISH_OUTPUT_DIRECTORY))

    source_paths = get_source_paths()
    if len(source_paths) == 0:
        printwarn("No source files were found!")
        return

    # Clean up previous txt files
    printinfo("Cleaning up previous txt files...")
    files = glob.glob("{}/*.txt".format(PUBLISH_OUTPUT_DIRECTORY))
    for f in files:
        os.unlink(f)

    total_queries = 0
    deps = get_deps_paths(source_paths)
    for index, (source_path, deps_paths) in enumerate(deps):
        source_full_path = "{}.{}".format(source_path, SOURCE_EXTENSION)

        # Write files to PUBLISH_OUTPUT_DIRECTORY
        output_name = os.path.relpath(source_path, tests_dir_path)
        output_name = os.path.normpath(output_name)
        output_name = "-".join(output_name.split(os.sep))
        output_qry_full_path = "{}{}".format(output_name, PUBLISH_QRY_EXTENSION)
        output_qry_full_path = os.path.join(PUBLISH_OUTPUT_DIRECTORY, output_qry_full_path)
        output_qry_full_path = os.path.abspath(output_qry_full_path)
        output_src_full_path = "{}{}".format(output_name, PUBLISH_SRC_EXTENSION)
        output_src_full_path = os.path.join(PUBLISH_OUTPUT_DIRECTORY, output_src_full_path)
        output_src_full_path = os.path.abspath(output_src_full_path)

        printinfoaccent("Publishing test ({}/{}): {}".format(index+1, len(deps), output_name))

        # Read files
        with open(source_full_path) as s:
            src_content = s.read()

        qry_lines = []
        for queries_path in deps_paths:
            queries_full_path = "{}.{}".format(queries_path, QUERIES_EXTENSION)
            with open(queries_full_path) as q:
                for line in q:
                    line = line.strip()
                    qry_lines.append(line)
            while qry_lines[-1] == "":
                qry_lines.pop()

        # Replace comment numbers
        count = 0
        for i in range(0, len(qry_lines), 5):
            count += 1
            line = qry_lines[i]
            line = re.sub(r"^\d+(.*)", r"\1", line)
            qry_lines[i] = "{}{}".format(count, line)
        qry_string = "\n".join(qry_lines)
        total_queries += count

        # Write files
        with open(output_src_full_path, "w") as s:
            s.write(src_content)

        with open(output_qry_full_path, "w") as q:
            q.write("{}\n".format(qry_string))

        printinfo("Queries: {}\n".format(count))

    printinfoaccent("All tests published! :) Please check '{}' directory for all published files!".format(os.path.relpath(PUBLISH_OUTPUT_DIRECTORY)))
    printinfo("Total queries published: {}\n".format(total_queries))

# Tests all .src files using AutoTester
def run():
    autotester_path = find_autotester_path()
    source_paths = get_source_paths()
    if len(source_paths) == 0:
        printwarn("No source files were found!")
        return

    tests = []
    deps = get_deps_paths(source_paths)
    for source_path, deps_paths in deps:
        for queries_path in deps_paths:
            tests.append((source_path, queries_path))

    if len(tests) == 0:
        printwarn("No query files were found!")
        return

    # Clean up previous xml files
    printinfo("Cleaning up previous xml files...")
    files = glob.glob("{}/*.xml".format(OUTPUT_DIRECTORY))
    for f in files:
        os.unlink(f)

    # Test each source file with it's queries
    printinfo("Testing all source files...")

    total_queries = 0
    total_queries_passed = 0
    for index, (source_path, queries_path) in enumerate(tests):
        source_full_path = "{}.{}".format(source_path, SOURCE_EXTENSION)
        queries_full_path = "{}.{}".format(queries_path, QUERIES_EXTENSION)
        test_no = index+1

        # Output Name Format: Source Path (Relative Query Path)
        s_name = os.path.relpath(source_path, tests_dir_path)
        s_name = os.path.normpath(s_name)
        s_name = "-".join(s_name.split(os.sep))
        q_name = os.path.relpath(queries_path, os.path.dirname(source_path))
        q_name = os.path.normpath(q_name)
        q_name = "-".join(q_name.split(os.sep))
        output_full_path = "{}. {} ({}).xml".format(test_no, s_name, q_name)
        output_full_path = os.path.join(OUTPUT_DIRECTORY, output_full_path)
        output_full_path = os.path.abspath(output_full_path)

        # Run AutoTester
        printinfoaccent("Running test ({}/{}): {} ({})".format(test_no, len(tests), s_name, q_name))
        process = subprocess.run([autotester_path, source_full_path, queries_full_path, output_full_path], stdout=subprocess.DEVNULL)

        if not process.returncode == 0:
            printerr("'{}' returned with non-zero exit code :(\nFull Command: {} {} {} {}"
                .format(AUTOTESTER_FILE_NAME, autotester_path, source_full_path, queries_full_path, output_full_path))

        # Parse output xml
        queries = ElementTree.parse(output_full_path).find("queries")
        num_passed = 0
        num_queries = len(queries)
        for query in queries:
            if query.find("passed") is not None:
                num_passed += 1
        printinfo("Passed queries: {}/{}\n".format(num_passed, num_queries))

        # Update total queries
        total_queries_passed += num_passed
        total_queries += num_queries

    if total_queries == 0:
        printwarn("No query file contained any queries!")
        return

    printinfoaccent("All tests done! :) Please check '{}' directory for all AutoTester outputs!".format(os.path.relpath(OUTPUT_DIRECTORY)))

    percentage = total_queries_passed / total_queries
    printinfo("Total passed queries: {}/{} ({:.0%})\n".format(total_queries_passed, total_queries, percentage))

# Summarize 'coverage' of all .qry and .src files
def summarize():
    source_paths = get_source_paths()
    if len(source_paths) == 0:
        printwarn("No source files were found!")
        return

    printinfo("Summarizing .src files...")
    for index, source_path in enumerate(source_paths):
        source_full_path = "{}.{}".format(source_path, SOURCE_EXTENSION)

        with open(source_full_path) as f:
            content = f.read()

        try:
            ast = parse_source(content)
        except:
            continue

        path = os.path.relpath(source_full_path)
        printinfoaccent("Summarizing ({}/{}): '{}'".format(index+1, len(source_paths), path))

        # Summarize AST
        summary = summarize_ast(ast)

        # Format and print outputs :)
        infos = []
        abstracts = {
            NODE_PROCEDURE : "Procedure",
            SUMMARY_STATEMENT : "Statement",
            SUMMARY_CONTAINER : "Container"
        }
        for k, v in abstracts.items():
            count = summary[k]
            infos.append("{}(s): {}".format(v, count))
        printinfo("{}".format(", ".join(infos)))

        infos = []
        statements = {
            NODE_STATEMENT_READ : "Read",
            NODE_STATEMENT_PRINT : "Print",
            NODE_STATEMENT_CALL : "Call",
            NODE_STATEMENT_WHILE : "While",
            NODE_STATEMENT_IF : "If",
            NODE_STATEMENT_ASSIGN : "Assign",
        }
        for k, v in statements.items():
            count = summary[k]
            infos.append("{}(s): {}".format(v, count))
        printinfo("{}\n".format(", ".join(infos)))

    printinfoaccent("All summarizing done! :)\n")

def summarize_ast(ast):
    result = collections.Counter()
    if ast[0] == NODE_PROGRAM:
        for c in ast[1]:
            result.update(summarize_ast(c))
    elif ast[0] == NODE_PROCEDURE:
        result[NODE_PROCEDURE] += 1
        result.update(summarize_ast(ast[1][1]))
    elif ast[0] == NODE_STATEMENT_LIST:
        for c in ast[1]:
            result.update(summarize_ast(c))
    elif ast[0] == NODE_STATEMENT_READ:
        result[SUMMARY_STATEMENT] += 1
        result[NODE_STATEMENT_READ] += 1
    elif ast[0] == NODE_STATEMENT_PRINT:
        result[SUMMARY_STATEMENT] += 1
        result[NODE_STATEMENT_PRINT] += 1
    elif ast[0] == NODE_STATEMENT_CALL:
        result[SUMMARY_STATEMENT] += 1
        result[NODE_STATEMENT_CALL] += 1
    elif ast[0] == NODE_STATEMENT_WHILE:
        result[SUMMARY_STATEMENT] += 1
        result[SUMMARY_CONTAINER] += 1
        result[NODE_STATEMENT_WHILE] += 1
        result.update(summarize_ast(ast[1][1]))
    elif ast[0] == NODE_STATEMENT_IF:
        result[SUMMARY_STATEMENT] += 1
        result[SUMMARY_CONTAINER] += 1
        result[NODE_STATEMENT_IF] += 1
        result.update(summarize_ast(ast[1][1]))
        result.update(summarize_ast(ast[1][2]))
    elif ast[0] == NODE_STATEMENT_ASSIGN:
        result[SUMMARY_STATEMENT] += 1
        result[NODE_STATEMENT_ASSIGN] += 1
    return result



### Main
def sigint_handler(s, f):
    printinfo("SIGINT caught, exiting! :)")
    sys.exit()
signal.signal(signal.SIGINT, sigint_handler)

# Fixes colors not showing on cmd
os.system("color")

# Ensure python is 3.5.0 or higher
if not (sys.version_info.major >= 3 and sys.version_info.minor >= 5):
    printerr("Please run this script with python 3.5.0 or higher!")

# Parse command arguments
parser = argparse.ArgumentParser()
parser.add_argument("-c", "--check", action="store_true", help="Check that all .src files contain valid grammar")
parser.add_argument("-l", "--label", action="store_true", help="Generate .lab files from existing .src files")
parser.add_argument("-p", "--publish", action="store_true", help="Creates compiled system test files for submission!")
parser.add_argument("-r", "--run", action="store_true", help="Tests all .src files using AutoTester")
parser.add_argument("-s", "--summarize", action="store_true", help="Summarize 'coverage' of all .qry and .src files")
parser.add_argument("tests_dir_path", help="Path of directory containing tests")
parser.add_argument("-d", "--depth", type=int, help="Maximum folder depth of source files to include.")
parser.add_argument("-f", "--filter", help="Name of source files to include.")
args = parser.parse_args()

# If no options are selected, run by default
if not (args.check or args.label or args.publish or args.run or args.summarize):
    args.run = True

# Check tests_path
tests_dir_path = args.tests_dir_path
tests_dir_path = os.path.abspath(tests_dir_path)
max_source_depth = args.depth
source_filter = args.filter
if not os.path.isdir(tests_dir_path):
    printerr("'{}' is not a directory!".format(tests_dir_path))

# Create output directory
if not os.path.exists(OUTPUT_DIRECTORY):
    os.mkdir(OUTPUT_DIRECTORY)

# Check output directory
if not os.path.isdir(OUTPUT_DIRECTORY):
    printerr("{} is not a directory! Please delete it.".format(OUTPUT_DIRECTORY))

# Run options
if args.check:
    check()

if args.label:
    label()

if args.publish:
    publish()

if args.run:
    run()

if args.summarize:
    summarize()
