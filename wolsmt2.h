#ifndef WOLSMT2PARSER_H_INCLUDED
#define WOLSMT2PARSER_H_INCLUDED

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include "common.h"

namespace wolver {

class WolNode;
class WolComplexNode;
class WolMgr;
class WolExpFactory;


class WolSmt2Parser {

public : // constructor and destructor 
	WolSmt2Parser(FILE *input_file);
	~WolSmt2Parser();


public : //enums

	enum WolSMT2TagClass {
		WOL_CLASS_BITS_SMT2                  = 6,
  		WOL_CLASS_SIZE_SMT2                  = (1 << WOL_CLASS_BITS_SMT2),
  		WOL_CLASS_MASK_SMT2                  = (WOL_CLASS_SIZE_SMT2 - 1),

  		WOL_OTHER_TAG_CLASS_SMT2             = 0,

  		WOL_CONSTANT_TAG_CLASS_SMT2          = (WOL_CLASS_SIZE_SMT2 << 0),
  		WOL_RESERVED_TAG_CLASS_SMT2          = (WOL_CLASS_SIZE_SMT2 << 1),
  		WOL_COMMAND_TAG_CLASS_SMT2           = (WOL_CLASS_SIZE_SMT2 << 2),
  		WOL_KEYWORD_TAG_CLASS_SMT2           = (WOL_CLASS_SIZE_SMT2 << 3),
  		WOL_CORE_TAG_CLASS_SMT2              = (WOL_CLASS_SIZE_SMT2 << 4),
  		WOL_ARRAY_TAG_CLASS_SMT2             = (WOL_CLASS_SIZE_SMT2 << 5),
  		WOL_BITVEC_TAG_CLASS_SMT2            = (WOL_CLASS_SIZE_SMT2 << 6),
  		WOL_LOGIC_TAG_CLASS_SMT2             = (WOL_CLASS_SIZE_SMT2 << 7),
	};

	enum WolSMT2Tag {
		WOL_INVALID_TAG_SMT2                 =  0 + WOL_OTHER_TAG_CLASS_SMT2,
  		WOL_PARENT_TAG_SMT2                  =  1 + WOL_OTHER_TAG_CLASS_SMT2,
  		WOL_LPAR_TAG_SMT2                    =  2 + WOL_OTHER_TAG_CLASS_SMT2,
  		WOL_RPAR_TAG_SMT2                    =  3 + WOL_OTHER_TAG_CLASS_SMT2,
  		WOL_SYMBOL_TAG_SMT2                  =  4 + WOL_OTHER_TAG_CLASS_SMT2,
  		WOL_ATTRIBUTE_TAG_SMT2               =  5 + WOL_OTHER_TAG_CLASS_SMT2,
  		WOL_EXP_TAG_SMT2                     =  6 + WOL_OTHER_TAG_CLASS_SMT2,
  		WOL_LETBIND_TAG_SMT2                 =  7 + WOL_OTHER_TAG_CLASS_SMT2,
  		WOL_PARLETBINDING_TAG_SMT2           =  8 + WOL_OTHER_TAG_CLASS_SMT2,

  		WOL_DECIMAL_CONSTANT_TAG_SMT2        =  0 + WOL_CONSTANT_TAG_CLASS_SMT2,
  		WOL_HEXADECIMAL_CONSTANT_TAG_SMT2    =  1 + WOL_CONSTANT_TAG_CLASS_SMT2,
  		WOL_BINARY_CONSTANT_TAG_SMT2         =  2 + WOL_CONSTANT_TAG_CLASS_SMT2,
  		WOL_STRING_CONSTANT_TAG_SMT2         =  3 + WOL_CONSTANT_TAG_CLASS_SMT2,

