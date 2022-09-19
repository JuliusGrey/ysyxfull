#include <am.h>
#include <nemu.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)

// void __am_gpu_init() {
// }

// void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
//   *cfg = (AM_GPU_CONFIG_T) {
//     .present = true, .has_accel = false,
//     .width = 0, .height = 0,
//     .vmemsz = 0
//   };
// }

void __am_gpu_init() {
//   uint32_t vgamsg = inl(VGACTL_ADDR);
//  int i;
//   int w = (int)(vgamsg >> 16);  // TODO: get the correct width
//  int h = (int)(vgamsg & 0x0000ffff);  // TODO: get the correct height
//   uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
//   for (i = 0; i < w * h; i ++) {fb[i] = i;};
//   outl(SYNC_ADDR, 1);
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  uint32_t vgamsg = inl(VGACTL_ADDR);
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = (int)(vgamsg >> 16), .height = (int)(vgamsg & 0x0000ffff),
    .vmemsz = 0
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {

     uint32_t vgamsg = inl(VGACTL_ADDR);
    int wight = (int)(vgamsg >> 16);  // TODO: get the correct width
    // if(wight == 400){
    //   putstr("come to here");
    // }
    int x = ctl->x;
    int y = ctl ->y;
        int w = ctl ->w;
    int h = ctl ->h;
    uint32_t *color_buf = ctl ->pixels;
    for(int i = 0; i < h ; i ++){
      // uint32_t *rowbase = (uint32_t *)(uintptr_t)(FB_ADDR + (y + i)*wight + x);
      uint32_t *rowbase = (uint32_t *)(uintptr_t)(FB_ADDR );
      for( int j = 0 ; j < w ; j ++){
        // rowbase[j] = color_buf[i*w + j];
        rowbase[(i + y)*wight + x + j] = color_buf[i*w + j];
      }
    }
    // uint32_t *rowbase = (uint32_t *)(uintptr_t)(FB_ADDR );
    // for(int i = 0 ; i < h ; i ++ ) rowbase[i*wight] = color_buf[i];


  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
