#ifndef LAMBDA2C_H
#define LAMBDA2C_H


#include "antlr4-runtime.h"
#include "lambda2cLexer.h"
#include "lambda2cParser.h"
#include "lambda2cVisitor.h"

using namespace antlr4;
using namespace tree;
using namespace std;


class EVAL : public lambda2cVisitor{    
    public:
        bool* changed_ptr;
        vector<string> comm;
        map<string, string> sbid_term;
        bool top;

        any visitToplevel(lambda2cParser::ToplevelContext *context);

        any visitC_term(lambda2cParser::C_termContext *context); 

        any visitC_sbid_eqs_term(lambda2cParser::C_sbid_eqs_termContext *context);    

        any visitC_dq_string(lambda2cParser::C_dq_stringContext *context);  

        any visitC_newline(lambda2cParser::C_newlineContext *context);

        any visitT_appterm(lambda2cParser::T_apptermContext *context);

        any visitT_abs_vs(lambda2cParser::T_abs_vsContext *context);

        any visitT_abs_sapp(lambda2cParser::T_abs_sappContext *context);

        any visitT_abs_ssapp(lambda2cParser::T_abs_ssappContext *context);

        any visitT_abs_term(lambda2cParser::T_abs_termContext *context);

        any visitSsapp_sapp_a(lambda2cParser::Ssapp_sapp_aContext *context);

        any visitSsapp_paren(lambda2cParser::Ssapp_parenContext *context);

        any visitSapp_app_a(lambda2cParser::Sapp_app_aContext *context);

        any visitSapp_paren(lambda2cParser::Sapp_parenContext *context);

        any visitApp_a(lambda2cParser::App_aContext *context);

        any visitApp_app_a(lambda2cParser::App_app_aContext *context);

        any visitA_paren(lambda2cParser::A_parenContext *context);

        any visitA_dparen(lambda2cParser::A_dparenContext *context);

        any visitA_vs(lambda2cParser::A_vsContext *context);

        any visitVs_var(lambda2cParser::Vs_varContext *context);

        any visitVs_combi(lambda2cParser::Vs_combiContext *context);

        any visitVs_sbid(lambda2cParser::Vs_sbidContext *context);

        any visitVs_paren(lambda2cParser::Vs_parenContext *context);

        any visitCombi(lambda2cParser::CombiContext *context);

        any visitVar(lambda2cParser::VarContext *context);

        any visitSbid(lambda2cParser::SbidContext *context);

        any visitDqString(lambda2cParser::DqStringContext *context);          
};

#endif  // LAMBDA2C_H