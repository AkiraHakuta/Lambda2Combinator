grammar lambda2c;

toplevel
    : (command ';')+ 
    ;

command
    : term                           # c_term
    | sbid EQS term                  # c_sbid_eqs_term
    | 'putStr' '(' dqString ')'      # c_dq_string
    | 'NL'                           # c_newline
    ;

term 
    : appterm                        # t_appterm  
    | '\\' var '.' vs                # t_abs_vs 
    | '\\' var '.' ssappterm         # t_abs_ssapp  
    | '\\' var '.' sappterm          # t_abs_sapp       
    | '\\' var '.' term              # t_abs_term
    ;

ssappterm
    : sappterm aterm                 # ssapp_sapp_a
    | '(' ssappterm ')'              # ssapp_paren
    ;

sappterm
    : appterm aterm                  # sapp_app_a
    | '(' sappterm ')'               # sapp_paren
    ;

appterm 
    : aterm                          # app_a
    | appterm aterm                  # app_app_a    
    ;

aterm                                                     
    : vs                             # a_vs
    | '(' '(' term ')' ')'           # a_dparen
    | '(' term ')'                   # a_paren           
    ;

vs    
    : var                            # vs_var
    | combi                          # vs_combi  
    | sbid                           # vs_sbid
    | '(' vs ')'                     # vs_paren 
    ;


combi : COMBINATOR  ;
var : LCID ;
sbid : SBID ;
dqString : StringLiteral;

EQS : [=<:|]*'=';
//EQS : '=' | ':=' | '|=' | '|:=';
COMBINATOR  : [SKIBC] | 'S\'' | 'B\'' | 'C\'';
LCID : [a-z] ;
SBID : '[' [a-zA-Z0-9_']+ ']';
StringLiteral : '"' (~["\\\r\n_])* '"';
WS : [ \t\n\r]+                             -> skip;
COMMENT : '/*' .*? '*/'                     -> skip;
SL_COMMENT :  '//' .*? ('\n'|'<EOF>')       -> skip;
