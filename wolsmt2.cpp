#include "wolsmt2.h"
#include "wolmgr.h"
#include <iostream>
#include <stdio.h>

namespace wolver{

WolSmt2Parser::WolSmt2Parser(WolMgr *mgr, FILE *file)
		: _mgr(mgr), _file(file)
{
	_saved = 0;
	_cc.assign(256, 0);
	wol_init_char_classes_smt();
	wol_insert_keywords_smt2();
	wol_insert_reserved_words_smt2();
	wol_insert_commands_smt2();
	wol_insert_core_symbols_smt2();
	wol_insert_bitvec_symbols_smt2();
	wol_insert_logics_smt2();		
}

WolSmt2Parser::~WolSmt2Parser()
{
	
}

void WolSmt2Parser::wol_init_char_classes_smt() 
{

	std::string wol_printable_ascii_chars_smt2 = "!\"#$%&'()*+,-./"
						      "0123456789"
						      ":;<=>?@"
						      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
						      "[\\]^_`"
						      "abcdefghijklmnopqrstuvwxyz"
						      "{|}~"
						      " \t\r\n";
	

	std::string wol_letters_smt2 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
					"abcdefghijklmnopqrstuvwxyz";

	std::string wol_decimal_digits_smt2("0123456789");
	std::string wol_hexadecimal_digits_smt2("0123456789abcdefABCDEF");
	std::string wol_extra_symbol_chars_smt2 ("+-/*=%?!.$_~&^<>@");
        std::string wol_extra_keyword_chars_smt2 ("+-/*=%?!.$_~&^<>@");

	for (char& p : wol_decimal_digits_smt2)
    		_cc[p] |= WOL_DECIMAL_DIGIT_CHAR_CLASS_SMT2;

  	for (char& p : wol_hexadecimal_digits_smt2)
    		_cc[p] |= WOL_HEXADECIMAL_DIGIT_CHAR_CLASS_SMT2;

	for (char& p : wol_printable_ascii_chars_smt2)
                _cc[p] |= WOL_STRING_CHAR_CLASS_SMT2;

	for (char& p : wol_letters_smt2)
                _cc[p] |= WOL_SYMBOL_CHAR_CLASS_SMT2;
	for (char& p : wol_decimal_digits_smt2)
                _cc[p] |= WOL_SYMBOL_CHAR_CLASS_SMT2;
	for (char& p : wol_extra_symbol_chars_smt2)
                _cc[p] |= WOL_SYMBOL_CHAR_CLASS_SMT2;

	for (char& p : wol_printable_ascii_chars_smt2)
		if (p != '\\' && p != '|')
                	_cc[p] |= WOL_QUOTED_SYMBOL_CHAR_CLASS_SMT2;

	for (char& p : wol_letters_smt2)
                _cc[p] |= WOL_KEYWORD_CHAR_CLASS_SMT2;
	for (char& p : wol_decimal_digits_smt2)
                _cc[p] |= WOL_KEYWORD_CHAR_CLASS_SMT2;
	for (char& p : wol_extra_keyword_chars_smt2)
                _cc[p] |= WOL_KEYWORD_CHAR_CLASS_SMT2;
}

void WolSmt2Parser::wol_insert_smt2(std::string str, WolSMT2Tag type) {
	
	WolSMT2Node *node = new WolSMT2Node(str, type);
        wol_insert_symbol_smt2(node);		
}

void WolSmt2Parser::wol_insert_symbol_smt2(WolSMT2Node *node) {

	_symbolTable[node->getName()] = node;
	//_symbolTable.insert(std::make_pair<std::string, WolSMT2Node*>(node->getName(), node));	
}


void WolSmt2Parser::wol_insert_keywords_smt2() {

  wol_insert_smt2(":all-statistics", WOL_ALL_STATISTICS_TAG_SMT2);
  wol_insert_smt2(":authors", WOL_AUTHORS_TAG_SMT2);
  wol_insert_smt2(":axioms", WOL_AXIOMS_TAG_SMT2);
  wol_insert_smt2(":chainable", WOL_CHAINABLE_TAG_SMT2);
  wol_insert_smt2(":definition", WOL_DEFINITION_TAG_SMT2);
  wol_insert_smt2(":diagnostic-output-channel", WOL_DIAG_OUTPUT_CHANNEL_TAG_SMT2);
  wol_insert_smt2(":error-behavior", WOL_ERROR_BEHAVIOR_TAG_SMT2);
  wol_insert_smt2(":expand-definitions", WOL_EXPAND_DEFINITIONS_TAG_SMT2);
  wol_insert_smt2(":extensions", WOL_EXTENSIONS_TAG_SMT2);
  wol_insert_smt2(":funs", WOL_FUNS_TAG_SMT2);
  wol_insert_smt2(":funs-description", WOL_FUNS_DESCRIPTION_TAG_SMT2);
  wol_insert_smt2(":interactive-mode", WOL_INTERACTIVE_MODE_TAG_SMT2);
  wol_insert_smt2(":language", WOL_LANGUAGE_TAG_SMT2);
  wol_insert_smt2(":left-assoc", WOL_LEFT_ASSOC_TAG_SMT2);
  wol_insert_smt2(":name", WOL_NAME_TAG_SMT2);
  wol_insert_smt2(":named", WOL_NAMED_TAG_SMT2);
  wol_insert_smt2(":notes", WOL_NOTES_TAG_SMT2);
  wol_insert_smt2(":print-success", WOL_PRINT_SUCCESS_TAG_SMT2);
  wol_insert_smt2(":produce-assignments", WOL_PRODUCE_ASSIGNMENTS_TAG_SMT2);
  wol_insert_smt2(":produce-models", WOL_PRODUCE_MODELS_TAG_SMT2);
  wol_insert_smt2(":produce-proofs", WOL_PRODUCE_PROOFS_TAG_SMT2);
  wol_insert_smt2(":produce-unsat-cores", WOL_PRODUCE_UNSAT_CORES_TAG_SMT2);
  wol_insert_smt2(":random-seed", WOL_RANDOM_SEED_TAG_SMT2);
  wol_insert_smt2(":reason-unknown", WOL_REASON_UNKNOWN_TAG_SMT2);
  wol_insert_smt2(":regular-output-channel", WOL_REGULAR_OUTPUT_CHANNEL_TAG_SMT2);
  wol_insert_smt2(":right-assoc", WOL_RIGHT_ASSOC_TAG_SMT2);
  wol_insert_smt2(":sorts", WOL_SORTS_TAG_SMT2);
  wol_insert_smt2(":sorts-description", WOL_SORTS_DESCRIPTION_TAG_SMT2);
  wol_insert_smt2(":status", WOL_STATUS_TAG_SMT2);
  wol_insert_smt2(":theories", WOL_THEORIES_TAG_SMT2);
  wol_insert_smt2(":values", WOL_VALUES_TAG_SMT2);
  wol_insert_smt2(":verbosity", WOL_VERBOSITY_TAG_SMT2);
  wol_insert_smt2(":version", WOL_VERSION_TAG_SMT2);
} 

void WolSmt2Parser::wol_insert_reserved_words_smt2() {
  wol_insert_smt2("!", WOL_BANG_TAG_SMT2);
  wol_insert_smt2("_", WOL_UNDERSCORE_TAG_SMT2);
  wol_insert_smt2("as", WOL_AS_TAG_SMT2);
  wol_insert_smt2("DECIMAL", WOL_DECIMAL_RESERVED_WORD_TAG_SMT2);
  wol_insert_smt2("exists", WOL_EXISTS_TAG_SMT2);
  wol_insert_smt2("forall", WOL_FORALL_TAG_SMT2);
  wol_insert_smt2("let", WOL_LET_TAG_SMT2);
  wol_insert_smt2("par", WOL_PAR_TAG_SMT2);
  wol_insert_smt2("STRING", WOL_STRING_RESERVED_WORD_TAG_SMT2);
}

void WolSmt2Parser::wol_insert_commands_smt2() {
 wol_insert_smt2("assert", WOL_ASSERT_TAG_SMT2);
  wol_insert_smt2("check-sat", WOL_CHECK_SAT_TAG_SMT2);
  wol_insert_smt2("declare-sort", WOL_DECLARE_SORT_TAG_SMT2);
  wol_insert_smt2("declare-fun", WOL_DECLARE_FUN_TAG_SMT2);
  wol_insert_smt2("define-sort", WOL_DEFINE_SORT_TAG_SMT2);
  wol_insert_smt2("define-fun", WOL_DEFINE_FUN_TAG_SMT2);
  wol_insert_smt2("exit", WOL_EXIT_TAG_SMT2);
  wol_insert_smt2("get-assertions", WOL_GET_ASSERTIONS_TAG_SMT2);
  wol_insert_smt2("get-assignment", WOL_GET_ASSIGNMENT_TAG_SMT2);
  wol_insert_smt2("get-info", WOL_GET_INFO_TAG_SMT2);
  wol_insert_smt2("get-option", WOL_GET_OPTION_TAG_SMT2);
  wol_insert_smt2("get-proof", WOL_GET_PROOF_TAG_SMT2);
  wol_insert_smt2("get-unsat-core", WOL_GET_UNSAT_CORE_TAG_SMT2);
  wol_insert_smt2("get-value", WOL_GET_VALUE_TAG_SMT2);
  wol_insert_smt2("pop", WOL_POP_TAG_SMT2);
  wol_insert_smt2("push", WOL_PUSH_TAG_SMT2);
  wol_insert_smt2("set-logic", WOL_SET_LOGIC_TAG_SMT2);
  wol_insert_smt2("set-info", WOL_SET_INFO_TAG_SMT2);
  wol_insert_smt2("set-option", WOL_SET_OPTION_TAG_SMT2);
}

void WolSmt2Parser::wol_insert_core_symbols_smt2() {
 wol_insert_smt2("Bool", WOL_BOOL_TAG_SMT2);
  wol_insert_smt2("true", WOL_TRUE_TAG_SMT2);
  wol_insert_smt2("false", WOL_FALSE_TAG_SMT2);
  wol_insert_smt2("not", WOL_NOT_TAG_SMT2);
  wol_insert_smt2("=>", WOL_IMPLIES_TAG_SMT2);
  wol_insert_smt2("and", WOL_AND_TAG_SMT2);
  wol_insert_smt2("or", WOL_OR_TAG_SMT2);
  wol_insert_smt2("xor", WOL_XOR_TAG_SMT2);
  wol_insert_smt2("=", WOL_EQUAL_TAG_SMT2);
  wol_insert_smt2("distinct", WOL_DISTINCT_TAG_SMT2);
  wol_insert_smt2("ite", WOL_ITE_TAG_SMT2);

}

void WolSmt2Parser::wol_insert_bitvec_symbols_smt2() {
wol_insert_smt2("BitVec", WOL_BITVEC_TAG_SMT2);
  wol_insert_smt2("concat", WOL_CONCAT_TAG_SMT2);
  wol_insert_smt2("extract", WOL_EXTRACT_TAG_SMT2);
  wol_insert_smt2("bvnot", WOL_BVNOT_TAG_SMT2);
  wol_insert_smt2("bvneg", WOL_BVNEG_TAG_SMT2);
  wol_insert_smt2("bvand", WOL_BVAND_TAG_SMT2);
  wol_insert_smt2("bvor", WOL_BVOR_TAG_SMT2);
  wol_insert_smt2("bvadd", WOL_BVADD_TAG_SMT2);
  wol_insert_smt2("bvmul", WOL_BVMUL_TAG_SMT2);
  wol_insert_smt2("bvudiv", WOL_BVUDIV_TAG_SMT2);
  wol_insert_smt2("bvurem", WOL_BVUREM_TAG_SMT2);
  wol_insert_smt2("bvshl", WOL_BVSHL_TAG_SMT2);
  wol_insert_smt2("bvlshr", WOL_BVLSHR_TAG_SMT2);
  wol_insert_smt2("bvult", WOL_BVULT_TAG_SMT2);
  wol_insert_smt2("bvnand", WOL_BVNAND_TAG_SMT2);
  wol_insert_smt2("bvnor", WOL_BVNOR_TAG_SMT2);
  wol_insert_smt2("bvxor", WOL_BVXOR_TAG_SMT2);
  wol_insert_smt2("bvxnor", WOL_BVXNOR_TAG_SMT2);
  wol_insert_smt2("bvcomp", WOL_BVCOMP_TAG_SMT2);
  wol_insert_smt2("bvsub", WOL_BVSUB_TAG_SMT2);
  wol_insert_smt2("bvsdiv", WOL_BVSDIV_TAG_SMT2);
  wol_insert_smt2("bvsrem", WOL_BVSREM_TAG_SMT2);
  wol_insert_smt2("bvsmod", WOL_BVSMOD_TAG_SMT2);
  wol_insert_smt2("bvashr", WOL_BVASHR_TAG_SMT2);
  wol_insert_smt2("repeat", WOL_REPEAT_TAG_SMT2);
  wol_insert_smt2("zero_extend", WOL_ZERO_EXTEND_TAG_SMT2);
  wol_insert_smt2("sign_extend", WOL_SIGN_EXTEND_TAG_SMT2);
  wol_insert_smt2("rotate_left", WOL_ROTATE_LEFT_TAG_SMT2);
  wol_insert_smt2("rotate_right", WOL_ROTATE_RIGHT_TAG_SMT2);
  wol_insert_smt2("bvule", WOL_BVULE_TAG_SMT2);
  wol_insert_smt2("bvugt", WOL_BVUGT_TAG_SMT2);
  wol_insert_smt2("bvuge", WOL_BVUGE_TAG_SMT2);
  wol_insert_smt2("bvslt", WOL_BVSLT_TAG_SMT2);
  wol_insert_smt2("bvsle", WOL_BVSLE_TAG_SMT2);
  wol_insert_smt2("bvsgt", WOL_BVSGT_TAG_SMT2);
  wol_insert_smt2("bvsge", WOL_BVSGE_TAG_SMT2);


}

void WolSmt2Parser::wol_insert_logics_smt2() {
 wol_insert_smt2("AUFLIA", WOL_AUFLIA_TAG_SMT2);
  wol_insert_smt2("AUFLIRA", WOL_AUFLIRA_TAG_SMT2);
  wol_insert_smt2("AUFNIRA", WOL_AUFNIRA_TAG_SMT2);
  wol_insert_smt2("LRA", WOL_LRA_TAG_SMT2);
  wol_insert_smt2("QF_ABV", WOL_QF_ABV_TAG_SMT2);
  wol_insert_smt2("QF_AUFBV", WOL_QF_AUFBV_TAG_SMT2);
  wol_insert_smt2("QF_AUFLIA", WOL_QF_AUFLIA_TAG_SMT2);
  wol_insert_smt2("QF_AX", WOL_QF_AX_TAG_SMT2);
  wol_insert_smt2("QF_BV", WOL_QF_BV_TAG_SMT2);
  wol_insert_smt2("QF_IDL", WOL_QF_IDL_TAG_SMT2);
  wol_insert_smt2("QF_LIA", WOL_QF_LIA_TAG_SMT2);
  wol_insert_smt2("QF_LRA", WOL_QF_LRA_TAG_SMT2);
  wol_insert_smt2("QF_NIA", WOL_QF_NIA_TAG_SMT2);
  wol_insert_smt2("QF_NRA", WOL_QF_NRA_TAG_SMT2);
  wol_insert_smt2("QF_RDL", WOL_QF_RDL_TAG_SMT2);
  wol_insert_smt2("QF_UF", WOL_QF_UF_TAG_SMT2);
  wol_insert_smt2("QF_UFBV", WOL_QF_UFBV_TAG_SMT2);
  wol_insert_smt2("QF_UFIDL", WOL_QF_UFIDL_TAG_SMT2);
  wol_insert_smt2("QF_UFLIA", WOL_QF_UFLIA_TAG_SMT2);
  wol_insert_smt2("QF_UFLRA", WOL_QF_UFLRA_TAG_SMT2);
  wol_insert_smt2("QF_UFNRA", WOL_QF_UFNRA_TAG_SMT2);
  wol_insert_smt2("UFLRA", WOL_UFLRA_TAG_SMT2);
  wol_insert_smt2("UFNIA", WOL_UFNIA_TAG_SMT2);
}

unsigned char WolSmt2Parser::wol_cc_smt2(int ch) {
	if (ch < 0 || ch >= 256) return 0;
	return _cc[ch];
}

int WolSmt2Parser::wol_nextch_smt2() {
	int res;
	if (_saved) {
		res = _savedch;
		_saved = 0;
	} 
	else
		res = fgetc(_file);

	// update line number and column number information

	return res;
}

void WolSmt2Parser::wol_savech_smt2(char ch) {
	_saved = 1;
	_savedch = ch;
	//update line number and colum number information
}

static
int wol_isspace_smt2(int ch) {
 return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n';
}


int WolSmt2Parser::wol_read_token_aux_smt2() {

	int ch;
	if (!_token.empty())
		std::cout << _token << std::endl;
	_token.clear();
	_lastNode = NULL;
	int _error = 0;
		
	while (1) {
		ch = wol_nextch_smt2();
		if (ch == EOF) return 0;
		else if (ch == ';') {
			while ((ch = wol_nextch_smt2 ()) != '\n') {
      				if (ch == EOF) {
					// error handling
					// unexpected enf-of-file in comment
        				return 0;
      				}
			}
			continue;
		}
		else if (wol_isspace_smt2(ch)) {
			continue;			
		}
		else {
			break;
		}
	}

	// assert statments on ch.	
	unsigned char cc = wol_cc_smt2(ch);
	if (ch == '(') {
		_token.push_back('(');
		_token.push_back('0');
    		return WOL_LPAR_TAG_SMT2;
  	}
  	if (ch == ')') {
		_token.push_back(')');
                _token.push_back('0');
    		return WOL_RPAR_TAG_SMT2;
  	}
	if (ch == '#') {
		_token.push_back('#');

		if ((ch = wol_nextch_smt2 ()) == EOF) {
			return 0;
			// error handling unexpected end-of-file after '#'
		}
		if (ch == 'b') {
			_token.push_back('b');
			
			if((ch = wol_nextch_smt2 ()) == EOF) 
				return 0;  //!wol_perr_smt2 (parser, "unexpected end-of-file after '#b'");
			
			if (ch != '0' && ch != '1')
				return _error; //!wol_perr_smt2 (parser, "expected '0' or '1' after '#b'");
			_token.push_back(ch);

			for (;;) {
				ch = wol_nextch_smt2 ();
				if (ch != '0' && ch != '1') break;
				_token.push_back(ch);
			}
			wol_savech_smt2 (ch);
			_token.push_back(0);
			return WOL_BINARY_CONSTANT_TAG_SMT2;
		} else if (ch == 'x') {
			_token.push_back('x');

			if ((ch = wol_nextch_smt2 ()) == EOF)
				return 0; //!wol_perr_smt2 (parser, "unexpected end-of-file after '#x'");
			
			if (!(wol_cc_smt2 (ch) & WOL_HEXADECIMAL_DIGIT_CHAR_CLASS_SMT2))
				return _error; //!wol_perr_smt2 (parser, "expected hexa-decimal digit after '#x'");
			_token.push_back(ch);
			
			for (;;) {
				ch = wol_nextch_smt2 ();
				if (!(wol_cc_smt2 (ch) & WOL_HEXADECIMAL_DIGIT_CHAR_CLASS_SMT2)) break;
				_token.push_back(ch);
			}
			wol_savech_smt2 (ch);
			_token.push_back(0);
			return WOL_HEXADECIMAL_CONSTANT_TAG_SMT2;
		} else 
			return _error; // !wol_perr_smt2 (parser, "expected 'x' or 'b' after '#'");
	}	 
	else if (ch == '"') {
		_token.push_back('"');

		for (;;) {

			if ((ch = wol_nextch_smt2 ()) == EOF)
				return _error; //!btor_cerr_smt2 (parser, "unexpected", ch, "in string");

			if (ch == '"') {
				_token.push_back('"');
				_token.push_back(0);
				return WOL_STRING_CONSTANT_TAG_SMT2;
			}

			if (ch == '\\') {
				if ((ch = wol_nextch_smt2()) != '"' && ch != '\\')
					return _error; //!btor_cerr_smt2 (parser, "unexpected", ch,"after backslash '\\\\' in string");
			} else if (!(wol_cc_smt2 (ch) & WOL_STRING_CHAR_CLASS_SMT2)) {
				return _error; //!btor_cerr_smt2 (parser, "invalid", ch, "in string");
			}
			_token.push_back(ch);
		}
	} 
	else if (ch == '|') {
		_token.push_back('|');

		for (;;) {

			if ((ch = wol_nextch_smt2 ()) == EOF)
				return _error; //!btor_cerr_smt2 (parser, "unexpected", ch, "in quoted symbol");

			if (ch == '|') {
				_token.push_back('|');
				_token.push_back(0);
				std::unordered_map<std::string, WolSMT2Node*>::iterator it = _symbolTable.find(_token);
				WolSMT2Node *node;

				if (it == _symbolTable.end()) {
					node = new WolSMT2Node(_token, WOL_SYMBOL_TAG_SMT2);
					wol_insert_symbol_smt2 (node);
				}
				else
				     node = it->second;
				_lastNode = node;
				return WOL_SYMBOL_TAG_SMT2;
			}

			if (!(wol_cc_smt2 (ch) & WOL_QUOTED_SYMBOL_CHAR_CLASS_SMT2))
				return _error; //!btor_cerr_smt2 (parser, "invalid", ch, "in quoted symbol");
			_token.push_back(ch);
		}
	} 
	else if (ch == ':') {
		_token.push_back(':');

    		if ((ch = wol_nextch_smt2 ()) == EOF)
      		return _error; //!btor_perr_smt2 (parser, "unexpected end-of-file after ':'");

    		if (!(wol_cc_smt2 (ch) & WOL_KEYWORD_CHAR_CLASS_SMT2))
      		return _error; //!btor_cerr_smt2 (parser, "unexpected", ch, "after ':'");
    		_token.push_back(ch);

    		while ((wol_cc_smt2 (ch = wol_nextch_smt2 ()) & WOL_KEYWORD_CHAR_CLASS_SMT2)) {
      			//assert (ch != EOF);
      			_token.push_back(ch);
    		}
    		wol_savech_smt2 (ch);
    		_token.push_back(0);
		std::unordered_map<std::string, WolSMT2Node*>::iterator it = _symbolTable.find(_token);
		WolSMT2Node *node;

		if (it == _symbolTable.end()) {
			node = new WolSMT2Node(_token, WOL_ATTRIBUTE_TAG_SMT2);
			wol_insert_symbol_smt2 (node);
		}
		else
			node = it->second;
		_lastNode = node;

    		return WOL_ATTRIBUTE_TAG_SMT2;
  	} else if (ch == '0') {
		_token.push_back('0');
		ch = wol_nextch_smt2();
	
		if (ch == '.') {
			_token.push_back('.');
			
			if ((ch = wol_nextch_smt2()) == EOF)
			return _error; //!btor_perr_smt2 (parser, "unexpected end-of-file after '0.'");

			if (!(wol_cc_smt2 (ch) & WOL_DECIMAL_DIGIT_CHAR_CLASS_SMT2))
				return _error; //!btor_perr_smt2 (parser, "expected decimal digit after '0.'");
			_token.push_back(ch);

			for (;;) {
				ch = wol_nextch_smt2();
				if (!(wol_cc_smt2(ch) & WOL_DECIMAL_DIGIT_CHAR_CLASS_SMT2)) break;
				_token.push_back(ch);
			}	
		}
		wol_savech_smt2(ch);
		_token.push_back(0);
		return WOL_DECIMAL_CONSTANT_TAG_SMT2;
	} else if (cc & WOL_DECIMAL_DIGIT_CHAR_CLASS_SMT2) {
		_token.push_back(ch);

		for (;;) {
			ch = wol_nextch_smt2 ();
			if (!(wol_cc_smt2 (ch) & WOL_DECIMAL_DIGIT_CHAR_CLASS_SMT2))
				break;
			_token.push_back(ch);
		}

		if (ch == '.') {
			_token.push_back('.');

			if ((ch = wol_nextch_smt2 ()) == EOF) {
				_token.push_back(0);
				return _error; // !btor_perr_smt2 (parser, "unexpected end-of-file after '%s'", parser->token.start);
			}

			if (!(wol_cc_smt2 (ch) & WOL_DECIMAL_DIGIT_CHAR_CLASS_SMT2)) {
				_token.push_back(0);
				return _error; //!btor_perr_smt2 (parser, "expected decimal digit after '%s'", parser->token.start);
			}
			_token.push_back(ch);

			for (;;) {
				ch = wol_nextch_smt2 ();
				if (!(wol_cc_smt2 (ch) & WOL_DECIMAL_DIGIT_CHAR_CLASS_SMT2)) break;
				_token.push_back(ch);
			}
		}
		wol_savech_smt2 (ch);
		_token.push_back(0);
		return WOL_DECIMAL_CONSTANT_TAG_SMT2;
	
	} else if (cc & WOL_SYMBOL_CHAR_CLASS_SMT2) {
		_token.push_back(ch);
		for (;;) {
			ch = wol_nextch_smt2 ();
			if (!(wol_cc_smt2 (ch) & WOL_SYMBOL_CHAR_CLASS_SMT2)) break;
			_token.push_back(ch);
		}
		wol_savech_smt2 (ch);
		_token.push_back(0);
		if (_token.compare("_") == 0) return WOL_UNDERSCORE_TAG_SMT2;
		
		std::unordered_map<std::string, WolSMT2Node*>::iterator it = _symbolTable.find(_token);
		WolSMT2Node *node;

		if (it == _symbolTable.end()) {
			node = new WolSMT2Node(_token, WOL_SYMBOL_TAG_SMT2);
			wol_insert_symbol_smt2 (node);
		}
		else
			node = it->second;
		_lastNode = node;

		return WOL_SYMBOL_TAG_SMT2;
	} else return _error; // !btor_cerr_smt2 (parser, "illegal", ch, 0);

	return _error;
}

int WolSmt2Parser::wol_read_token_smt2() {

	_lastcoo = _coo;
	int res = wol_read_token_aux_smt2();
	// printing utils

	return res;
}


int WolSmt2Parser::wol_read_command_smt2() {

	std::pair<int, int> coo(0,0);
	int type;

	type = wol_read_token_smt2();

	return type;
}

void WolSmt2Parser::parse() {
	
	while(wol_read_command_smt2());
		
}


}
