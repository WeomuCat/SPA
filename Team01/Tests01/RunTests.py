#
# A friendly script to help with system testing :)
#

import argparse
import collections
import os
import re
import subprocess
import sys



AUTOTESTER_DIRECTORIES = [".", "../Code01/Debug"]
AUTOTESTER_FILE_NAME = "AutoTester.exe"
OUTPUT_DIRECTORY = "./RunTestsOutput"
PUBLISH_OUTPUT_DIRECTORY = os.path.join(OUTPUT_DIRECTORY, "Publish")

QUERIES_EXTENSION = "qry"
SOURCE_EXTENSION = "src"
SOURCE_DEPENDENCIES_EXTENSION = "dep"
LABEL_EXTENSION = "lab"
PUBLISH_QRY_EXTENSION = "_queries.txt"
PUBLISH_SRC_EXTENSION = "_source.txt"

WARNING_COLOR = "\033[93m"
ERROR_COLOR = "\033[91m"
END_COLOR = "\033[0m"

LABEL_INDENTATION = "    "

TOKEN_NAME = 0
TOKEN_INTEGER = 1
TOKEN_OPEN_BRACKET = 2
TOKEN_CLOSE_BRACKET = 3
TOKEN_OPEN_BRACE = 4
TOKEN_CLOSE_BRACE = 5
TOKEN_SEMICOLON = 6
TOKEN_EXPR = 7
TOKEN_TERM = 8
TOKEN_REL_EXPR = 9
TOKEN_NOT = 10
TOKEN_AND = 11
TOKEN_OR = 12
TOKEN_EQUALS = 13
TOKEN_EOF = 14

NODE_PROGRAM = 0
NODE_PROCEDURE = 1
NODE_STATEMENT_LIST = 2
NODE_STATEMENT_READ = 3
NODE_STATEMENT_PRINT = 4
NODE_STATEMENT_CALL = 5
NODE_STATEMENT_WHILE = 6
NODE_STATEMENT_IF = 7
NODE_STATEMENT_ASSIGN = 8

