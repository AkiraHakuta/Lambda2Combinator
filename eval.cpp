#include "lambda2c.h"
#include <array>
#include <regex>

any EVAL::visitToplevel(lambda2cParser::ToplevelContext *ctx) {
    if (top)    
        for (int i = 0; i < ctx->command().size(); i++)
            comm.push_back(any_cast<string>(visit(ctx->command(i))));      
    return 1;
}


any EVAL::visitC_term(lambda2cParser::C_termContext *ctx){ 
    string result_str = "";
    if (top)
        result_str = ctx->term()->getText();
    else
        result_str = any_cast<string>(visit(ctx->term())) + ";";
    return result_str;
}


any EVAL::visitC_sbid_eqs_term(lambda2cParser::C_sbid_eqs_termContext *ctx){
    string result_str;
    string sb = ctx->sbid()->getText();
    string t;
    if (top){
        result_str = ctx->getText();
    }
     else {
        result_str = any_cast<string>(visit(ctx->term()));
        sbid_term[sb] = result_str;
        result_str = sb + " " + ctx->EQS()->getText() + " " + result_str + ";";
    }        
    return result_str;
}


any EVAL::visitC_dq_string(lambda2cParser::C_dq_stringContext *ctx){
    string result_str;
    if (top)
        result_str = ctx->getText();
    else
        result_str = any_cast<string>(visit(ctx->dqString()));
    return result_str;    
}


any EVAL::visitC_newline(lambda2cParser::C_newlineContext *ctx){
    string result_str;
    if (top)
        result_str = "NL";
    else
        result_str = "";
    return result_str;    
}


any EVAL::visitT_appterm(lambda2cParser::T_apptermContext *ctx) {
   return any_cast<string>(visit(ctx->appterm()));
}


any EVAL::visitT_abs_vs(lambda2cParser::T_abs_vsContext *ctx) {
    *changed_ptr = true;
    string vs_str = any_cast<string>(visit(ctx->vs()));
    string var_str = ctx->var()->getText();
    string result_str;
    if (var_str == vs_str)
        result_str = "I"; // conversion rule (1)
    else
        result_str = "(K" + vs_str + ")"; // conversion rule (2)
    return result_str;
}


any EVAL::visitT_abs_ssapp(lambda2cParser::T_abs_ssappContext *ctx) {    
    string result_str;
    
    string var_str = ctx->var()->getText();
    pair<pair<string, string>, string> pp = any_cast<pair<pair<string, string>, string>>(visit(ctx->ssappterm()));
    pair<string, string> pp_first = pp.first;

    string term0_str = pp.first.first;
    string term1_str = pp.first.second;
    string term2_str = pp.second;
    regex sbp("\\[(.*?)\\]");
    string term0_str_ersq = regex_replace(term0_str, sbp, "");
    string term1_str_ersq = regex_replace(term1_str, sbp, "");
    string term2_str_ersq = regex_replace(term2_str, sbp, "");    
    
    term0_str = "(" + term0_str + ")";
    term1_str = "(" + term1_str + ")";
    string pp_second = pp.second;
    if (term0_str_ersq.find(var_str) != string::npos){
        *changed_ptr = true;
        if(term2_str_ersq.find(var_str) == string::npos)
            result_str = "(C(\\" +  var_str + "." + term0_str + term1_str +")" + pp_second +")";// conversion rule (5)
        else
            // conversion rule (7)
            result_str = "(S(\\" +  var_str + "." + term0_str + term1_str +")(\\" +  var_str + "." + pp_second +"))";;
    }
    else{
        *changed_ptr = true;
        if(term1_str == var_str || term1_str == "(" + var_str + ")")
            if(term2_str_ersq.find(var_str) == string::npos)
                result_str = "(C" + term0_str + pp_second +")";// conversion rule (8)
            else
                result_str = "(S" + term0_str + "(\\" +  var_str + "." + pp_second +"))";// conversion rule (9)
        else if(term1_str_ersq.find(var_str) == string::npos){
                if (pp_second == var_str || pp_second == "(" + var_str + ")")
                    result_str = "(" + term0_str + term1_str +")";// conversion rule (10)
                else if (term2_str_ersq.find(var_str) != string::npos)
                    result_str = "(B'" + term0_str + term1_str + "(\\" +  var_str + "." + pp_second +"))";// conversion rule (11)
                else
                    result_str = "(K(" + term0_str + term1_str +  pp_second + "))";// conversion rule (2)
        }     
        else if(term2_str_ersq.find(var_str) == string::npos)
                result_str = "(C'" + term0_str + "(\\" +  var_str + "." + term1_str +")" + pp_second +")";// conversion rule (12)
             else
                // conversion rule (13)
                result_str = "(S'" + term0_str + "(\\" +  var_str + "." + term1_str +")" + "(\\" +  var_str + "." + pp_second +"))";
    }   
    return result_str;
}