  		WOL_PAR_TAG_SMT2                     =  0 + WOL_RESERVED_TAG_CLASS_SMT2,
  		WOL_NUMERAL_RESERVED_WORD_TAG_SMT2   =  1 + WOL_RESERVED_TAG_CLASS_SMT2,
  		WOL_DECIMAL_RESERVED_WORD_TAG_SMT2   =  2 + WOL_RESERVED_TAG_CLASS_SMT2,
  		WOL_STRING_RESERVED_WORD_TAG_SMT2    =  3 + WOL_RESERVED_TAG_CLASS_SMT2,
  		WOL_UNDERSCORE_TAG_SMT2              =  4 + WOL_RESERVED_TAG_CLASS_SMT2,
  		WOL_BANG_TAG_SMT2                    =  5 + WOL_RESERVED_TAG_CLASS_SMT2,
  		WOL_AS_TAG_SMT2                      =  6 + WOL_RESERVED_TAG_CLASS_SMT2,
  		WOL_LET_TAG_SMT2                     =  7 + WOL_RESERVED_TAG_CLASS_SMT2,
  		WOL_FORALL_TAG_SMT2                  =  8 + WOL_RESERVED_TAG_CLASS_SMT2,
  		WOL_EXISTS_TAG_SMT2                  =  9 + WOL_RESERVED_TAG_CLASS_SMT2,

 		WOL_SET_LOGIC_TAG_SMT2               =  0 + WOL_COMMAND_TAG_CLASS_SMT2,
		WOL_SET_OPTION_TAG_SMT2              =  1 + WOL_COMMAND_TAG_CLASS_SMT2,
  		WOL_SET_INFO_TAG_SMT2                =  2 + WOL_COMMAND_TAG_CLASS_SMT2,
  		WOL_DECLARE_SORT_TAG_SMT2            =  3 + WOL_COMMAND_TAG_CLASS_SMT2,
  		WOL_DEFINE_SORT_TAG_SMT2             =  4 + WOL_COMMAND_TAG_CLASS_SMT2,
  		WOL_DECLARE_FUN_TAG_SMT2             =  5 + WOL_COMMAND_TAG_CLASS_SMT2,
  		WOL_DEFINE_FUN_TAG_SMT2              =  6 + WOL_COMMAND_TAG_CLASS_SMT2,
  		WOL_PUSH_TAG_SMT2                    =  7 + WOL_COMMAND_TAG_CLASS_SMT2,
  		WOL_POP_TAG_SMT2                     =  8 + WOL_COMMAND_TAG_CLASS_SMT2,
  		WOL_ASSERT_TAG_SMT2                  =  9 + WOL_COMMAND_TAG_CLASS_SMT2,
  		WOL_CHECK_SAT_TAG_SMT2               = 10 + WOL_COMMAND_TAG_CLASS_SMT2,
  		WOL_GET_ASSERTIONS_TAG_SMT2          = 11 + WOL_COMMAND_TAG_CLASS_SMT2,
  		WOL_GET_PROOF_TAG_SMT2               = 12 + WOL_COMMAND_TAG_CLASS_SMT2,
  		WOL_GET_UNSAT_CORE_TAG_SMT2          = 13 + WOL_COMMAND_TAG_CLASS_SMT2,
  		WOL_GET_VALUE_TAG_SMT2               = 14 + WOL_COMMAND_TAG_CLASS_SMT2,
  		WOL_GET_ASSIGNMENT_TAG_SMT2          = 15 + WOL_COMMAND_TAG_CLASS_SMT2,
  		WOL_GET_OPTION_TAG_SMT2              = 16 + WOL_COMMAND_TAG_CLASS_SMT2,
  		WOL_GET_INFO_TAG_SMT2                = 17 + WOL_COMMAND_TAG_CLASS_SMT2,
  		WOL_EXIT_TAG_SMT2                    = 18 + WOL_COMMAND_TAG_CLASS_SMT2,
		
