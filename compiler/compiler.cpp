#include "ack.cpp"

#include <cstdio>

// @lexer
enum TokenType {
    // words
    TT_Identifier,
    TT_Number,

    // keywords
    TT_Let,
    TT_Return,
    TT_Fn,

    // singles
    TT_Colon,
    TT_Equals,
    TT_Semicolon,
    TT_Plus,
    TT_Minus,
    TT_Asterisk,
    TT_Paren_Open,
    TT_Paren_Close,
    TT_Brace_Open,
    TT_Brace_Close,
};

struct Token {
    TokenType type;
    string source;
};

bool is_alpha(u8 c);
bool is_digit(char c);
bool is_keyword(string s, TokenType *type);
slice<Token> lex(Arena *arena, string source);

string tokens_to_string(Arena *arena, slice<Token> tokens);
string token_type_to_string(TokenType type);

bool is_alpha(u8 c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

bool is_keyword(string s, TokenType *type) {
    if (slice_memcmp(s, slice<u8>("let"))) {
        *type = TT_Let;
        return true;
    }

    if (slice_memcmp(s, slice<u8>("return"))) {
        *type = TT_Return;
        return true;
    }

    if (slice_memcmp(s, slice<u8>("fn"))) {
        *type = TT_Fn;
        return true;
    }

    return false;
}

slice<Token> lex(Arena *arena, string source) {
    DynamicArray<Token> tokens = dynamic_array_create<Token>(arena, source.len);

    i32 i = 0;

    while (i < source.len) {
        u8 c = source[i];

        switch (c) {
            case '\n':
            case '\t':
            case '\r':
            case ' ':
                i++;
                continue;
            case ':': {
                string s = slice_range(source, i, i + 1);
                Token token = {.type = TT_Colon, .source = s};

                append(&tokens, token);

                i++;
                continue;
            } break;
            case '=': {
                string s = slice_range(source, i, i + 1);
                Token token = {.type = TT_Equals, .source = s};

                append(&tokens, token);

                i++;
                continue;
            } break;
            case ';': {
                string s = slice_range(source, i, i + 1);
                Token token = {.type = TT_Semicolon, .source = s};

                append(&tokens, token);

                i++;
                continue;
            } break;
            case '+': {
                string s = slice_range(source, i, i + 1);
                Token token = {.type = TT_Plus, .source = s};

                append(&tokens, token);

                i++;
                continue;
            } break;
            case '-': {
                string s = slice_range(source, i, i + 1);
                Token token = {.type = TT_Minus, .source = s};

                append(&tokens, token);

                i++;
                continue;
            } break;
            case '*': {
                string s = slice_range(source, i, i + 1);
                Token token = {.type = TT_Asterisk, .source = s};

                append(&tokens, token);

                i++;
                continue;
            } break;
            case '(': {
                string s = slice_range(source, i, i + 1);
                Token token = {.type = TT_Paren_Open, .source = s};

                append(&tokens, token);

                i++;
                continue;
            } break;
            case ')': {
                string s = slice_range(source, i, i + 1);
                Token token = {.type = TT_Paren_Close, .source = s};

                append(&tokens, token);

                i++;
                continue;
            } break;
            case '{': {
                string s = slice_range(source, i, i + 1);
                Token token = {.type = TT_Brace_Open, .source = s};

                append(&tokens, token);

                i++;
                continue;
            } break;
            case '}': {
                string s = slice_range(source, i, i + 1);
                Token token = {.type = TT_Brace_Close, .source = s};

                append(&tokens, token);

                i++;
                continue;
            } break;
        }

        if (is_digit(c)) {
            i32 start = i;

            while (i < source.len && is_digit(source[i])) {
                i++;
            }

            string s = slice_range(source, start, i);
            Token token = {.type = TT_Number, .source = s};

            append(&tokens, token);

            continue;
        }

        if (is_alpha(c)) {
            i32 start = i;

            while (i < source.len && (is_alpha(source[i]) || is_digit(source[i]))) {
                i++;
            }

            string s = slice_range(source, start, i);

            TokenType type;
            if (is_keyword(s, &type)) {
                Token token = {.type = type, .source = s};
                append(&tokens, token);
            } else {
                Token token = {.type = TT_Identifier, .source = s};
                append(&tokens, token);
            }

            continue;
        }

        printf("Char: '%c'\n", c);
        Unreachable("Lexer failed with above character");
    }

    return to_slice(&tokens);
}

string tokens_to_string(Arena *arena, slice<Token> tokens) {
    DynamicArray<u8> bytes = dynamic_array_create<u8>(arena, 1024);

    for (i32 i = 0; i < tokens.len; i++) {
        Token token = tokens[i];
        fmt(&bytes, "{}: '{}'\n", token_type_to_string(token.type), token.source);
    }

    return to_slice(&bytes);
}

string token_type_to_string(TokenType type) {
    switch (type) {
        case TT_Identifier: return "Identifier";
        case TT_Number:     return "Number";
        case TT_Let:        return "Let";
        case TT_Return:     return "Return";
        case TT_Fn:         return "Fn";
        case TT_Colon:      return "Colon";
        case TT_Equals:     return "Equals";
        case TT_Semicolon:  return "Semicolon";
        case TT_Plus:       return "Plus";
        case TT_Minus:      return "Minus";
        case TT_Asterisk:   return "Asterisk";
        case TT_Paren_Open: return "Paren_Open";
        case TT_Paren_Close:return "Paren_Close";
        case TT_Brace_Open: return "Brace_Open";
        case TT_Brace_Close:return "Brace_Close";
        default:            Unreachable("unsupported token type in token_type_to_string");
    }

    return "";
}

// @parser
struct ASTNode;

struct NumberLiteralASTNode {
    Token value;
};

struct IdentifierASTNode {
    Token name;
};

struct BinaryASTNode {
    ASTNode *left;
    Token op;
    ASTNode *right;
};

struct FunctionASTNode {
    Token name;
    array<Token, 4> parameters;
    slice<ASTNode *> body;
};

struct ReturnASTNode {
    ASTNode *node;
};

enum ASTNodeType {
    // expressions
    NT_NumberLiteral,
    NT_Identifier,
    NT_Binary,

    // statements
    NT_Function,
    NT_Return,
};

struct ASTNode {
    ASTNodeType type;

    union {
        // expressions
        NumberLiteralASTNode number_literal;
        IdentifierASTNode identifier;
        BinaryASTNode binary;

        // statements
        FunctionASTNode function;
        ReturnASTNode ret;
    };
};

struct AST {
    ASTNode *root;
};

struct Parser {
    Arena *arena;

    i64 position;
    slice<Token> tokens;
};

AST parse(Arena *arena, slice<Token> tokens);

ASTNode *parse_node(Parser *parser);
ASTNode *parse_function(Parser *parser);
ASTNode *parse_return(Parser *parser);

ASTNode *parse_expression(Parser *parser);
ASTNode *parse_binary_1(Parser *parser);
ASTNode *parse_literal(Parser *parser);
Token parser_next(Parser *parser);
bool parser_is_next(Parser *parser, TokenType type);

string ast_to_string(Arena *arena, AST *ast);
string node_to_string(DynamicArray<u8> *bytes, ASTNode *node, i32 indent_level);

AST parse(Arena *arena, slice<Token> tokens) {
    Parser parser = {
        .arena = arena,
        .position = 0,
        .tokens = tokens,
    };

    ASTNode *root = parse_node(&parser);
    Assert(root);
    Assert(root->type == NT_Function);

    AST ast = {
        .root = root,
    };

    return ast;

}

ASTNode *parse_node(Parser *parser) {
    if (parser_is_next(parser, TT_Fn)) {
        return parse_function(parser);
    }

    if (parser_is_next(parser, TT_Return)) {
        return parse_return(parser);
    }

    Unreachable("unexpected token in parse_node");
    return NULL;
}

ASTNode *parse_function(Parser *parser) {
    ASTNode *node = arena_alloc<ASTNode>(parser->arena);
    DynamicArray<ASTNode *> body = dynamic_array_create<ASTNode *>(parser->arena, 0);

    Token fn = parser_next(parser);
    Assert(fn.type == TT_Fn);

    Token name = parser_next(parser);
    Assert(name.type == TT_Identifier);

    Token paren_open = parser_next(parser);
    Assert(paren_open.type == TT_Paren_Open);

    Token param0 = parser_next(parser);
    Assert(param0.type == TT_Identifier);

    Token param1 = parser_next(parser);
    Assert(param1.type == TT_Identifier);

    Token param2 = parser_next(parser);
    Assert(param2.type == TT_Identifier);

    Token param3 = parser_next(parser);
    Assert(param3.type == TT_Identifier);

    Token paren_close = parser_next(parser);
    Assert(paren_close.type == TT_Paren_Close);

    Token brace_open = parser_next(parser);
    Assert(brace_open.type == TT_Brace_Open);

    while (!parser_is_next(parser, TT_Brace_Close)) {
        ASTNode *body_node = parse_node(parser);
        Assert(body_node);

        append(&body, body_node);
    }

    Token brace_close = parser_next(parser);
    Assert(brace_close.type == TT_Brace_Close);

    node->type = NT_Function;
    node->function.name = name;
    node->function.parameters[0] = param0;
    node->function.parameters[1] = param1;
    node->function.parameters[2] = param2;
    node->function.parameters[3] = param3;
    node->function.body = to_slice(&body);

    return node; 
}

ASTNode *parse_return(Parser *parser) {
    Token token = parser_next(parser);
    Assert(token.type == TT_Return);

    ASTNode *node = parse_expression(parser);
    Assert(node);

    ASTNode *return_node = arena_alloc<ASTNode>(parser->arena);
    return_node->type = NT_Return;
    return_node->ret.node = node;

    Token semi_colon = parser_next(parser);
    Assert(semi_colon.type == TT_Semicolon);

    return return_node;
}

ASTNode *parse_expression(Parser *parser) {
    return parse_binary_1(parser);
}

ASTNode *parse_binary_1(Parser *parser) {
    ASTNode *expression = parse_literal(parser);

    while (parser_is_next(parser, TT_Plus) || parser_is_next(parser, TT_Minus)) {
        Token op = parser_next(parser);
        ASTNode *right = parse_literal(parser);

        ASTNode *binary = arena_alloc<ASTNode>(parser->arena);
        binary->type = NT_Binary;
        binary->binary.left = expression;
        binary->binary.op = op;
        binary->binary.right = right;

        expression = binary;
    }

    return expression;
}

ASTNode *parse_literal(Parser *parser) {
    if (parser_is_next(parser, TT_Number)) {
        Token token = parser_next(parser);

        ASTNode *node = arena_alloc<ASTNode>(parser->arena);
        node->type = NT_NumberLiteral;
        node->number_literal.value = token;

        return node;
    }

    if (parser_is_next(parser, TT_Identifier)) {
        Token token = parser_next(parser);

        ASTNode *node = arena_alloc<ASTNode>(parser->arena);
        node->type = NT_Identifier;
        node->identifier.name = token;

        return node;
    }

    Unreachable("unexpected token in parse_literal");
    return NULL;
}

Token parser_next(Parser *parser) {
    Token token = parser->tokens[parser->position];
    parser->position += 1;

    return token;
}

bool parser_is_next(Parser *parser, TokenType type) {
    if (parser->position >= parser->tokens.len) {
        return false;
    }

    return parser->tokens[parser->position].type == type;
}

string ast_to_string(Arena *arena, AST *ast) {
    DynamicArray<u8> bytes = dynamic_array_create<u8>(arena, 1024);

    node_to_string(&bytes, ast->root, 0);

    return to_slice(&bytes);
}

string node_to_string(DynamicArray<u8> *bytes, ASTNode *node, i32 indent_level) {
    static const auto indent = [](DynamicArray<u8> *bytes, i32 level) {
        for (i32 i = 0; i < level; i++) {
            if (i == level - 1) {
                fmt(bytes, "+ ");
            } else {
                fmt(bytes, "| ");
            }
        }
    };

    switch (node->type) {
        case NT_NumberLiteral: {
            indent(bytes, indent_level);
            fmt(bytes, "Number Literal: {}\n", node->number_literal.value.source);
        } break;
        case NT_Identifier: {
            indent(bytes, indent_level);
            fmt(bytes, "Identifier: {}\n", node->identifier.name.source);
        } break;
        case NT_Binary: {
            indent(bytes, indent_level);
            fmt(bytes, "Binary:\n");

            node_to_string(bytes, node->binary.left, indent_level + 1);

            indent(bytes, indent_level + 1);
            fmt(bytes, "Op: {}\n", node->binary.op.source);

            node_to_string(bytes, node->binary.right, indent_level + 1);
        } break;
        case NT_Function: {
            indent(bytes, indent_level);
            fmt(bytes, "Fn {}:\n", node->function.name.source);

            for (ASTNode *statement : node->function.body) {
                node_to_string(bytes, statement, indent_level + 1);
            }
        } break;
        case NT_Return: {
            indent(bytes, indent_level);
            fmt(bytes, "Return:\n");

            node_to_string(bytes, node->ret.node, indent_level + 1);
        } break;
        default:
            Unreachable("unsupported expression type in ast_to_string");
    }

    return to_slice(bytes);
}

// @ir
enum InstructionType {
    IT_StartFunction,
    IT_EndFunction,
    IT_Push,
    IT_Add,
    IT_Local,
    IT_Return,
};

struct Instruction {
    InstructionType type;
    i32 value;
    string string;
};

struct IR {
    array<Token, 4> parameters;
    DynamicArray<Instruction> instructions;
};

IR ir_gen(Arena *arena, AST *ast);

void ir_gen_node(IR *ir, ASTNode *node);

void ir_gen_number_literal(IR *ir, ASTNode *node);
void ir_gen_identifier(IR *ir, ASTNode *node);
void ir_gen_binary(IR *ir, ASTNode *node);

void ir_gen_function(IR *ir, ASTNode *node);
void ir_gen_return(IR *ir, ASTNode *node);

i32 ir_get_parameter_index(IR *ir, string name);

string ir_to_string(Arena *arena, slice<Instruction> instructions);

IR ir_gen(Arena *arena, AST *ast) {
    Assertf(ast->root->type == NT_Function, "ir_gen only supports function root nodes");

    IR ir = {
        .parameters = ast->root->function.parameters,
        .instructions = dynamic_array_create<Instruction>(arena, 1024),
    };

    ir_gen_node(&ir, ast->root);

    return ir;
}

void ir_gen_node(IR *ir, ASTNode *node) {
    switch (node->type) {
        case NT_NumberLiteral:
            ir_gen_number_literal(ir, node);
            break;
        case NT_Identifier:
            ir_gen_identifier(ir, node);
            break;
        case NT_Binary:
            ir_gen_binary(ir, node);
            break;
        case NT_Return: {
            ir_gen_return(ir, node->ret.node);
        } break;
        case NT_Function:
            ir_gen_function(ir, node);
            break;
        default:
            Unreachable("unsupported expression type in ir_gen_node");
    }
}

void ir_gen_number_literal(IR *ir, ASTNode *node) {
    i32 value = atoi((const char *) node->number_literal.value.source.ptr);

    Instruction instruction = {.type = IT_Push, .value = value};
    append(&ir->instructions, instruction);
}

void ir_gen_identifier(IR *ir, ASTNode *node) {
    i32 index = ir_get_parameter_index(ir, node->identifier.name.source);
    if (index == -1) {
        Unreachable("unknown identifier in ir_gen_identifier");
    }

    Instruction instruction = {.type = IT_Local, .value = index};
    append(&ir->instructions, instruction);
}

void ir_gen_binary(IR *ir, ASTNode *node) {
    ir_gen_node(ir, node->binary.left);
    ir_gen_node(ir, node->binary.right);

    switch (node->binary.op.type) {
        case TT_Plus: {
            Instruction instruction = {.type = IT_Add};
            append(&ir->instructions, instruction); 
        } break;
        default:
            Unreachable("unsupported binary operator in ir_gen_binary");
    }
}

void ir_gen_function(IR *ir, ASTNode *node) {
    append(&ir->instructions, {.type = IT_StartFunction, .string = node->function.name.source});

    for (ASTNode *statement : node->function.body) {
        ir_gen_node(ir, statement);
    }

    append(&ir->instructions, {.type = IT_EndFunction, .string = node->function.name.source});
}

void ir_gen_return(IR *ir, ASTNode *node) {
    ir_gen_node(ir, node);

    Instruction instruction = {.type = IT_Return};
    append(&ir->instructions, instruction);
}

i32 ir_get_parameter_index(IR *ir, string name) {
    for (i32 i = 0; i < ir->parameters.size(); i++) {
        if (slice_memcmp(ir->parameters[i].source, name)) {
            return i;
        }
    }

    return -1;
}

string ir_to_string(Arena *arena, IR *ir) {
    DynamicArray<u8> bytes = dynamic_array_create<u8>(arena, 1024);

    for (i32 i = 0; i < ir->instructions.len; i++) {
        Instruction instruction = ir->instructions[i];

        fmt(&bytes, "[{}] ", i);

        switch (instruction.type) {
            case IT_Push: {
                fmt(&bytes, "Push {}\n", instruction.value);
            } break;
            case IT_Add: {
                fmt(&bytes, "Add\n");
            } break;
            case IT_Local: {
                fmt(&bytes, "Local {}\n", instruction.value);
            } break;
            case IT_Return: {
                fmt(&bytes, "Return\n");
            } break;
            case IT_StartFunction: {
                fmt(&bytes, "StartFunction '{}'\n", instruction.string);
            } break;
            case IT_EndFunction: {
                fmt(&bytes, "EndFunction '{}'\n", instruction.string);
            } break;
            default:
                Unreachable("unsupported instruction type in instruction_to_string");
        }
    }

    return to_slice(&bytes);
}

// @asm
string asmgen(Arena *arena, IR *ir);

string asmgen(Arena *arena, IR *ir) {
    DynamicArray<u8> bytes = dynamic_array_create<u8>(arena, 1024);

    slice<Instruction> instructions = to_slice(&ir->instructions);

    fmt(&bytes, ".code\n");

    for (i32 i = 0; i < instructions.len; i++) {
        Instruction instruction = instructions[i];

        fmt(&bytes, "\n; [{}]\n", i);

        switch (instruction.type) {
            case IT_Push: {
                fmt(&bytes, "push {}\n", instruction.value);
            } break;
            case IT_Add: {
                fmt(&bytes, "pop rbx\n");
                fmt(&bytes, "pop rax\n");
                fmt(&bytes, "add rax, rbx\n");
                fmt(&bytes, "push rax\n");
            } break;
            case IT_Local: {
                if (instruction.value == 0) {
                    fmt(&bytes, "push rcx\n");
                } else if (instruction.value == 1) {
                    fmt(&bytes, "push rdx\n");
                } else if (instruction.value == 2) {
                    fmt(&bytes, "push r8\n");
                } else if (instruction.value == 3) {
                    fmt(&bytes, "push r9\n");
                }
            } break;
            case IT_Return: {
                fmt(&bytes, "pop rax\n");
                fmt(&bytes, "pop rbp\n");
                fmt(&bytes, "ret\n");
            } break;
            case IT_StartFunction: {
                fmt(&bytes, "{} proc\n", instruction.string);
                fmt(&bytes, "push rbp\n");
                fmt(&bytes, "mov rbp, rsp\n");
            } break;
            case IT_EndFunction: {
                fmt(&bytes, "{} endp\n", instruction.string);
            } break;
            default:
                Unreachable("unsupported instruction type in asmgen");
        }
    }

    fmt(&bytes, "end\n");

    return to_slice(&bytes);
}

// @main
i32 main() {
    log_set_options(false, false);

    Arena arena = arena_create(GB(1));

    string source = read_entire_file("program/code.code");

    {
        Log("=== SOURCE ===");
        Log(source);
    }

    slice<Token> tokens = lex(&arena, source);

    {
        Arena temp_arena = arena_create(MB(5));

        string tokens_string = tokens_to_string(&temp_arena, tokens);
        Log("=== TOKENS ===");
        Log(tokens_string);

        arena_destroy(&temp_arena);
    }

    AST ast = parse(&arena, tokens);

    {
        Arena temp_arena = arena_create(MB(5));

        string ast_string = ast_to_string(&temp_arena, &ast);
        Log("=== AST ===");
        Log(ast_string);

        arena_destroy(&temp_arena);
    }

    IR ir = ir_gen(&arena, &ast);

    {
        Arena temp_arena = arena_create(MB(5));

        string ir_string = ir_to_string(&temp_arena, &ir);
        Log("=== IR ===");
        Log(ir_string);

        arena_destroy(&temp_arena);
    }

    string assembly = asmgen(&arena, &ir);

    {
        Log("=== ASSEMBLY ===");
        Log(assembly);
    }

    File output_file = new_file("program/output.asm");

    bool ok = create_file(&output_file);
    if (!ok) {
        Err("Failed to create output file");
        return 1;
    }

    ok = write_file(&output_file, assembly);
    if (!ok) {
        Err("Failed to write asm output file");
        return 1;
    }
}














