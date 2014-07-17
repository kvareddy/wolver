#include "wolsmt2.h"
#include "wolmgr.h"
#include "wolexpfactory.h"
#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <string>
#include <memory>

namespace wolver{

using namespace std;

WolSmt2Parser::WolSmt2Parser(FILE *file)
		: _file(file)
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
	_token.clear();
	_lastNode = NULL;
		
	while (1) {
		ch = wol_nextch_smt2();
		if (ch == EOF) return 0;
		else if (ch == ';') {
			while ((ch = wol_nextch_smt2 ()) != '\n') {
      				if (ch == EOF) {
					      cout << "unexpected enf-of-file in comment" << endl;
        				   return EOF;
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
         cout << "error handling unexpected end-of-file ater #" << endl;
			return EOF;
		}
		if (ch == 'b') {
			_token.push_back('b');
			
			if((ch = wol_nextch_smt2 ()) == EOF) {
            cout << "unexpected end-of-file after '#b'" << endl; 
				return EOF;
         }
			
			if (ch != '0' && ch != '1') {
            cout << "expected '0' or '1' after '#b'" << endl;
				return WOL_INVALID_TAG_SMT2;
         }
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

			if ((ch = wol_nextch_smt2 ()) == EOF) {
		      cout << "unexpected end-of-file after '#x'" << endl;	
            return 0;
         }
			if (!(wol_cc_smt2 (ch) & WOL_HEXADECIMAL_DIGIT_CHAR_CLASS_SMT2)) {
			   cout << "expected hexa-decimal digit after '#x'" << endl;
            return WOL_INVALID_TAG_SMT2;
         }
			_token.push_back(ch);
			
			for (;;) {
				ch = wol_nextch_smt2 ();
				if (!(wol_cc_smt2 (ch) & WOL_HEXADECIMAL_DIGIT_CHAR_CLASS_SMT2)) break;
				_token.push_back(ch);
			}
			wol_savech_smt2 (ch);
			_token.push_back(0);
			return WOL_HEXADECIMAL_CONSTANT_TAG_SMT2;
		} else {
         cout << "expected x or b after #" << endl;
			return WOL_INVALID_TAG_SMT2;
      }
	}	 
	else if (ch == '"') {
		_token.push_back('"');

		for (;;) {

			if ((ch = wol_nextch_smt2 ()) == EOF) {
			   cout << "unexpected " << ch << " in string" << endl;
            return EOF;
         }
			if (ch == '"') {
				_token.push_back('"');
				_token.push_back(0);
				return WOL_STRING_CONSTANT_TAG_SMT2;
			}

			if (ch == '\\') {
				if ((ch = wol_nextch_smt2()) != '"' && ch != '\\') {
               cout << "unexpected " << ch << " after backslash '\\\\' in string" << endl;
					return WOL_INVALID_TAG_SMT2;
			   }
         } else if (!(wol_cc_smt2 (ch) & WOL_STRING_CHAR_CLASS_SMT2)) {
            cout << "invalid " << ch << " in string" << endl;
				return WOL_INVALID_TAG_SMT2;
			}
			_token.push_back(ch);
		}
	} 
	else if (ch == '|') {
		_token.push_back('|');

		for (;;) {

			if ((ch = wol_nextch_smt2 ()) == EOF) {
            cout << "unexpected " << ch << " in quoted symbol" << endl;
				return EOF;
         }

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

			if (!(wol_cc_smt2 (ch) & WOL_QUOTED_SYMBOL_CHAR_CLASS_SMT2)) {
            cout << "invalid " << ch << " in quoted symbol" << endl;   
				return WOL_INVALID_TAG_SMT2;
         }
			_token.push_back(ch);
		}
	} 
	else if (ch == ':') {
		_token.push_back(':');

      if ((ch = wol_nextch_smt2 ()) == EOF) {
         cout << "unexpected end-of-file after ':'" << endl;
      	return EOF;
      }

    	if (!(wol_cc_smt2 (ch) & WOL_KEYWORD_CHAR_CLASS_SMT2)) {
         cout << "unexpected " << ch << "after ':'" << endl;
      	return WOL_INVALID_TAG_SMT2; 
      }
    	_token.push_back(ch);

    	while ((wol_cc_smt2 (ch = wol_nextch_smt2 ()) & WOL_KEYWORD_CHAR_CLASS_SMT2)) {
      	assert (ch != EOF);
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

    	return node->getType();
  	} else if (ch == '0') {
		_token.push_back('0');
		ch = wol_nextch_smt2();
	
		if (ch == '.') {
			_token.push_back('.');
			
			if ((ch = wol_nextch_smt2()) == EOF) {
            cout << "unexpected end-of-file after '0.'" << endl;
			   return EOF;
         }
			if (!(wol_cc_smt2 (ch) & WOL_DECIMAL_DIGIT_CHAR_CLASS_SMT2)) {
            cout << "expected decimal digit after '0.'" << endl;
				return WOL_INVALID_TAG_SMT2;
         }
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
            cout << "unexpected end-of-file after " << _token << endl;
				return EOF; 
			}

			if (!(wol_cc_smt2 (ch) & WOL_DECIMAL_DIGIT_CHAR_CLASS_SMT2)) {
				_token.push_back(0);
            cout << "expected decimal digit after " << _token << endl;
				return WOL_INVALID_TAG_SMT2;
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
		if (_token.compare("_") == 0) return WOL_UNDERSCORE_TAG_SMT2;
	
		std::unordered_map<std::string, WolSMT2Node*>::const_iterator it = _symbolTable.find(_token);
		WolSMT2Node *node;
      
		if (it != _symbolTable.end()) {
         node = it->second;
      }
      else {
			node = new WolSMT2Node(_token, WOL_SYMBOL_TAG_SMT2);
			wol_insert_symbol_smt2 (node);
		}
		_lastNode = node;

		return node->getType();
	} else {
      cout << "illegal " << ch << endl;
      return WOL_INVALID_TAG_SMT2;
   }

	return WOL_INVALID_TAG_SMT2;
}


int WolSmt2Parser::wol_read_symbol(const char * str) {

   int type = wol_read_token_smt2 ();
   if (type == WOL_INVALID_TAG_SMT2) return 0;
   if (type == EOF) {
      cout << "expected symbol " << str << " but reaced end-of-file" << endl;
      return 0;
   }
   if (type != WOL_SYMBOL_TAG_SMT2){
      cout << "expected symbol " << str << " at" << _token << endl;
      return 0;
   }
   assert (_lastNode->getType() == WOL_SYMBOL_TAG_SMT2);
   return 1;

}

int WolSmt2Parser::wol_read_rpar_smt2 (const char * msg) {
   int type = wol_read_token_smt2 ();
   if (type == EOF) {
      cout << "expected ')' " << msg << " at end-of-file" << endl;
      return 0;
   }
   if (type == WOL_INVALID_TAG_SMT2) { return 0; }
   if (type != WOL_RPAR_TAG_SMT2) {
      cout << "expected ')' " << msg << " at " << _token << endl;
      return 0;
   }
   return 1;
}

int WolSmt2Parser::wol_read_lpar_smt2 (const char * msg) {
   int type = wol_read_token_smt2 ();
   if (type == EOF) {
      cout << "expected '(' " << msg << " at end-of-file" << endl;
      return 0;
   }
   if (type == WOL_INVALID_TAG_SMT2) { return 0; }
   if (type != WOL_LPAR_TAG_SMT2) {
      cout << "expected '(' " << msg << " at " << _token << endl;
      return 0;
   }
   return 1;
}

int WolSmt2Parser::wol_parse_bitvec_sort_smt2(int skiplu, int &width) {
   
   int type;
   
   if (!skiplu) {
      if (!wol_read_lpar_smt2 ("")) return 0;
      type  = wol_read_token_smt2 ();
      if (type == WOL_INVALID_TAG_SMT2) return 0;
      if (type == EOF) {
         cout << "expected '_' but reached end-of-file" << endl;
         return 0;
      }
      if (type != WOL_UNDERSCORE_TAG_SMT2) {
         cout << "expected '_' at " << _token << endl;
         return 0;
      }
   }

   type  = wol_read_token_smt2 ();
   if (type == WOL_INVALID_TAG_SMT2) return 0;
   if (type == EOF) {
      cout << "expected 'BitVec' but reached end-of-file" << endl;
      return 0;
   } 
   if (type != WOL_BITVEC_TAG_SMT2) {
      cout << "expected 'BitVec' at " << _token  << endl;
      return 0;
   }   

   type  = wol_read_token_smt2 ();
   if (type == WOL_INVALID_TAG_SMT2) return 0;
   if (type == EOF){
      cout << "expected bit-width but reached end-of-file" << endl;
      return 0;
   } 
   if (type != WOL_DECIMAL_CONSTANT_TAG_SMT2) {
      cout << "expected bit-width at " << _token << endl;
      return 0;
   }
   assert (_token[0] != '-');
   if (_token[0] == '0') {
      assert (!_token[1]);
      cout << "invalid zero bit-width" << endl;  
      return 0;
   }
   if (_token.find('.') != string::npos) {
      cout << "invalid floating point bit-width " << _token << endl;
      return 0;
   }
   
   width = atoi(_token.c_str());
   return wol_read_rpar_smt2 (" to close bit-vector sort");
}

int WolSmt2Parser::wol_declare_fun_smt2 () {

   int width = 0;
   if (!wol_read_symbol("after 'declare-fun'")) return 0;
   
   WolSMT2Node *varNode = _lastNode;   

   if (!wol_read_lpar_smt2 (" after function name")) return 0;
   if (!wol_read_rpar_smt2 (" (only constants and no uninterpreted functions are supported) after '('"))
    return 0;
   int type = wol_read_token_smt2 ();
   if (type == WOL_INVALID_TAG_SMT2) return 0;
   if (type == EOF) {
      cout << "reached end-of-file but expected '(' or 'Bool'" << endl;
      return 0;
   }
   if (type == WOL_BOOL_TAG_SMT2) { 
      width = 1;
      varNode->setWolNode(WolExpFactory::getInstance().makeVarExpr(width, varNode->getName()));
   }
   else {
      if (type != WOL_LPAR_TAG_SMT2) {
         cout << "expected '(' or 'Bool' at " << _token << endl;
         return 0;
      }
      type = wol_read_token_smt2 ();
      if (type == WOL_INVALID_TAG_SMT2) return 0;
      if (type == EOF) {
         cout << "reached end-of-file expecting '_'" << endl;
         return 0;
      }
      if (type == WOL_UNDERSCORE_TAG_SMT2) {
         if (!wol_parse_bitvec_sort_smt2 (1, width)) return 0;
         varNode->setWolNode(WolExpFactory::getInstance().makeVarExpr(width, varNode->getName()));
      }
      else {
         cout << "expected '_' at " << _token << endl;
         return 0;
      }
  }
  _inputs.push_back(varNode->getWolNode()); 
  return wol_read_rpar_smt2 (" to close declaration");

}

int WolSmt2Parser::wol_parse_bitvec_term_smt2(int type, WolNodeSptr& exp) {
         
   WolNodeSptr (WolExpFactory:: *unary_func) (WolNodeSptr);
   WolNodeSptr (WolExpFactory:: *binary_func) (WolNodeSptr, WolNodeSptr); 

   if (type == WOL_BITVEC_TAG_SMT2) {
      cout << "unexpected symbol bitvec" << endl;
      return 0;
   }     
   else if (type == WOL_CONCAT_TAG_SMT2) {
      binary_func = &WolExpFactory::makeConcatExpr;
   }            
   else if (type == WOL_EXTRACT_TAG_SMT2) {
      cout << "unexpected symbol extract" << endl;
      return 0;
   }
   else if (type == WOL_BVNOT_TAG_SMT2) {
      unary_func = &WolExpFactory::makeNotExpr;
   }
   else if (type == WOL_BVNEG_TAG_SMT2) {
      unary_func = &WolExpFactory::makeNegExpr;
   }
   else if (type == WOL_BVAND_TAG_SMT2) {
      binary_func = &WolExpFactory::makeAndExpr;
   }
   else if (type == WOL_BVOR_TAG_SMT2) {
      binary_func = &WolExpFactory::makeOrExpr;
   }
   else if (type == WOL_BVADD_TAG_SMT2) {
      binary_func = &WolExpFactory::makeAddExpr;
   }
   else if (type == WOL_BVMUL_TAG_SMT2) {
      binary_func = &WolExpFactory::makeMulExpr;
   }
   else if (type == WOL_BVUDIV_TAG_SMT2) {
      binary_func = &WolExpFactory::makeUdivExpr;
   }
   else if (type == WOL_BVUREM_TAG_SMT2) {
      binary_func = &WolExpFactory::makeUremExpr;
   }
   else if (type == WOL_BVSHL_TAG_SMT2) {
      binary_func = &WolExpFactory::makeSllExpr;
   }
   else if (type == WOL_BVLSHR_TAG_SMT2) {
      binary_func = &WolExpFactory::makeSrlExpr;
   }
   else if (type == WOL_BVULT_TAG_SMT2) {
      binary_func = &WolExpFactory::makeUltExpr;
   }
   else if (type == WOL_BVNAND_TAG_SMT2) {
      binary_func = &WolExpFactory::makeNandExpr;
   }
   else if (type == WOL_BVNOR_TAG_SMT2) {
      binary_func = &WolExpFactory::makeNorExpr;
   }
   else if (type == WOL_BVXOR_TAG_SMT2) {
      binary_func = &WolExpFactory::makeXorExpr;
   }
   else if (type == WOL_BVXNOR_TAG_SMT2) {
      binary_func = &WolExpFactory::makeXnorExpr;
   }
   else if (type == WOL_BVCOMP_TAG_SMT2) {
      binary_func = &WolExpFactory::makeEqExpr;
   }
   else if (type == WOL_BVSUB_TAG_SMT2) {
      binary_func = &WolExpFactory::makeSubExpr;
   }
   else if (type == WOL_BVSDIV_TAG_SMT2) {
      binary_func = &WolExpFactory::makeSdivExpr;
   }
   else if (type == WOL_BVSREM_TAG_SMT2) {
      binary_func = &WolExpFactory::makeSremExpr;
   }
   else if (type == WOL_BVSMOD_TAG_SMT2) {
      binary_func = &WolExpFactory::makeSmodExpr;
   }
   else if (type == WOL_BVASHR_TAG_SMT2) {
      binary_func = &WolExpFactory::makeSraExpr;
   }
   else if (type == WOL_BVULE_TAG_SMT2) {
      binary_func = &WolExpFactory::makeUlteExpr;
   }
   else if (type == WOL_BVUGT_TAG_SMT2) {
      binary_func = &WolExpFactory::makeUgtExpr;   
   }
   else if (type == WOL_BVUGE_TAG_SMT2) {
      binary_func = &WolExpFactory::makeUgteExpr;
   }
   else if (type == WOL_BVSLT_TAG_SMT2) {
      binary_func = &WolExpFactory::makeSltExpr;
   }
   else if (type == WOL_BVSLE_TAG_SMT2) {
      binary_func = &WolExpFactory::makeSlteExpr;
   }
   else if (type == WOL_BVSGT_TAG_SMT2) {
      binary_func = &WolExpFactory::makeSgtExpr;
   }
   else if (type == WOL_BVSGE_TAG_SMT2) {
      binary_func = &WolExpFactory::makeSgteExpr;
   }
   else if (type == WOL_REPEAT_TAG_SMT2) {
      cout << "unexpected token repeat" << endl;
      return 0;
   }
   else if (type == WOL_ZERO_EXTEND_TAG_SMT2) {
      cout << "unexpected token zeo_extend" << endl;
      return 0;
   }
   else if (type == WOL_SIGN_EXTEND_TAG_SMT2) {
      cout << "unexpected sign extend operator" << endl;
      return 0;
   }
   else if (type == WOL_ROTATE_LEFT_TAG_SMT2) {
      cout << "unexpected rotate left operator" << endl;
      return 0;
   }
   else if (type == WOL_ROTATE_RIGHT_TAG_SMT2) {
      cout << "unexpected rotate right operator" << endl;
      return 0;
   }
   else { 
      assert(0);
   }

   if (type == WOL_BVNEG_TAG_SMT2 || type == WOL_BVNOT_TAG_SMT2) {
       WolNodeSptr exp1;
       if(!wol_parse_term_smt2(exp1))  
           return 0;
       exp = (WolExpFactory::getInstance().*unary_func)(exp1); 
   }
   else {
      WolNodeSptr exp1, exp2;
      if(!wol_parse_term_smt2(exp1))
         return 0;
      if(!wol_parse_term_smt2(exp2)) {
         return 0;
      }
      if (exp1->getWidth() != exp2->getWidth()) {
          return 0;
      }
         
      exp = (WolExpFactory::getInstance().*binary_func)(exp1, exp2);
   }
   return 1;
}

int WolSmt2Parser::wol_parse_logic_term_smt2(int type, WolNodeSptr& exp) {
   
   WolNodeSptr exp1 = 0;
   int result = 0;

   if (type == WOL_BOOL_TAG_SMT2) {
      cout << "unexpected token bool " << endl;
      return result;
   }
   else if (type == WOL_TRUE_TAG_SMT2) {
      exp = WolExpFactory::getInstance().makeTrueExpr();
      if(!wol_read_rpar_smt2("")) return result;
      result = 1;
   }
   else if (type == WOL_FALSE_TAG_SMT2) {
      exp = WolExpFactory::getInstance().makeFalseExpr();
      if(!wol_read_rpar_smt2("")) return result;
      result = 1;
   }
   else if (type == WOL_NOT_TAG_SMT2) {
      if (!wol_parse_term_smt2(exp1)) {
         cout << "expected single argument for 'not' operator" << endl;
         return result;
      }   
      if (exp1->getWidth() != 1) {
         cout << "unexpected bit-vector of width" << exp->getWidth() << "as argument to 'not'"  << endl;
         return result;  
      }
      exp = WolExpFactory::getInstance().makeNotExpr(exp1);
      if(!wol_read_rpar_smt2("after single argument")) return result;
      result = 1;   
   }
   else if (type == WOL_ITE_TAG_SMT2) {
      WolNodeSptr exp2, exp3;   
      if (!wol_parse_term_smt2(exp1)) {
         cout << "expected three arguments for 'ite' operator" << endl;
         return result;
      }   
      if (!wol_parse_term_smt2(exp2)){
         cout << "expected three arguments for 'ite' operator" << endl;   
         return result;
      }      
      if (!wol_parse_term_smt2(exp3)){
         cout << "expected three arguments for 'ite' operator" << endl;   
      }
         
      if (exp1->getWidth() != 1) {
         cout << "first argument of 'ite' is bit-vector of bit-width" << exp1->getWidth() << endl;   
      }   
      else if (exp2->getWidth() != exp3->getWidth()) {
         cout << "second argument of 'ite' is bit-vector of width" << exp2->getWidth() << "third argument is bit"            "vector of width" << exp3->getWidth() << endl; 
      }   
      else {
         exp = WolExpFactory::getInstance().makeCondExpr(exp1, exp2, exp3);
         result = 1;   
      }   
      if(!wol_read_rpar_smt2("after ITE arguments")) return result;   
   }    
   else { 
      // collect parametric number of arguments
      std::string op = _token;
      std::vector<WolNodeSptr> args;
      WolNodeSptr arg = 0;
      while (wol_parse_term_smt2(arg)) {
         args.push_back(arg);
         arg = 0;   
      }   
      if (_token.compare(")0")) {
         cout << "expected token ')'" << endl;
         return result;   
      }   

      int nargs = args.size();

      if (nargs == 0) {
         cout << "no arguments for operator" << op << endl;
         return result;
      }  

      if (nargs == 1) {
         cout << " minimum two arguments are expected for operator" << op << endl;
         return result;
      }
   
      int width = args[0]->getWidth();
      int del_exp = 0;
      for (WolNodeSptr arg : args) {
         if (arg->getWidth() != width){
            cout << "arguments are of different widths for the operator " << op << endl;   
            del_exp = 1;
         }
      }
      if (del_exp) {
         return result;
      }             

      WolNodeSptr temp;
      WolNodeSptr old;
      if (type == WOL_DISTINCT_TAG_SMT2) {
         exp = WolExpFactory::getInstance().makeTrueExpr();
         for (int i = 0; i < (nargs-1); i++) {
            for(int j = i + 1; j < nargs; j++) {
               temp = WolExpFactory::getInstance().makeNeExpr(args[i], args[j]);
               old = exp;
               exp = WolExpFactory::getInstance().makeAndExpr(old, temp);
            }
         }
         result = 1;      
      }
      else if (type == WOL_EQUAL_TAG_SMT2) {
         exp = WolExpFactory::getInstance().makeTrueExpr();
         for (int i = 0; i < (nargs-1); i++) {
            temp = WolExpFactory::getInstance().makeEqExpr(args[i], args[i + 1]);
            old = exp;
            exp = WolExpFactory::getInstance().makeAndExpr(old, temp);
         }
         result = 1;
      }
      else {
         if (width != 1) {
            cout << "arguments should be of width 1 for the operator " << op << endl;
            result = 0;
         }
         else {
            exp = 0;
            if (type == WOL_IMPLIES_TAG_SMT2) {
               for (int i = nargs-1; i >= 0; i--) {
                  if (exp) {
                     old = exp;
                     exp = WolExpFactory::getInstance().makeImpliesExpr(args[i], old);
                  }
                  else {
                     exp = args[i];
                     exp->incRefCount();
                  }
               }
            }
            else if (type == WOL_AND_TAG_SMT2) {
               for (int i = 0; i < nargs; i++) {
                  if (exp) {
                     old = exp;
                     exp = WolExpFactory::getInstance().makeAndExpr(old, args[i]);
                  }
                  else {
                     exp = args[i];
                     exp->incRefCount();
                  }
               }
            }
            else if (type == WOL_OR_TAG_SMT2) {
               for (int i = 0; i < nargs; i++) {
                  if (exp) {
                     old = exp;
                     exp = WolExpFactory::getInstance().makeOrExpr(old, args[i]);
                  }
                  else {
                     exp = args[i];
                     exp->incRefCount();
                  }
               }
            }
            else if (type == WOL_XOR_TAG_SMT2) {
               for (int i = 0; i < nargs; i++) {
                  if (exp) {
                     old = exp;
                     exp = WolExpFactory::getInstance().makeXorExpr(old, args[i]);
                  }
                  else {
                     exp = args[i];
                     exp->incRefCount();
                  }
               }
            }
            else {
               assert(0);  
            }
            result = 1;
         }
      }
   }

   return result;   
}

int WolSmt2Parser::wol_parse_underscore_term_smt2(WolNodeSptr& exp) {
   
   int term_type = wol_read_token_smt2();
   int type;
   int high, low, param;
   WolNodeSptr exp1;
   if (term_type == EOF || term_type == WOL_INVALID_TAG_SMT2) {
      cout << "unexpected end-of-file or invalid token" << endl;
      return 0;
   }
         
   switch(term_type) {
            
      case WOL_EXTRACT_TAG_SMT2 :
         type = wol_read_token_smt2();
             
         if (type == EOF || type == WOL_INVALID_TAG_SMT2) {
            cout << "unexpected end-of-file or invalid token" << endl;
            return 0;
         }
	
         if (type != WOL_DECIMAL_CONSTANT_TAG_SMT2) {
             cout << "expected bit index at " << _token << endl;
             return 0;
         }
         
         high = atoi(_token.c_str());

         type = wol_read_token_smt2();
             
         if (type == EOF || type == WOL_INVALID_TAG_SMT2) {
            cout << "unexpected end-of-file or invalid token" << endl;
            return 0;
         }

         if (type != WOL_DECIMAL_CONSTANT_TAG_SMT2) {
            cout << "expected bit index at " << _token << endl;
            return 0;
         }
         low = atoi(_token.c_str());
         assert(high >= low);

         type = wol_read_token_smt2();

         if (type == EOF || type == WOL_INVALID_TAG_SMT2) {
            cout << "unexpected end-of-file or invalid token" << endl;
            return 0;
         }

         if (type != WOL_RPAR_TAG_SMT2) {
            cout << "expected ')' at " << _token << endl;
            return 0;
         }
      
         if(!wol_parse_term_smt2(exp1))
            return 0;

         exp = WolExpFactory::getInstance().makeSliceExpr(exp1, high, low);
         return 1;
         break;
         
      case WOL_REPEAT_TAG_SMT2 :
      case WOL_ZERO_EXTEND_TAG_SMT2 :
      case WOL_SIGN_EXTEND_TAG_SMT2 :
      case WOL_ROTATE_LEFT_TAG_SMT2 :
      case WOL_ROTATE_RIGHT_TAG_SMT2 :
         type = wol_read_token_smt2();
   
         if (type == EOF || type == WOL_INVALID_TAG_SMT2) {
            cout << "unexpected end-of-file or invalid token" << endl;
            return 0;
         } 
	       
         if (type != WOL_DECIMAL_CONSTANT_TAG_SMT2) {
            cout << "expected bit index at " << _token << endl;
            return 0;
         }
         param = atoi(_token.c_str());
         type = wol_read_token_smt2();

         if (type == EOF || type == WOL_INVALID_TAG_SMT2) {
            cout << "unexpected end-of-file or invalid token" << endl;
            return 0;
         }

         if (type != WOL_RPAR_TAG_SMT2) {
            cout << "expected ')' at " << _token << endl;
            return 0;
         }
         break;
      default :
         cout << "unexpected token " << _token << endl;
         return 0;
   }
   
   WolNodeSptr exp2;
   if(!wol_parse_term_smt2(exp2))
      return 0;

   switch (term_type) {
      case WOL_REPEAT_TAG_SMT2 :
         exp = WolExpFactory::getInstance().makeRepeatExpr(exp2, param);
         break;     
      case WOL_ZERO_EXTEND_TAG_SMT2 :
         exp = WolExpFactory::getInstance().makeUextExpr(exp2, param);
         break;
      case WOL_SIGN_EXTEND_TAG_SMT2 :
         exp = WolExpFactory::getInstance().makeSextExpr(exp2, param);
         break;
      case WOL_ROTATE_LEFT_TAG_SMT2 :
         exp = WolExpFactory::getInstance().makeRolExpr(exp2, param);
         break;
      case WOL_ROTATE_RIGHT_TAG_SMT2 :
         exp = WolExpFactory::getInstance().makeRorExpr(exp2, param);
         break;
      default :
         assert(0); 
   }
   
   return 1; 
}

/*
   Term expression grammar:
   
   <var_binding>  ::= ( WOL_SYMBOL_TAG_SMT2 <term> )     
   <term>      ::=    WOL_CONSTANT_TAG_CLASS_SMT2
                |     WOL_SYMBOL_TAG_SMT2
                |     ( WOL_BITVEC_TAG_CLASS_SMT2 <term>+ ) 
                |     ( WOL_CORE_TAG_CLASS_SMT2) <term>+ )
                |     ( ( WOL_UNDERSCORE_TAG_SMT2 WOL_BITVEC_TAG_CLASS_SMT2 <WOL_CONSTANT_TAG_CLASS_SMT2>+ ) <term> )
                |     ( WOL_LET_TAG_SMT2 ( <var_binding>+ ) <term> )
*/

int WolSmt2Parser::wol_parse_let_binding_smt2(WolNodeSptr& exp) {
   
   int type = wol_read_token_smt2();
   int result = 1;
   if (type == EOF || type == WOL_INVALID_TAG_SMT2) {
      cout << "unexpected end-of-file or invalid token" << endl;
      return 0;
   }

   if (type != WOL_LPAR_TAG_SMT2) {
      cout << "expected '(' at" << _token << endl;
      return 0;
   } 
    
   std::vector<string> currStrings;
   std::string letToken;
   while((type = wol_read_token_smt2()) != WOL_RPAR_TAG_SMT2) {

      if (type != WOL_LPAR_TAG_SMT2) {
         cout << "expected '(' at" << _token << endl;
         result = 0;
         break;
      }
      
      if(!wol_read_symbol("")) {
         result = 0;
         break;
      }
      letToken = _token;      
 
      if(_lastNode->getWolNode()) {
         cout << _token  << "is already declared as variable" << endl;
         result = 0;
         break;
      }   
      else {
         WolNodeSptr tmp; 
         if (!wol_parse_term_smt2(tmp)) { 
            result = 0;
            break;       
         }
         
         if (_letMap.find(letToken) == _letMap.end())
            _letMap[letToken] = new std::vector<WolNodeSptr>();      
         _letMap.find(letToken)->second->push_back(tmp); 
            
         currStrings.push_back(letToken);      
      }
      
      if(!wol_read_rpar_smt2("after a let binding")) { 
         result = 0;      
         break; 
      }

   }

   if (result) {
      if (!wol_parse_term_smt2(exp)) result = 0;
   }
   
   if (result) {
      if(!wol_read_rpar_smt2("after let binding term"))  result = 0;
   }

   for(auto i : currStrings) {
      std::vector<WolNodeSptr> *iv = _letMap.find(i)->second;
      iv->pop_back();
      if (iv->empty()) {
          delete iv;  
         _letMap.erase(i);
      }
   }

   return result;
}

static std::string convert_dec_to_bin(std::string s) {

   int i,j,c= 0;
   std::string binarys;
   int next = 0;
   int et = s.length();
   do {
      int r = 0;
      int st = next;
      while (st < et)
      {
         int d = 0;
         d = r * 10 + s[st]-'0';
         r = d%2;
         d = d/2;
         s[st++] = d+'0';
      }
      binarys.push_back(r+'0');
      while( next < et && s[next]=='0')
         next++;
   } while(next < et);

   // reverse the string binarys
   for (i = 0, j = (binarys.length())-1; i < j; i++, j--) {
      c = binarys[i];
      binarys[i] = binarys[j];
      binarys[j] = c;
   }

   return binarys;
}

int WolSmt2Parser::wol_parse_bv_const_smt2(WolNodeSptr& exp) {
   
   int type = wol_read_token_smt2();
   if (type == EOF || type == WOL_INVALID_TAG_SMT2) {
      cout << "unexpected end-of-file or invalid token" << endl;
      return 0;
   }
         
   if (!((type == WOL_SYMBOL_TAG_SMT2) && (_token[0] == 'b') && (_token[1] == 'v') && (_token.size() > 2))) {
      cout << "expected bv constant at " << _token << endl;
      return 0;
   }

   std::string constString = _token.substr(2);
   constString = convert_dec_to_bin(constString);
   int length = constString.length();     
 
   type = wol_read_token_smt2();
   if (type == EOF || type == WOL_INVALID_TAG_SMT2) {
      cout << "unexpected end-of-file or invalid token" << endl;
      return 0;
   } 

   if (type != WOL_DECIMAL_CONSTANT_TAG_SMT2) {
      cout << "expected bit width at " << _token << endl;
      return 0;
   }
   int width = atoi(_token.c_str());

   if (length > width) {
      cout << "insufficient bit width at " << _token << endl;
      return 0;
   }
   else if(length == width) {
      exp = WolExpFactory::getInstance().makeConstExpr(constString, 2);
   }   
   else if(!length) {
      exp = WolExpFactory::getInstance().makeZeroExpr(width);
   }
   else {
      std::string temp(width -length, '0');
      constString =  (temp + constString);
      exp = WolExpFactory::getInstance().makeConstExpr(constString, 2);
      assert(exp != NULL);
   }
         
   if(!wol_read_rpar_smt2("after bv constant")) {
      return 0;
   }
      
   return 1;
}

int WolSmt2Parser::wol_parse_term_smt2(WolNodeSptr& exp) {

   int type = wol_read_token_smt2();
   
   if (type == EOF || type == WOL_INVALID_TAG_SMT2) return 0;
   
   if (type & WOL_CONSTANT_TAG_CLASS_SMT2) {

      _token.pop_back();
      if (type == WOL_BINARY_CONSTANT_TAG_SMT2) {
         exp = WolExpFactory::getInstance().makeConstExpr(_token.substr(2), 2);   
         return 1;       
      }
      else if (type == WOL_HEXADECIMAL_CONSTANT_TAG_SMT2) {
         exp = WolExpFactory::getInstance().makeConstExpr(_token.substr(2), 16);
         return 1;
      }
      else {
          cout << "expected binary or hexadecimal constant at" << _token << endl;
          return 0;
      }
   } 
   else if (type == WOL_SYMBOL_TAG_SMT2) {
      if (_lastNode->getWolNode()) {
         exp = _lastNode->getWolNode();
         exp->incRefCount();
         return 1;
      }      
      else if (_letMap.find(_token) != _letMap.end()) {
         exp = _letMap.find(_token)->second->back();
         exp->incRefCount();
         return 1;
      }
      else {
         cout << "undefined symbol...... " << _token << endl;
         return 0;
      }
   }
   else if (type == WOL_TRUE_TAG_SMT2) {
      exp = WolExpFactory::getInstance().makeTrueExpr();
      return 1;
   }
   else if (type == WOL_FALSE_TAG_SMT2) {
      exp = WolExpFactory::getInstance().makeFalseExpr();
      return 1;
   }
   else if (type == WOL_LPAR_TAG_SMT2){
      type = wol_read_token_smt2();
      if (type == EOF || type == WOL_INVALID_TAG_SMT2) {
         cout << "unexpected end-of-file or invalid token" << endl;
         return 0;
      }
      
      if (type & WOL_BITVEC_TAG_CLASS_SMT2) { 
         if(!wol_parse_bitvec_term_smt2(type, exp))
            return 0;
         
         std::string op = _token;   
         type = wol_read_token_smt2();
         if (type == EOF || type == WOL_INVALID_TAG_SMT2) {
            cout << "unexpected end-of-file or invalid token" << endl;
            return 0;
         }

         if (type != WOL_RPAR_TAG_SMT2) {
            cout << "wrong number of arguements for operator" << op << endl;
            return 0;  
         }              
      }
      else if (type & WOL_CORE_TAG_CLASS_SMT2) {
         if(!wol_parse_logic_term_smt2(type, exp))
            return 0;
      }
      else if (type ==  WOL_LPAR_TAG_SMT2) {

         type = wol_read_token_smt2();
         if (type == EOF || type == WOL_INVALID_TAG_SMT2) {
            cout << "unexpected end-of-file or invalid token" << endl;
            return 0;
         }
         
         if (!(type == WOL_UNDERSCORE_TAG_SMT2)) {
            cout << "expected '_' at " << _token << endl;
            return 0;
         }
   
         if(!wol_parse_underscore_term_smt2(exp))
            return 0;
         
         type = wol_read_token_smt2();
         if (type == EOF || type == WOL_INVALID_TAG_SMT2) {
            cout << "unexpected end-of-file or invalid token" << endl;
            return 0;
         }
         
         if (type != WOL_RPAR_TAG_SMT2) {
            cout << "wrong number of arguments for underscore operator" << endl;
            return 0;
         }            
                                        
      }
      else if (type == WOL_LET_TAG_SMT2) {
         if(!wol_parse_let_binding_smt2(exp)) 
            return 0;        
      }
      else if (type == WOL_UNDERSCORE_TAG_SMT2) {
         if(!wol_parse_bv_const_smt2(exp))
            return 0;
      }
      else {
         cout << "expected '(' or let or operator " << _token << endl;
         return 0;
      }
   }
   else {
      if (_token.compare(")0"))
      cout << "undefined symbol " << _token << endl;
      return 0;
   }

   return 1;  
}

int WolSmt2Parser:: wol_skip_sexprs() {

   int type, open = 1;
   while (open > 0) {
      type = wol_read_token_smt2();
      if (type == EOF) {
         if (open > 0) {
            cout << "')' missing at end-of-file" << endl;
            return 0;
         }
         return 1;
      }
      if (type == WOL_INVALID_TAG_SMT2) return 0;
      else if (type == WOL_LPAR_TAG_SMT2) open++;
      else if (type == WOL_RPAR_TAG_SMT2) open--;
   }   
   return 1;

}

int WolSmt2Parser::wol_set_info_smt2() {

   int type = wol_read_token_smt2();
   if (type == EOF || type == WOL_INVALID_TAG_SMT2) {
      cout << "unexpected end-of-file or invalid token" << endl;
      return 0;
   }
   
   if (type == WOL_RPAR_TAG_SMT2) {
      cout << "keyword after 'set-info' missing" << endl ;
      return 0;
   }

   if (type == WOL_STATUS_TAG_SMT2) {
      type = wol_read_token_smt2();
      if (type == EOF || type == WOL_INVALID_TAG_SMT2) {
         cout << "unexpected end-of-file or invalid token after ':status'" << endl;
         return 0;
      }

      if (type == WOL_RPAR_TAG_SMT2) {
         cout << "value after ':status' missing" << endl ;
         return 0;
      }

      if (type != WOL_SYMBOL_TAG_SMT2) {
         cout << "invalid value " <<_token << "after ':status'" << endl;
         return 0;
      }
      
      if (!_token.compare("sat")  && !_token.compare("unsat") && !_token.compare("unknown")) {
         cout << "invalid value " <<_token << "after ':status'" << endl;
         return 0;      
      }

      return wol_read_rpar_smt2("after 'set-info'");
   }   

   return wol_skip_sexprs();
}

int WolSmt2Parser::wol_read_token_smt2() {

	_lastcoo = _coo;
	int res = wol_read_token_aux_smt2();
	return res;
}


int WolSmt2Parser::wol_read_command_smt2() {

	int type;
	int _error = 0;
   WolNodeSptr exp = 0;

	type = wol_read_token_smt2();
	if (type == EOF || type == WOL_INVALID_TAG_SMT2) return _error;
	
	if (type != WOL_LPAR_TAG_SMT2) {
      cout << "expected '(' at " << _token << endl;
      return _error;
	}
	type = wol_read_token_smt2();

   if (type == EOF) {
      cout << "unexpected end-of-file after '('" << endl;
      return _error;
   }
   if (type == WOL_INVALID_TAG_SMT2) return _error;
   	
	if (!(type & WOL_COMMAND_TAG_CLASS_SMT2)) {
      cout << "expected command at "<< type << " " << _token << endl; 
		return _error;
   }
   switch (type)
   {
      case WOL_SET_LOGIC_TAG_SMT2:

         type = wol_read_token_smt2 ();
         if (type == EOF || type == WOL_INVALID_TAG_SMT2) {
            cout << "unexpected end-of-file after 'set-logic'" << endl;
            return _error;
         }

         if (!(type & WOL_LOGIC_TAG_CLASS_SMT2)) {
             cout << "expected logic at " << _token << endl; 
             return _error;
         }

         if (type != WOL_QF_BV_TAG_SMT2) {
            cout << "unsuported logic " << _token << endl; 
            return _error;
         }

         type = wol_read_token_smt2();
         if (type == 0 || type == WOL_INVALID_TAG_SMT2) return 0;

         if (type != WOL_RPAR_TAG_SMT2) {
            cout << "expected ')' after " << _token << endl;   
            return _error;
         }
         assert(!commands.set_logic);
         commands.set_logic++;
 
         break;

      case WOL_CHECK_SAT_TAG_SMT2:
         type = wol_read_token_smt2();
         if (type == 0 || type == WOL_INVALID_TAG_SMT2) return _error;

         if (type != WOL_RPAR_TAG_SMT2) {
            cout << "expected ')' after 'check-sat'" << endl;
            return _error;
         }
         assert(!commands.check_sat);
   
         commands.check_sat++;
         break;

      case WOL_DECLARE_FUN_TAG_SMT2:
         if (!wol_declare_fun_smt2()) return _error;
         break;
   
      case WOL_DEFINE_FUN_TAG_SMT2:
         cout << "uninterpreted functions are not supported" << endl;
         return _error;
         break;
      
      case WOL_ASSERT_TAG_SMT2:

         if (!wol_parse_term_smt2 (exp)) return _error;
         
         _outputs.push_back(exp);
         if (exp->isArray()) { 
            cout << "assert argument is an array and not a formula" << endl;
            return _error;
         }
         if (exp->getWidth() != 1) {
            cout << "assert argument is a bit-vector of length " << exp->getWidth() << endl;
            return _error; 
         }
         if (!wol_read_rpar_smt2 (" after asserted expression")) return _error;
         commands.asserts++;
         break;

      case WOL_EXIT_TAG_SMT2:

         type = wol_read_token_smt2();
         if (type == 0 || type == WOL_INVALID_TAG_SMT2) return _error;

         if (type != WOL_RPAR_TAG_SMT2) {
            cout << "expected ')' after exit command" << endl;
            return _error;
         }        

         assert(!commands.exits);
         commands.exits++;
         
         type = wol_read_token_smt2 ();
         if (type == WOL_INVALID_TAG_SMT2) return _error;
         if (type != EOF) {
               cout << "expected end-of-file after 'exit' command at " << _token << endl;
               return _error;
         }
         break;

      case WOL_SET_INFO_TAG_SMT2:
         if(!wol_set_info_smt2()) return 0;
         break;

      default :
         cout << "unsupported command " << _token << endl; 
   }      

   commands.all++;
	return 1;
}

int WolSmt2Parser::parse() {

	while(wol_read_command_smt2());
   if (commands.exits == 0)
      return 0;
   else
      return 1;		
}

void WolSmt2Parser::print() {

   assert(_letMap.size() == 0);
   int numOutputs = _outputs.size();
   for (int i = 0; i < numOutputs; i++) {
      _outputs[i]->print();
      cout << endl ;
   }

}


}
