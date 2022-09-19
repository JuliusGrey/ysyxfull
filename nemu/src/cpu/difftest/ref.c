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
#include <cpu/cpu.h>
#include <difftest-def.h>
#include <memory/paddr.h>

void difftest_memcpy(paddr_t addr, void *buf, size_t n, bool direction) {
  if (direction == DIFFTEST_TO_REF) memcpy(guest_to_host(addr) , buf, n);
  else memcpy(buf, guest_to_host(addr), n);
  // assert(0);
}



void difftest_regcpy(void *dut, bool direction ,int *test) {
    printf("test is %d",*test);
    // struct kvm_regs *ref = &(vcpu.kvm_run->s.regs.regs);
  riscv64_CPU_state *rcv64 = dut;
printf("send to ref!!!!!!\n");
        for(int i = 0 ; i <32 ; i ++){
        printf("gpr[%d] = 0x%lx\n", i, rcv64->gpr[i]);
      }
        printf("pc is %#lx\n",rcv64->pc);
printf("leave to ref!!!!!!\n");


  if (direction == DIFFTEST_TO_REF) {
    // printf("send to ref!!!!!!\n");
    for( int i = 0; i < 32; i ++ ){
      //  printf("gpr\t%lx\n",rcv64->gpr[i]);
    cpu.gpr[i] = rcv64->gpr[i];}
    cpu.pc = rcv64->pc;
    // printf("pc\t%lx\n",rcv64->pc);
  } else {
    for( int i = 0; i < 32; i ++ ){
     rcv64->gpr[i]=cpu.gpr[i] ;}
     rcv64->pc=cpu.pc ;
  }
  // assert(0);
}
void difftest_setreg(uint64_t regval, int regindex){
  // printf("come to ref!!!!!\n");
  // printf("index is %d", regindex);
  if(regindex < 32){
      cpu.gpr[regindex] = regval;
  }else{
    cpu.pc = regval;
  }
  // printf("leave to ref!!!!!\n");
  return;
}

uint64_t difftest_getreg( int regindex){
  if(regindex < 32){
      return cpu.gpr[regindex] ;
  }else{
    return cpu.pc;
  }
}




void difftest_exec(uint64_t n) {
  cpu_exec(n);
  // assert(0);
}

void difftest_raise_intr(word_t NO) {
  assert(0);
}

void difftest_init(int port) {
  /* Perform ISA dependent initialization. */
  init_isa();
}
