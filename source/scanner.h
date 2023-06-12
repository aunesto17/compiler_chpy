#ifndef scanner_h
#define scanner_h

#include <string>
#include <cctype>
#include "tokens.h"

class Token {
public:
    Token(TokenType type, std::string lexeme, std::size_t line)
        : type_(type),
          lexeme_(std::move(lexeme)),
          line_(line) {}

    TokenType getType() const { return type_; }
    std::string getLexeme() const { return lexeme_; }
    std::size_t getLine() const { return line_; }

private:
    TokenType type_;
    std::string lexeme_;
    std::size_t line_;
};

class Scanner {
public:
    Scanner(const std::string& sourceCode)
        : sourceCode_(sourceCode),
          currentPosition_(0),
          line_(1) {}

    Token getNextToken() {
        skipWhitespaceAndComments();

        if (isAtEnd()) {
            return Token(END_OF_FILE, "", line_);
        }

        char c = getChar();

        switch (c) {
            // Handle single and double character tokens
            case '+':
                return Token(PLUS, "+", line_);
            case '-':
                char nextChar = peekChar();
                if (match(nextChar, '>')) {
                    return Token(ARROW, "->", line_);
                } else {
                    return Token(MINUS, "-", line_);
                }
            case '*':
                return Token(ASTERISK, "*", line_);
            case '/':
                return Token(DOUBLE_SLASH, "//", line_);
            case '%':
                return Token(MODULO, "%", line_);
            case '<':
                char nextChar = peekChar();
                if (match(nextChar, '=')) {
                    return Token(LESS_EQUAL, "<=", line_);
                } else {
                    return Token(LESS, "<", line_);
                }
            case '>':    
                char nextChar = peekChar();
                if (match(nextChar, '=')) {
                    return Token(GREATER_EQUAL, ">=", line_);
                } else {
                    return Token(GREATER, ">", line_);
                }
            case '=':
                char nextChar = peekChar();
                if (match(nextChar, '=')) {
                    return Token(EQUALS, "==", line_);
                } else {
                    return Token(ASSIGN, "=", line_);
                }
            case '!':
                char nextChar = peekChar();
                if (match(nextChar, '=')) {
                    return Token(NOT_EQUAL, "!=", line_);
                } else {
                    // handle error and skip to next char
                }
            case '(':
                return Token(LEFT_PAREN, "(", line_);
            case ')':
                return Token(RIGHT_PAREN, ")", line_);
            case '[':
                return Token(LEFT_BRACKET, "[", line_);
            case ']':
                return Token(RIGHT_BRACKET, "]", line_);
            case ',':
                return Token(COMMA, ",", line_);
            case ':':
                return Token(COLON, ":", line_);
            case '.':
                return Token(DOT, ".", line_);

            default:
                if (isAlpha(c)) {
                    return scanIdentifier();
                }
                // ... (handle other cases)
        }
    }

private:
    bool isAtEnd() const {
        return currentPosition_ >= sourceCode_.length();
    }

    char getChar() {
        return sourceCode_[currentPosition_++];
    }

    char peekChar() const {
        if (isAtEnd()) {
            return '\0';
        }
        return sourceCode_[currentPosition_+1];
    }

    bool match(char peek, char expected) {
        if (isAtEnd() || peek != expected) {
            return false;
        }
        currentPosition_++;
        return true;
    }

    void skipWhitespaceAndComments() {
        while (!isAtEnd()) {
            char c = sourceCode_[currentPosition_];

            if (c == ' ' || c == '\t' || c == '\r') {
                // Skip whitespace
                currentPosition_++;
            } else if (c == '\n') {
                // Newline encountered, increment line number
                currentPosition_++;
                line_++;
            } else if (c == '#') {
                // Skip comments until the end of the line
                while (!isAtEnd() && sourceCode_[currentPosition_] != '\n') {
                    currentPosition_++;
                }
            } else {
                // No more whitespace or comments to skip
                break;
            }
        }
    }

    Token scanIdentifier() {
        std::string lexeme;
        lexeme += sourceCode_[currentPosition_ - 1];

        while (!isAtEnd() && (isAlphaNumeric(sourceCode_[currentPosition_]) || sourceCode_[currentPosition_] == '_')) {
            lexeme += getChar();
        }

        // Check if the identifier is a keyword
        TokenType type = getKeywordType(lexeme);

        return Token(type, lexeme, line_);
    }

    TokenType getKeywordType(const std::string& lexeme) {
        if (lexeme == "false") return KW_FALSE;
        else if (lexeme == "none") return KW_NONE;
        else if (lexeme == "true") return KW_TRUE;
        else if (lexeme == "and") return KW_AND;
        else if (lexeme == "as") return KW_AS;
        else if (lexeme == "assert") return KW_ASSERT;
        else if (lexeme == "async") return KW_ASYNC;
        else if (lexeme == "await") return KW_AWAIT;
        else if (lexeme == "break") return KW_BREAK;
        else if (lexeme == "class") return KW_CLASS;
        else if (lexeme == "continue") return KW_CONTINUE;
        else if (lexeme == "def") return KW_DEF;
        else if (lexeme == "del") return KW_DEL;
        else if (lexeme == "elif") return KW_ELIF;
        else if (lexeme == "else") return KW_ELSE;
        else if (lexeme == "except") return KW_EXCEPT;
        else if (lexeme == "finally") return KW_FINALLY;
        else if (lexeme == "for") return KW_FOR;
        else if (lexeme == "from") return KW_FROM;
        else if (lexeme == "global") return KW_GLOBAL;
        else if (lexeme == "if") return KW_IF;
        else if (lexeme == "import") return KW_IMPORT;
        else if (lexeme == "in") return KW_IN;
        else if (lexeme == "is") return KW_IS;
        else if (lexeme == "lambda") return KW_LAMBDA;
        else if (lexeme == "nonlocal") return KW_NONLOCAL;
        else if (lexeme == "not") return KW_NOT;
        else if (lexeme == "or") return KW_OR;
        else if (lexeme == "pass") return KW_PASS;
        else if (lexeme == "raise") return KW_RAISE;
        else if (lexeme == "return") return KW_RETURN;
        else if (lexeme == "try") return KW_TRY;
        else if (lexeme == "while") return KW_WHILE;
        else if (lexeme == "with") return KW_WITH;
        else if (lexeme == "yield") return KW_YIELD;
        
        // else if (lexeme == "int") return KW_INT;
        // else if (lexeme == "float") return KW_FLOAT;
        // else if (lexeme == "bool") return KW_BOOL;
        // else if (lexeme == "str") return KW_STR;
        // else if (lexeme == "list") return KW_LIST;
        // else if (lexeme == "tuple") return KW_TUPLE;
        // else if (lexeme == "set") return KW_SET;
        // else if (lexeme == "dict") return KW_DICT;
        // else if (lexeme == "len") return
        // ... (handle other keywords)

        // If it's not a keyword, treat it as a regular identifier
        return IDENTIFIER;
    }

    bool isAlpha(char c) const {
        return std::isalpha(static_cast<unsigned char>(c)) || c == '_';
    }

    bool isAlphaNumeric(char c) const {
        return std::isalnum(static_cast<unsigned char>(c)) || c == '_';
    }

private:
    const std::string& sourceCode_;
    std::size_t currentPosition_;
    std::size_t line_;
};


#endif /* scanner_h */
