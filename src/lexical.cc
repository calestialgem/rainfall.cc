// SPDX-FileCopyrightText: 2022 Cem Geçgel <gecgelcem@outlook.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "linear.cc"
#include "tester.cc"

#include <string_view>
#include <utility>

/* Representation of code as a list of groups of characters. */
struct Lexical {
  /* Indivisible group of characters from the linear source. */
  struct Lexeme {
    /* Section of the source file the lexeme is in. */
    std::string_view section;

    /* Kind of the lexeme. */
    enum Kind {
      /* Mark ",". */
      COMMA,
      /* Mark ":". */
      COLON,
      /* Mark ";". */
      SEMICOLON,
      /* Mark "(". */
      OPENING_PARENTHESIS,
      /* Mark ")". */
      CLOSING_PARENTHESIS,
      /* Mark "{". */
      OPENING_BRACE,
      /* Mark "}". */
      CLOSING_BRACE,
      /* Mark "*=". */
      STAR_EQUAL,
      /* Mark "*". */
      STAR,
      /* Mark "/=". */
      SLASH_EQUAL,
      /* Mark "/". */
      SLASH,
      /* Mark "%=". */
      PERCENT_EQUAL,
      /* Mark "%". */
      PERCENT,
      /* Mark "+=". */
      PLUS_EQUAL,
      /* Mark "++". */
      PLUS_PLUS,
      /* Mark "+". */
      PLUS,
      /* Mark "->". */
      MINUS_RIGHT_ARROW,
      /* Mark "-=". */
      MINUS_EQUAL,
      /* Mark "--". */
      MINUS_MINUS,
      /* Mark "-". */
      MINUS,
      /* Mark "&=". */
      AMPERCENT_EQUAL,
      /* Mark "&&". */
      AMPERCENT_AMPERCENT,
      /* Mark "&". */
      AMPERCENT,
      /* Mark "|=". */
      PIPE_EQUAL,
      /* Mark "||". */
      PIPE_PIPE,
      /* Mark "|". */
      PIPE,
      /* Mark "^=". */
      CARET_EQUAL,
      /* Mark "^". */
      CARET,
      /* Mark "<<=". */
      LEFT_ARROW_LEFT_ARROW_EQUAL,
      /* Mark "<<". */
      LEFT_ARROW_LEFT_ARROW,
      /* Mark "<=". */
      LEFT_ARROW_EQUAL,
      /* Mark "<". */
      LEFT_ARROW,
      /* Mark ">>=". */
      RIGHT_ARROW_RIGHT_ARROW_EQUAL,
      /* Mark ">>". */
      RIGHT_ARROW_RIGHT_ARROW,
      /* Mark ">=". */
      RIGHT_ARROW_EQUAL,
      /* Mark ">". */
      RIGHT_ARROW,
      /* Mark "==". */
      EQUAL_EQUAL,
      /* Mark "=". */
      EQUAL,
      /* Mark "!=". */
      EXCLAMETION_EQUAL,
      /* Mark "!". */
      EXCLAMETION,
      /* Mark "~". */
      TILDE,
      /* Marks the end of the file. */
      END_OF_FILE,
      /* Keyword "let". */
      LET,
      /* Keyword "var". */
      VAR,
      /* Keyword "module". */
      MODULE,
      /* Keyword "intern". */
      INTERN,
      /* Keyword "extern". */
      EXTERN,
      /* Identifier. */
      IDENTIFIER,
      /* Decimal number literal. */
      DECIMAL,
      /* Characters that could not be lexed. */
      ERROR
    } kind;

    /* Lexeme kind enumaration where marks start. */
    static auto constexpr FIRST_MARK  = COMMA;
    /* Lexeme kind enumaration where marks end. */
    static auto constexpr AFTER_MARKS = END_OF_FILE;
    /* Mark lexeme kinds as strings. */
    static auto constexpr MARKS =
      std::array<std::string_view, AFTER_MARKS - FIRST_MARK>{
        ",",  ":",  ";",  "(",  ")",  "{",  "}",   "*=", "*",  "/=", "/",
        "%=", "%",  "+=", "++", "+",  "->", "-=",  "--", "-",  "&=", "&&",
        "&",  "|=", "||", "|",  "^=", "^",  "<<=", "<<", "<=", "<",  ">>=",
        ">>", ">=", ">",  "==", "=",  "!=", "!",   "~"};

    /* Lexeme kind enumaration where keywords start. */
    static auto constexpr FIRST_KEYWORD  = LET;
    /* Lexeme kind enumaration where keywords end. */
    static auto constexpr AFTER_KEYWORDS = IDENTIFIER;
    /* Keyword lexeme kinds as strings. */
    static auto constexpr KEYWORDS =
      std::array<std::string_view, AFTER_KEYWORDS - FIRST_KEYWORD>{
        "let", "var", "module", "intern", "extern"};
  };