		WOL_ALL_STATISTICS_TAG_SMT2          =  0 + WOL_KEYWORD_TAG_CLASS_SMT2,
  		WOL_AUTHORS_TAG_SMT2                 =  1 + WOL_KEYWORD_TAG_CLASS_SMT2,
  		WOL_AXIOMS_TAG_SMT2                  =  2 + WOL_KEYWORD_TAG_CLASS_SMT2,
  		WOL_CHAINABLE_TAG_SMT2               =  3 + WOL_KEYWORD_TAG_CLASS_SMT2,
  		WOL_DEFINITION_TAG_SMT2              =  4 + WOL_KEYWORD_TAG_CLASS_SMT2,
  		WOL_DIAG_OUTPUT_CHANNEL_TAG_SMT2     =  5 + WOL_KEYWORD_TAG_CLASS_SMT2,
  		WOL_ERROR_BEHAVIOR_TAG_SMT2          =  6 + WOL_KEYWORD_TAG_CLASS_SMT2,
  		WOL_EXPAND_DEFINITIONS_TAG_SMT2      =  7 + WOL_KEYWORD_TAG_CLASS_SMT2,
  		WOL_EXTENSIONS_TAG_SMT2              =  8 + WOL_KEYWORD_TAG_CLASS_SMT2,
		WOL_FUNS_TAG_SMT2                    =  9 + WOL_KEYWORD_TAG_CLASS_SMT2,
		WOL_FUNS_DESCRIPTION_TAG_SMT2        = 10 + WOL_KEYWORD_TAG_CLASS_SMT2,
		WOL_INTERACTIVE_MODE_TAG_SMT2        = 11 + WOL_KEYWORD_TAG_CLASS_SMT2,
		WOL_LANGUAGE_TAG_SMT2                = 12 + WOL_KEYWORD_TAG_CLASS_SMT2,
		WOL_LEFT_ASSOC_TAG_SMT2              = 13 + WOL_KEYWORD_TAG_CLASS_SMT2,
		WOL_NAME_TAG_SMT2                    = 14 + WOL_KEYWORD_TAG_CLASS_SMT2,
		WOL_NAMED_TAG_SMT2                   = 15 + WOL_KEYWORD_TAG_CLASS_SMT2,
		WOL_NOTES_TAG_SMT2                   = 16 + WOL_KEYWORD_TAG_CLASS_SMT2,
		WOL_PRINT_SUCCESS_TAG_SMT2           = 17 + WOL_KEYWORD_TAG_CLASS_SMT2,
		WOL_PRODUCE_ASSIGNMENTS_TAG_SMT2     = 18 + WOL_KEYWORD_TAG_CLASS_SMT2,
		WOL_PRODUCE_MODELS_TAG_SMT2          = 19 + WOL_KEYWORD_TAG_CLASS_SMT2,
		WOL_PRODUCE_PROOFS_TAG_SMT2          = 20 + WOL_KEYWORD_TAG_CLASS_SMT2,
		WOL_PRODUCE_UNSAT_CORES_TAG_SMT2     = 21 + WOL_KEYWORD_TAG_CLASS_SMT2,
		WOL_RANDOM_SEED_TAG_SMT2             = 22 + WOL_KEYWORD_TAG_CLASS_SMT2,
		WOL_REASON_UNKNOWN_TAG_SMT2          = 23 + WOL_KEYWORD_TAG_CLASS_SMT2,
		WOL_REGULAR_OUTPUT_CHANNEL_TAG_SMT2  = 24 + WOL_KEYWORD_TAG_CLASS_SMT2,
		WOL_RIGHT_ASSOC_TAG_SMT2             = 25 + WOL_KEYWORD_TAG_CLASS_SMT2,
		WOL_SORTS_TAG_SMT2                   = 26 + WOL_KEYWORD_TAG_CLASS_SMT2,
		WOL_SORTS_DESCRIPTION_TAG_SMT2       = 27 + WOL_KEYWORD_TAG_CLASS_SMT2,
		WOL_STATUS_TAG_SMT2                  = 28 + WOL_KEYWORD_TAG_CLASS_SMT2,
		WOL_THEORIES_TAG_SMT2                = 29 + WOL_KEYWORD_TAG_CLASS_SMT2,
		WOL_VALUES_TAG_SMT2                  = 30 + WOL_KEYWORD_TAG_CLASS_SMT2,
		WOL_VERBOSITY_TAG_SMT2               = 31 + WOL_KEYWORD_TAG_CLASS_SMT2,
		WOL_VERSION_TAG_SMT2                 = 32 + WOL_KEYWORD_TAG_CLASS_SMT2,

