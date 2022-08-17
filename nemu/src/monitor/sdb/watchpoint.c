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

#include "sdb.h"

#define NR_WP 32

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  char exprc[32];
  uint64_t value;
  // bool used;

  /* TODO: Add more members if necessary */

} WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    // wp_pool[i].used =false;
    wp_pool[i].value = 0;
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
// low first
WP* new_wp(){
  WP* free_first ;
  if(free_ == NULL){
    printf("no watchpoint! \n");
    assert(0);
  }else{
    free_first = free_;
    free_ = free_->next;
  }


  free_first -> next = head;
  head = free_first;
  // free_first ->used = true;
  // WP* headlast = head;
  // while(headlast->next != NULL){
  //   headlast = headlast->next;
  // }
  // headlast->next = free_first;
  return free_first;
}

void free_wp(WP *wp){
  WP *head_index = head;
  WP * head_pre = NULL ;
  while(head_index  != NULL && head_index -> NO != wp->NO){
    head_pre = head_index;
    head_index =  head_index ->next;
  }

  if(head_index -> NO != wp->NO){
    printf("no such wp \n");
    assert(0);
  }
  if(head_pre != NULL){
    head_pre -> next = head_index ->next;
    head_index ->next = free_;
    free_ = head_index;
  }else {
    head = head ->next;
    head_index ->next = free_;
    free_ = head_index;
  }
  return;
}

bool probe(){
  bool stop = false;
  WP *head_ptr = head;
  while(head_ptr != NULL ){
    bool success;
    uint64_t result = expr(head_ptr ->exprc, &success);
    if(head_ptr ->value != result && success){
      stop = true;
      head_ptr ->value = result;
    }
    head_ptr = head_ptr -> next;
  }
  return stop;

}

void printwp(){
  WP *head_ptr = head;
  int i = 0;
  while(head_ptr != NULL ) {

    printf("Num:%d\texpr:%s\tvalue:%lu\n",i,head_ptr->exprc,head_ptr -> value);
    i++;
    head_ptr = head_ptr ->next;
  }
}

void deletewp(int wp){
  int index = 0;
  WP *head_ptr = head;
  while(index < wp){
    head_ptr = head_ptr -> next;
    index ++ ;
  }
  free_wp(head_ptr);
}


void creatwp(char* exprstr){
     
   WP* wp = new_wp();
  //  printf("come to here \n");
   strcpy(wp ->exprc , exprstr);


   bool success ;
   wp -> value = expr(exprstr, &success);
   return;
}