  /* A Thrice source. */
  struct Source {
    /* Path to the source file. */
    std::filesystem::path path;
    /* Name of the source. */
    std::string           name;
    /* Contents of the source. */
    std::string           contents;
    /* Lex of the source. */
    std::vector<Lexeme>   lex;
  };

  /* A Thrice module. */
  struct Module {
    /* Name of the module. */
    std::string                             name;
    /* Contained sources that are directly under the module. This does not
     * include the sources that are contained by the submodules of the
     * module. */
    std::unordered_map<std::string, Source> sources;
    /* Contained modules that are directly under the module. This does not
     * include the modules that are contained by the submodules of the
     * module. */
    std::unordered_map<std::string, Module> submodules;
  };

  /* A Thrice package. */
  struct Package {
    /* Name of the package. */
    std::string                  name;
    /* Contents of the package as a module or as a single source package. */
    std::variant<Module, Source> contents;
  };

  /* A Rainfall workspace, which contains Thrice packages. */
  struct Workspace {
    /* Packages in the workspace. */
    std::unordered_map<std::string, Package> packages;
  };

  /* Register the tests in the lexical module to the given tester. */
  static void RegisterTests(Tester& tester) {}

  /* Lex the given linear workspace. */
  static Workspace LexWorkspace(Linear::Workspace lexed) {
    Workspace result;
    for (auto const& package : lexed.packages) {
      Package resultPackage = {.name = package.first};

      std::visit(
        Visitor{
          [](Linear::Module asModule) {
        for (auto const& subModule : range - expression) statements
          },
          [](Linear::Source asSource) {}},
        package.second.contents);

      result.packages[resultPackage.name] = resultPackage;
    }
    return result;
  }

private:

  /* Whether the given character is in the English alphabet. */
  static bool CompareToAlpha(char compared) {
    return (compared >= 'a' && compared <= 'z') ||
           (compared >= 'A' && compared <= 'Z');
  }

  /* Whether the given character is a decimal digit. */
  static bool CompareToDecimalDigit(char compared) {
    return compared >= '0' && compared <= '9';
  }

  /* Whether the given character is whitespace. */
  static bool CompareToWhitespace(char compared) {
    return compared == ' ' || compared == '\t' || compared == '\n';
  }

  /* Lexes a source file. */
  class Lexer {
    /* Result lex. */
    std::vector<Lexeme>                        result;
    /* Lexed source. */
    Linear::Source                             lexed;
    /* Position of the currently lexed character. */
    std::string::const_iterator                current;
    /* Start of the unknown portion.  */
    std::optional<std::string::const_iterator> unknownStart;

    /* Construct a lexer from the given linear source. */
    explicit Lexer(Linear::Source&& lexed)
      : lexed(std::move(lexed))
      , current{lexed.contents.cbegin()} {}

    /* Lex the given linear source. */
    static Source LexSource(Linear::Source&& lexed) {
      Lexer lexer{std::move(lexed)};
      lexer.Lex();
      return {
        std::move(lexer.lexed.path), std::move(lexer.lexed.name),
        std::move(lexer.lexed.contents), lexer.result};
    }

    void Lex() {
      while (CheckCharacterExistance()) {
        auto start = current;

        if (LexKeyword() || LexDecimal() || LexWord()) {
          if (LexSeparator()) continue;
          // Roll back the word or decimal that was lexed because it is not
          // separated from what comes after it.
          current = start;
          result.pop_back();
        }

        if (LexSeparator()) continue;

        // Unknown character! Mark all characters until a separator is found.
        unknownStart = start;
        do { current++; } while (!LexSeparator());
      }
    }

    /* Lex a separator. */
    bool LexSeparator() { return LexWhitespace() || LexComment() || LexMark(); }

    /* Lex a whitespace. */
    bool LexWhitespace() {
      if (!TakeAsMuchAsPossible(CompareToWhitespace)) return false;
      ReportUnknown();
      return true;
    }

    /* Lex a comment. */
    bool LexComment() {
      if (!TakeOne('/')) return false;
      if (!TakeInitialAndRest(
            [](char compared) { return compared == '/'; },
            [](char compared) { return compared != '\n'; })) {
        current--; // Roll back the first '/'.
        return false;
      }
      ReportUnknown();
      return true;
    }

    /* Lex a mark. */
    bool LexMark() {
      auto start = current;

      // Check whether there is a mark.
      auto mark =
        std::ranges::find_if(Lexeme::MARKS, [this](std::string_view mark) {
          return TakeFixed(mark);
        });
      if (mark == Lexeme::MARKS.cend()) return false;

      PushLexeme(
        CreateSection(start),
        (Lexeme::Kind)(Lexeme::FIRST_MARK + mark - Lexeme::MARKS.cbegin()));
      return true;
    }