		WOL_BOOL_TAG_SMT2                    =  0 + WOL_CORE_TAG_CLASS_SMT2,
		WOL_TRUE_TAG_SMT2                    =  1 + WOL_CORE_TAG_CLASS_SMT2,
		WOL_FALSE_TAG_SMT2                   =  2 + WOL_CORE_TAG_CLASS_SMT2,
		WOL_NOT_TAG_SMT2                     =  3 + WOL_CORE_TAG_CLASS_SMT2,
		WOL_IMPLIES_TAG_SMT2                 =  4 + WOL_CORE_TAG_CLASS_SMT2,
		WOL_AND_TAG_SMT2                     =  5 + WOL_CORE_TAG_CLASS_SMT2,
		WOL_OR_TAG_SMT2                      =  6 + WOL_CORE_TAG_CLASS_SMT2,
		WOL_XOR_TAG_SMT2                     =  7 + WOL_CORE_TAG_CLASS_SMT2,
		WOL_EQUAL_TAG_SMT2                   =  8 + WOL_CORE_TAG_CLASS_SMT2,
		WOL_DISTINCT_TAG_SMT2                =  9 + WOL_CORE_TAG_CLASS_SMT2,
		WOL_ITE_TAG_SMT2                     = 10 + WOL_CORE_TAG_CLASS_SMT2,

		WOL_ARRAY_TAG_SMT2                   =  0 + WOL_ARRAY_TAG_CLASS_SMT2,
		WOL_SELECT_TAG_SMT2                  =  1 + WOL_ARRAY_TAG_CLASS_SMT2,
		WOL_STORE_TAG_SMT2                   =  2 + WOL_ARRAY_TAG_CLASS_SMT2,

		WOL_BITVEC_TAG_SMT2                  =  0 + WOL_BITVEC_TAG_CLASS_SMT2,
		WOL_CONCAT_TAG_SMT2                  =  1 + WOL_BITVEC_TAG_CLASS_SMT2,
		WOL_EXTRACT_TAG_SMT2                 =  2 + WOL_BITVEC_TAG_CLASS_SMT2,
		WOL_BVNOT_TAG_SMT2                   =  3 + WOL_BITVEC_TAG_CLASS_SMT2,
		WOL_BVNEG_TAG_SMT2                   =  4 + WOL_BITVEC_TAG_CLASS_SMT2,
		WOL_BVAND_TAG_SMT2                   =  5 + WOL_BITVEC_TAG_CLASS_SMT2,
		WOL_BVOR_TAG_SMT2                    =  6 + WOL_BITVEC_TAG_CLASS_SMT2,
		WOL_BVADD_TAG_SMT2                   =  7 + WOL_BITVEC_TAG_CLASS_SMT2,
		WOL_BVMUL_TAG_SMT2                   =  8 + WOL_BITVEC_TAG_CLASS_SMT2,
		WOL_BVUDIV_TAG_SMT2                  =  9 + WOL_BITVEC_TAG_CLASS_SMT2,
		WOL_BVUREM_TAG_SMT2                  = 10 + WOL_BITVEC_TAG_CLASS_SMT2,
		WOL_BVSHL_TAG_SMT2                   = 11 + WOL_BITVEC_TAG_CLASS_SMT2,
		WOL_BVLSHR_TAG_SMT2                  = 12 + WOL_BITVEC_TAG_CLASS_SMT2,
		WOL_BVULT_TAG_SMT2                   = 13 + WOL_BITVEC_TAG_CLASS_SMT2,
		WOL_BVNAND_TAG_SMT2                  = 14 + WOL_BITVEC_TAG_CLASS_SMT2,
		WOL_BVNOR_TAG_SMT2                   = 15 + WOL_BITVEC_TAG_CLASS_SMT2,
		WOL_BVXOR_TAG_SMT2                   = 16 + WOL_BITVEC_TAG_CLASS_SMT2,
		WOL_BVXNOR_TAG_SMT2                  = 17 + WOL_BITVEC_TAG_CLASS_SMT2,
		WOL_BVCOMP_TAG_SMT2                  = 18 + WOL_BITVEC_TAG_CLASS_SMT2,
		WOL_BVSUB_TAG_SMT2                   = 19 + WOL_BITVEC_TAG_CLASS_SMT2,
		WOL_BVSDIV_TAG_SMT2                  = 20 + WOL_BITVEC_TAG_CLASS_SMT2,
		WOL_BVSREM_TAG_SMT2                  = 21 + WOL_BITVEC_TAG_CLASS_SMT2,
		WOL_BVSMOD_TAG_SMT2                  = 22 + WOL_BITVEC_TAG_CLASS_SMT2,
		WOL_BVASHR_TAG_SMT2                  = 23 + WOL_BITVEC_TAG_CLASS_SMT2,
		WOL_REPEAT_TAG_SMT2                  = 24 + WOL_BITVEC_TAG_CLASS_SMT2,
		WOL_ZERO_EXTEND_TAG_SMT2             = 25 + WOL_BITVEC_TAG_CLASS_SMT2,
		WOL_SIGN_EXTEND_TAG_SMT2             = 26 + WOL_BITVEC_TAG_CLASS_SMT2,
		WOL_ROTATE_LEFT_TAG_SMT2             = 27 + WOL_BITVEC_TAG_CLASS_SMT2,
		WOL_ROTATE_RIGHT_TAG_SMT2            = 28 + WOL_BITVEC_TAG_CLASS_SMT2,
		WOL_BVULE_TAG_SMT2                   = 29 + WOL_BITVEC_TAG_CLASS_SMT2,
		WOL_BVUGT_TAG_SMT2                   = 30 + WOL_BITVEC_TAG_CLASS_SMT2,
		WOL_BVUGE_TAG_SMT2                   = 31 + WOL_BITVEC_TAG_CLASS_SMT2,
		WOL_BVSLT_TAG_SMT2                   = 32 + WOL_BITVEC_TAG_CLASS_SMT2,
		WOL_BVSLE_TAG_SMT2                   = 33 + WOL_BITVEC_TAG_CLASS_SMT2,
		WOL_BVSGT_TAG_SMT2                   = 34 + WOL_BITVEC_TAG_CLASS_SMT2,
		WOL_BVSGE_TAG_SMT2                   = 35 + WOL_BITVEC_TAG_CLASS_SMT2,

