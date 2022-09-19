/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include <memory/paddr.h>
enum {
  TK_NOTYPE = 256, TK_EQ,TK_DNUM,TK_BRF,TK_BRL,TK_REG,TK_HNUM,TK_PTR,TK_IE,TK_AND,

  /* TODO: Add more token types */

};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */
  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"\\-", '-'},         // sub
  {"\\*", '*'},         // mul
  {"\\/", '/'},         // de
  {"==", TK_EQ},        // equal
  {"!=", TK_IE},        // iequal
  {"&&", TK_AND},        // and
  {"\\(", TK_BRF},        // bracket-first
  {"\\)", TK_BRL},        // bracket-last
  {"[1-9][0-9]*|0" ,TK_DNUM},
  {"\\$[\\$a-z][a-z0-9]+" ,TK_REG},
  {"0x[a-fA-F0-9]+" ,TK_HNUM},
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
      // printf(" regexec(&re[i], e + position, 1, &pmatch, 0) is %d \n",  regexec(&re[i], e + position, 1, &pmatch, 0));
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
          switch (rules[i].token_type) {
          // default: TODO();
           case TK_NOTYPE :
               break;
            case TK_DNUM :  
                  strncpy(tokens[nr_token].str,substr_start,substr_len);
                  tokens[nr_token].type =   rules[i].token_type;
                  nr_token++;
                  break;
            case TK_REG :  
                  strncpy(tokens[nr_token].str,substr_start + 1,substr_len -1);
                  tokens[nr_token].type =   rules[i].token_type;
                  nr_token++;
                  break;
            case TK_HNUM :  
                  strncpy(tokens[nr_token].str,substr_start,substr_len);
                  tokens[nr_token].type =   rules[i].token_type;
                  nr_token++;
                  break;
            default :
              tokens[nr_token].type =   rules[i].token_type;
              nr_token++;
          break;}

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

long long change_num(char *str, int length)
{
   char revstr[16] = {0}; //根据十六进制字符串的长度，这里注意数组不要越界
   long long num[16] = {0};
   long long count = 1;
   long long result = 0;
   
   strcpy(revstr, str);

   for(int i = length - 1; i >= 0; i--)
   {
      if((revstr[i] >= '0') && (revstr[i] <= '9'))
      {
         num[i] = revstr[i] - 48;   //字符0的ASCII值为48
      }
      else if((revstr[i] >= 'a') && (revstr[i] <= 'f'))
      {
         num[i] = revstr[i] - 'a' + 10;
      }
      else if((revstr[i] >= 'A') && (revstr[i] <= 'F'))
      {
         num[i] = revstr[i] - 'A' + 10;
      }
      else
      {
         num[i] = 0;
      }

      result += num[i] * count;
      count *= 16; //十六进制(如果是八进制就在这里乘以8)
   }

   return result;
}


int find_main_op(int sp, int ep){

  int bra_num = 0;

  for(int i = sp ;i <= ep ;i++ ){
    if(tokens[i].type == TK_BRF){
      bra_num++;
    }else if(tokens[i].type == TK_BRL){
      bra_num--;
    }
    if((tokens[i].type == TK_EQ || tokens[i].type == TK_IE) && bra_num == 0){
      return i;
    }
  }


  for(int i = sp ;i <= ep ;i++ ){
    if(tokens[i].type == TK_BRF){
      bra_num++;
    }else if(tokens[i].type == TK_BRL){
      bra_num--;
    }
    if((tokens[i].type == '+' || tokens[i].type == '-') && bra_num == 0){
      return i;
    }
  }

  for(int i = sp ;i <= ep ;i++ ){
     if(tokens[i].type == TK_BRF){
      bra_num++;
    }else if(tokens[i].type == TK_BRL){
      bra_num--;
    }   
    if((tokens[i].type == '*' || tokens[i].type == '/') && bra_num == 0){
      return i;
    }
  }

  for(int i = sp ;i <= ep ;i++ ){
     if(tokens[i].type == TK_BRF){
      bra_num++;
    }else if(tokens[i].type == TK_BRL){
      bra_num--;
    }   
    if((tokens[i].type == TK_AND) && bra_num == 0){
      return i;
    }
  }

  return 0;
}
uint64_t eval (int p , int q){
  if(p >q){
  }else if(p == q){
    if(tokens[ p].type== TK_DNUM){
      return strtoul(tokens[ p].str,NULL,10);
    }else if(tokens[ p].type== TK_REG){
      bool success;
      uint64_t regval = isa_reg_str2val(tokens[ p].str, &success);
      if(success){
        return regval;
      }else {
        printf("read reg fail \n");
        // return 0;
        assert(0);
      }
    }else if(tokens[ p].type== TK_HNUM){
      return change_num(tokens[ p].str, strlen(tokens[ p].str));
    }
  }else if(tokens[p].type == TK_BRF && tokens[q].type == TK_BRL){
    return eval(p+1 , q-1);
  }else if(p <= q && tokens[q].type == TK_PTR){
    uint32_t baseaddr = eval(p + 1 ,q);
    uint32_t addr =baseaddr +  RESET_VECTOR;
    return  paddr_read(addr , 4);


  }else{
    int main_op = find_main_op(p,q);
    int val1 = eval(p ,main_op -1);
    int val2 = eval(main_op + 1 ,q);
    switch (tokens[main_op].type)
    {
    case '+':
      return val1 + val2;
      break;
    case '-':
      return val1 - val2;
      break;
    case '*':
      return val1 * val2;
      break;
    case '/':
      return val1 / val2;
      break;
    case TK_AND :
       return val1 && val2;
       break;
    case TK_EQ :
       return val1 == val2;
       break;
    case TK_IE :
       return val1 != val2;
       break;
    default:
       return 0;
      break;
    }
  }
  return 0;
  

}

uint64_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

for (int i = 0; i < nr_token; i ++) {
  if (tokens[i].type == '*' && (i == 0 || tokens[i - 1].type == '+'|| tokens[i - 1].type == '-'|| tokens[i - 1].type == '*'|| tokens[i - 1].type == '/'|| tokens[i - 1].type == TK_BRF) ) {
    tokens[i].type = TK_PTR;
  }
}
  /* TODO: Insert codes to evaluate the expression. */
  // TODO();
  int p= 0;
  int q=  nr_token - 1;
  *success = true;
  return eval(p,q);
  

  return 0;
}