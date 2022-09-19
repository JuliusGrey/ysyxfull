#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  uint32_t keymsg = inl(KBD_ADDR);
  kbd->keydown = (KEYDOWN_MASK & keymsg) != 0;
  // if(kbd->keydown != AM_KEY_NONE){
  // putstr("come to here");}
  kbd->keycode = ~KEYDOWN_MASK & keymsg;
// kbd->keydown =1;
// kbd->keycode =0;

  // if(kbd->keycode != AM_KEY_NONE) kbd->keydown = 1;
  
}