		WOL_AUFLIA_TAG_SMT2                  =  0 + WOL_LOGIC_TAG_CLASS_SMT2,
		WOL_AUFLIRA_TAG_SMT2                 =  1 + WOL_LOGIC_TAG_CLASS_SMT2,
		WOL_AUFNIRA_TAG_SMT2                 =  2 + WOL_LOGIC_TAG_CLASS_SMT2,
		WOL_LRA_TAG_SMT2                     =  3 + WOL_LOGIC_TAG_CLASS_SMT2,
		WOL_QF_ABV_TAG_SMT2                  =  4 + WOL_LOGIC_TAG_CLASS_SMT2,
		WOL_QF_AUFBV_TAG_SMT2                =  5 + WOL_LOGIC_TAG_CLASS_SMT2,
		WOL_QF_AUFLIA_TAG_SMT2               =  6 + WOL_LOGIC_TAG_CLASS_SMT2,
		WOL_QF_AX_TAG_SMT2                   =  7 + WOL_LOGIC_TAG_CLASS_SMT2,
		WOL_QF_BV_TAG_SMT2                   =  8 + WOL_LOGIC_TAG_CLASS_SMT2,
		WOL_QF_IDL_TAG_SMT2                  =  9 + WOL_LOGIC_TAG_CLASS_SMT2,
		WOL_QF_LIA_TAG_SMT2                  = 10 + WOL_LOGIC_TAG_CLASS_SMT2,
		WOL_QF_LRA_TAG_SMT2                  = 11 + WOL_LOGIC_TAG_CLASS_SMT2,
		WOL_QF_NIA_TAG_SMT2                  = 12 + WOL_LOGIC_TAG_CLASS_SMT2,
		WOL_QF_NRA_TAG_SMT2                  = 13 + WOL_LOGIC_TAG_CLASS_SMT2,
		WOL_QF_RDL_TAG_SMT2                  = 14 + WOL_LOGIC_TAG_CLASS_SMT2,
		WOL_QF_UF_TAG_SMT2                   = 15 + WOL_LOGIC_TAG_CLASS_SMT2,
		WOL_QF_UFBV_TAG_SMT2                 = 16 + WOL_LOGIC_TAG_CLASS_SMT2,
		WOL_QF_UFIDL_TAG_SMT2                = 17 + WOL_LOGIC_TAG_CLASS_SMT2,
		WOL_QF_UFLIA_TAG_SMT2                = 18 + WOL_LOGIC_TAG_CLASS_SMT2,
		WOL_QF_UFLRA_TAG_SMT2                = 19 + WOL_LOGIC_TAG_CLASS_SMT2,
		WOL_QF_UFNRA_TAG_SMT2                = 20 + WOL_LOGIC_TAG_CLASS_SMT2,
		WOL_UFLRA_TAG_SMT2                   = 21 + WOL_LOGIC_TAG_CLASS_SMT2,
		WOL_UFNIA_TAG_SMT2                   = 22 + WOL_LOGIC_TAG_CLASS_SMT2,
	};