any EVAL::visitT_abs_sapp(lambda2cParser::T_abs_sappContext *ctx) {    
    string result_str;
    
    string var_str = ctx->var()->getText();
    pair<string, string> p = any_cast<pair<string, string>>(visit(ctx->sappterm()));
    *changed_ptr = true;
    string term0_str = p.first;
    string term1_str = p.second;
    regex sbp("\\[(.*?)\\]");
    string term0_str_ersq = regex_replace(term0_str, sbp, "");
    string term1_str_ersq = regex_replace(term1_str, sbp, "");

    if (term0_str_ersq.find(var_str) == string::npos)
        if (term1_str_ersq.find(var_str) == string::npos)
            result_str = "(K((" + term0_str + ")(" + term1_str + ")))"; // conversion rule (2)
        else{
            if (var_str == term1_str || "(" + var_str + ")" == term1_str){
                if (term0_str.length() == 1)
                    result_str = term0_str;
                else
                    result_str = "(" + term0_str + ")"; // conversion rule (3)
            }                   
            else // conversion rule (4)
                result_str = "(B(" + term0_str + ")(\\" + var_str + "." + term1_str +"))";
        }            
    else
        if (term1_str_ersq.find(var_str) == string::npos) 
            result_str = "(C(\\" + var_str + "." + term0_str +")(" + term1_str + "))";// conversion rule (5)
        else 
            if (term0_str == term1_str && term0_str.length() != 1)
                result_str = "(B(SII)(\\" +  var_str + "." + term0_str +"))";// conversion rule (6)
            else // conversion rule (7)
                result_str = "(S(\\" + var_str + "." + term0_str +")(\\" + var_str + "." + term1_str +"))";
    return result_str;
}


any EVAL::visitT_abs_term(lambda2cParser::T_abs_termContext *ctx) {
    string result_str;
    string var_str = ctx->var()->getText();
    string term_str = any_cast<string>(visit(ctx->term()));
    regex sbp("\\[(.*?)\\]");
    string term_str_ersq = regex_replace(term_str, sbp, "");
    if (term_str_ersq.find(var_str) == string::npos){
        *changed_ptr = true;
        result_str = "(K" + term_str + ")";// conversion rule (2)
    }
    else
        result_str = "(\\" + var_str + "." + term_str + ")";
    return result_str; 
}


any EVAL::visitSsapp_sapp_a(lambda2cParser::Ssapp_sapp_aContext *ctx) {
    pair<pair<string, string>,string> pp;
    pp.first = any_cast<pair<string, string>>(visit(ctx->sappterm()));
    pp.second = any_cast<string>(visit(ctx->aterm()));
    return pp;
}


any EVAL::visitSsapp_paren(lambda2cParser::Ssapp_parenContext *ctx) {
    return visit(ctx->ssappterm());
}

any EVAL::visitSapp_app_a(lambda2cParser::Sapp_app_aContext *ctx) {
    pair<string, string> p;
    p.first = any_cast<string>(visit(ctx->appterm()));
    p.second = any_cast<string>(visit(ctx->aterm()));
    return p;
}


any EVAL::visitSapp_paren(lambda2cParser::Sapp_parenContext *ctx) {
    return visit(ctx->sappterm());
}


any EVAL::visitApp_a(lambda2cParser::App_aContext *ctx) {
    return any_cast<string>(visit(ctx->aterm()));
}


any EVAL::visitApp_app_a(lambda2cParser::App_app_aContext *ctx) {
    return any_cast<string>(visit(ctx->appterm())) + any_cast<string>(visit(ctx->aterm()));
}


any EVAL::visitA_dparen(lambda2cParser::A_dparenContext *ctx) {
    string result_str = any_cast<string>(visit(ctx->term()));
    if (result_str.length() != 1)
        result_str = "(" + result_str + ")";
    return result_str; 
}


any EVAL::visitA_paren(lambda2cParser::A_parenContext *ctx) {
    string result_str = any_cast<string>(visit(ctx->term()));
    if (result_str.length() != 1)
        result_str = "(" + result_str + ")";
    return result_str; 
}


any EVAL::visitA_vs(lambda2cParser::A_vsContext *ctx) {
    return visit(ctx->vs());
}


any EVAL::visitVs_var(lambda2cParser::Vs_varContext *ctx) {
    return visit(ctx->var());
}


any EVAL::visitVs_combi(lambda2cParser::Vs_combiContext *ctx) {
    return visit(ctx->combi());
}


any EVAL::visitVs_sbid(lambda2cParser::Vs_sbidContext *ctx) {
    return visit(ctx->sbid());
}


any EVAL::visitVs_paren(lambda2cParser::Vs_parenContext *ctx) {
    return visit(ctx->vs());
}


any EVAL::visitCombi(lambda2cParser::CombiContext *ctx) {
    return ctx->getText();
}


any EVAL::visitVar(lambda2cParser::VarContext *ctx) {
    return ctx->getText();
}
  

any EVAL::visitSbid(lambda2cParser::SbidContext *ctx) {
    return ctx->getText();
}
  

any EVAL::visitDqString(lambda2cParser::DqStringContext *ctx) {
    string result_str = ctx->getText();
    (result_str.erase(0, 1)).pop_back();
    return result_str;
}