TOKENS_PATTERN = {
    "[A-Za-z][A-Za-z\d]*": TOKEN_NAME,
    "\d+": TOKEN_INTEGER,
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
    source_paths = []
    # BFS
    depth = 0
    queue = collections.deque()
    queue.append(tests_dir_path)
    while True:
        # No more directories to search
        if len(queue) == 0:
            return source_paths
        # The maximum depth is non-negative, and
        # The current depth exceeds the maximum depth
        if max_source_depth >= 0 and depth > max_source_depth:
            return source_paths
        # Get current path
        root_path = queue.popleft()
        # Find all files and directories in the current path
        for file_name in os.listdir(root_path):
            path = os.path.join(root_path, file_name)
            if os.path.isfile(path):
                match = re.search("^(.+)\\.([^.]+)$", file_name)
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
    if tokens[0][0] == TOKEN_NAME or tokens[0][0] == TOKEN_INTEGER:
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
    if tokens[0][0] == TOKEN_NAME or tokens[0][0] == TOKEN_INTEGER:
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

    # Constants are sequences of digits. If more than one digit, the first digit cannot be 0
    # TODO: Upgrade parser

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
        printinfo("Checking ({}/{}): '{}'".format(index+1, len(source_paths), path))

        try:
            ast = parse_source(content)
        except Exception as err:
            printwarn("Invalid grammar!\nError Message: {}".format(err))
            continue

        printinfo("Valid grammar :)")

    printinfo("All checking done! :)")

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
        printinfo("Labelling ({}/{}): '{}'".format(index+1, len(source_paths), path))
        statements = []
        for i, p in enumerate(ast[1]):
            if i != 0:
                statements.append((False, 0, ""))
            v = "procedure {} {{".format(p[1][0])
            statements.append((False, 0, v))
            statements.extend(label_statements(p[1][1][1], 1))
            statements.append((False, 0, "}"))

        lines = []
        n = 0
        for b, i, v in statements:
            indent = LABEL_INDENTATION * i
            if b:
                n += 1
                lines.append("{:03d} {}{}".format(n, indent, v))
            else:
                lines.append("--- {}{}".format(indent, v))

        with open(label_full_path, "w") as l:
            l.write("\n".join(lines) + "\n")

    printinfo("All labelling done! :)")

def label_statements(statement_list, indent):
    result = []
    for s in statement_list:
        if s[0] == NODE_STATEMENT_READ:
            v = "read {};".format(s[1][0])
            result.append((True, indent, v))
        elif s[0] == NODE_STATEMENT_PRINT:
            v = "print {};".format(s[1][0])
            result.append((True, indent, v))
        elif s[0] == NODE_STATEMENT_CALL:
            v = "call {};".format(s[1][0])
            result.append((True, indent, v))
        elif s[0] == NODE_STATEMENT_WHILE:
            v = "while ({}) {{".format(s[1][0])
            result.append((True, indent, v))
            result.extend(label_statements(s[1][1][1], indent + 1))
            result.append((False, indent, "}"))
        elif s[0] == NODE_STATEMENT_IF:
            v = "if ({}) then {{".format(s[1][0])
            result.append((True, indent, v))
            result.extend(label_statements(s[1][1][1], indent + 1))
            result.append((False, indent, "} else {"))
            result.extend(label_statements(s[1][2][1], indent + 1))
            result.append((False, indent, "}"))
        elif s[0] == NODE_STATEMENT_ASSIGN:
            v = "{} = {};".format(s[1][0], s[1][1])
            result.append((True, indent, v))
    return result

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

    deps = get_deps_paths(source_paths)
    for index, (source_path, deps_paths) in enumerate(deps):
        source_full_path = "{}.{}".format(source_path, SOURCE_EXTENSION)

        qry_lines = []
        for queries_path in deps_paths:
            queries_full_path = "{}.{}".format(queries_path, QUERIES_EXTENSION)
            with open(queries_full_path) as q:
                for line in q:
                    line = line.strip()
                    if line == "":
                        break
                    qry_lines.append(line)

        # Replace comment numbers
        count = 1
        for i in range(0, len(qry_lines), 5):
            line = qry_lines[i]
            line = re.sub(r"^\d+(.*)", r"\1", line)
            qry_lines[i] = "{}{}".format(count, line)
            count += 1

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

        printinfo("Publishing test ({}/{}): {}".format(index+1, len(deps), output_name))
        with open(source_full_path) as s:
            src_content = s.read()

        with open(output_src_full_path, "w") as s:
            s.write(src_content)

        with open(output_qry_full_path, "w") as q:
            q.write("\n".join(qry_lines) + "\n")

    printinfo("All tests published! :) Please check '{}' directory for all published files!".format(os.path.relpath(PUBLISH_OUTPUT_DIRECTORY)))

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

    # Test each source file with it's queries
    printinfo("Testing all source files...")
    for index, (source_path, queries_path) in enumerate(tests):
        source_full_path = "{}.{}".format(source_path, SOURCE_EXTENSION)
        queries_full_path = "{}.{}".format(queries_path, QUERIES_EXTENSION)

        # Output Name Format: Source Path (Relative Query Path)
        s_name = os.path.relpath(source_path, tests_dir_path)
        s_name = os.path.normpath(s_name)
        s_name = "-".join(s_name.split(os.sep))
        q_name = os.path.relpath(queries_path, os.path.dirname(source_path))
        q_name = os.path.normpath(q_name)
        q_name = "-".join(q_name.split(os.sep))
        output_full_path = "{} ({}).xml".format(s_name, q_name)
        output_full_path = os.path.join(OUTPUT_DIRECTORY, output_full_path)
        output_full_path = os.path.abspath(output_full_path)

        # Run AutoTester
        printinfo("Running test ({}/{}): {} ({})".format(index+1, len(tests), s_name, q_name))
        process = subprocess.run([autotester_path, source_full_path, queries_full_path, output_full_path], stdout=subprocess.DEVNULL)

        if not process.returncode == 0:
            printerr("'{}' returned with non-zero exit code :(\nFull Command: {} {} {} {}"
            .format(AUTOTESTER_FILE_NAME, autotester_path, source_full_path, queries_full_path, output_full_path))

    printinfo("All tests done! :) Please check '{}' directory for all AutoTester outputs!".format(os.path.relpath(OUTPUT_DIRECTORY)))

# Summarize 'coverage' of all .qry and .src files
def summarize():
    printerr("This function is not implemented yet :)")



### Main

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
parser.add_argument("-d", "--depth", type=int, default=-1, help="Maximum folder depth of source files to include.")
args = parser.parse_args()

# If no options are selected, run by default
if not (args.check or args.label or args.publish or args.run or args.summarize):
    args.run = True

# Check tests_path
tests_dir_path = args.tests_dir_path
tests_dir_path = os.path.abspath(tests_dir_path)
max_source_depth = args.depth
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