	enum WolSMT2CharClass {
	  	WOL_DECIMAL_DIGIT_CHAR_CLASS_SMT2            = (1<<0),
	  	WOL_HEXADECIMAL_DIGIT_CHAR_CLASS_SMT2        = (1<<1),
	  	WOL_STRING_CHAR_CLASS_SMT2                   = (1<<2),
	  	WOL_SYMBOL_CHAR_CLASS_SMT2                   = (1<<3),
	  	WOL_QUOTED_SYMBOL_CHAR_CLASS_SMT2            = (1<<4),
	  	WOL_KEYWORD_CHAR_CLASS_SMT2                  = (1<<5),
	};


public : // local classes

	class WolSMT2Node {
	public:
		WolSMT2Node(std::string name, WolSMT2Tag type) :
			_name(name), _type(type) {
         _exp = NULL;
         _next = NULL;
      }
	   	~WolSMT2Node();			

		std::string getName() {return _name;}
		WolSMT2Tag getType() {return _type;}
		unsigned getBound() {return _bound;}
	   WolNodeSptr getWolNode() {return _exp;}
		WolSMT2Node *getNext() {return _next;}

		void setType(WolSMT2Tag type) {_type = type;}
		void setName(std::string name) {_name = name;}
		void setBound(unsigned bound) {_bound = bound;}
		void setWolNode(WolNodeSptr exp) {_exp = exp;}
		void setNextNode(WolSMT2Node *next) {_next = next;}

	private: 
		std::string _name;
		WolSMT2Tag _type;
	  	unsigned _bound:1;
		std::pair<int,int> _coo;
		WolNodeSptr _exp;
	  	WolSMT2Node *_next;
	};

   class commandsData {
   public:
      int all;
      int set_logic;
      int asserts;
      int check_sat;
      int exits;
   };


public : //functions
	int parse();
        void print();
        std::vector<WolNodeSptr> getInputs() {return _inputs;}
        std::vector<WolNodeSptr> getOutputs() {return _outputs;}

private : // functions
	void wol_init_char_classes_smt();
	void wol_insert_keywords_smt2();
	void wol_insert_reserved_words_smt2();
	void wol_insert_commands_smt2();
	void wol_insert_core_symbols_smt2();
  	void wol_insert_smt2(std::string str, WolSMT2Tag type);
   void wol_insert_bitvec_symbols_smt2();
   void wol_insert_logics_smt2();
	int wol_read_command_smt2();
	int wol_read_token_smt2();
	int wol_read_token_aux_smt2();
	void wol_savech_smt2(char ch);
	int wol_nextch_smt2();
	unsigned char wol_cc_smt2(int ch);
	void wol_insert_symbol_smt2(WolSMT2Node *node);
   int wol_read_rpar_smt2(const char *msg);
   int wol_read_lpar_smt2(const char *msg);
   int wol_read_symbol(const char *str);
   int wol_declare_fun_smt2();
   int wol_set_info_smt2();
   int wol_parse_bitvec_sort_smt2(int skiplu, int &width);
   int wol_parse_bitvec_term_smt2(int type, WolNodeSptr& exp);
   int wol_parse_logic_term_smt2(int type, WolNodeSptr& exp);
   int wol_parse_underscore_term_smt2(WolNodeSptr& exp);
   int wol_parse_let_binding_smt2(WolNodeSptr& exp);
   int wol_parse_term_smt2(WolNodeSptr& exp);
   int wol_parse_bv_const_smt2(WolNodeSptr& exp);   
   int wol_skip_sexprs(); 
   
private : // data
	FILE *_file;
	char *_name;
	std::vector<unsigned char> _cc;
	std::unordered_map<std::string, WolSMT2Node*> _symbolTable;
	std::pair<int, int> _coo, _lastcoo;
	std::string _token;	
	WolSMT2Node *_lastNode;
	bool _saved;
	int _savedch;
   commandsData commands;   
   std::vector<WolNodeSptr> _inputs; 
   std::vector<WolNodeSptr> _outputs;
   std::unordered_map<std::string, std::vector<WolNodeSptr>* > _letMap;
	// symbol table	

};


}

#endif