    /* Lex a keyword. */
    bool LexKeyword() {
      auto start = current;

      // Check whether there is a keyword.
      auto mark =
        std::ranges::find_if(Lexeme::KEYWORDS, [this](std::string_view mark) {
          return TakeFixed(mark);
        });
      if (mark == Lexeme::KEYWORDS.cend()) return false;

      PushLexeme(
        CreateSection(start),
        (Lexeme::Kind)(
          Lexeme::FIRST_KEYWORD + mark - Lexeme::KEYWORDS.cbegin()));
      return true;
    }

    /* Lex a decimal. */
    bool LexDecimal() {
      auto start = current;

      // Lex the optional sign.
      TakeOne('+') || TakeOne('-');

      // Lex the whole part.
      if (!TakeInitialAndRest(CompareToDecimalDigit, [](char compared) {
            return CompareToDecimalDigit(compared) || compared == '_';
          })) {
        current = start;
        return false;
      }

      // Lex the fraction after dot.
      auto fractionStart = current;
      if (
        TakeOne('.') &&
        !TakeInitialAndRest(CompareToDecimalDigit, [](char compared) {
          return CompareToDecimalDigit(compared) || compared == '_';
        }))
        current = fractionStart;

      // Lex the exponent after 'e' or 'E'.
      auto exponentStart = current;
      if (TakeOne('e') || TakeOne('E')) {
        // Lex the optional sign.
        TakeOne('+') || TakeOne('-');

        if (!TakeInitialAndRest(CompareToDecimalDigit, [](char compared) {
              return CompareToDecimalDigit(compared) || compared == '_';
            }))
          current = exponentStart;
      }

      PushLexeme(CreateSection(start), Lexeme::DECIMAL);
      return true;
    }

    /* Lex a word. */
    bool LexWord() {
      auto start = current;

      if (!TakeInitialAndRest(
            [](char compared) {
        return CompareToAlpha(compared) || compared == '_';
            },
            [](char compared) {
        return CompareToAlpha(compared) || CompareToDecimalDigit(compared) ||
               compared == '_';
          })) {
        current = start;
        return false;
      }

      PushLexeme(CreateSection(start), Lexeme::IDENTIFIER);
      return true;
    }

    /* If there was an error, reports and clears the error start. */
    void ReportUnknown() {
      if (!unknownStart) return;
      auto unknown = CreateSection(*unknownStart);
      unknownStart.reset();

      // Remove the trailing whitespace.
      while (CompareToWhitespace(unknown.back())) unknown.remove_suffix(1);
      std::cout << std::format(
                     "{}: error: could not recognize {}: '{}'",
                     lexed.path.string(),
                     unknown.size() > 1 ? "these characters" : "this character",
                     unknown)
                << std::endl;
      result.push_back({.section = unknown, .kind = Lexeme::ERROR});
    }

    /* Create section starting from the given position to the current one. */
    std::string_view CreateSection(std::string::const_iterator start) {
      return {std::move(start), current};
    }

    /* Add a lexeme with the given value and type. Reports any errors. */
    void PushLexeme(std::string_view section, Lexeme::Kind kind) {
      ReportUnknown();
      result.push_back({.section = section, .kind = kind});
    }

    /* Whether the current character exists and it equals to the given one.
     * Consumes the character if true. */
    bool TakeOne(char taken) {
      // Compare the character.
      if (!CompareCurrent(taken)) return false;

      current++;
      return true;
    }

    /* Whether the next characters are the same as the given string. Consumes
     * the characters if true. */
    bool TakeFixed(std::string_view taken) {
      auto start = current;

      // If any character does not match retreat and return false.
      if (!std::ranges::all_of(
            taken, [this](char taken) { return TakeOne(taken); })) {
        current = start;
        return false;
      }

      return true;
    }

    /* Consume the characters if the first one fits the given leading predicate
     * and the remaining fit the given trailing check. */
    template<typename Leading, typename Trailing>
    bool TakeInitialAndRest(Leading&& leading, Trailing&& trailing) {
      // Check the first character.
      if (!CheckCurrent(leading)) return false;
      current++;

      // Then, consume all the characters until there is a mismatch.
      while (CheckCurrent(trailing)) current++;
      return true;
    }

    /* Consume all the characters that fit the given predicate. */
    template<typename All>
    bool TakeAsMuchAsPossible(All&& all) {
      return TakeInitialAndRest(all, all);
    }

    /* Whether the current character equals to the given one. */
    bool CompareCurrent(char compared) {
      return CheckCurrent(
        [compared](char checked) { return checked == compared; });
    }

    /* Whether the current character fits the given predicate. */
    template<typename Predicate>
    bool CheckCurrent(Predicate&& predicate) {
      return CheckCharacterExistance() && predicate(*current);
    }

    /* Whether there is a character to lex. */
    bool CheckCharacterExistance() const {
      return current < lexed.contents.cend();
    }
  };
};
