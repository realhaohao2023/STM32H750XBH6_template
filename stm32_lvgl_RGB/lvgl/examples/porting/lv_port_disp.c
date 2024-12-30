/**
 * @file lv_port_disp_templ.c
 *
 */

/*Copy this file as "lv_port_disp.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_disp.h"
#include <stdbool.h>
#include "lcd_rgb.h"
#include "sdram.h"

/*********************
 *      DEFINES
 *********************/
#define LVGL_MemoryAdd (LCD_MemoryAdd + LCD_Width * LCD_Height * BytesPerPixel_0) // 显示缓冲区地址

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void disp_init(void);

static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);
// static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
//         const lv_area_t * fill_area, lv_color_t color);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_disp_init(void)
{
    /*-------------------------
     * Initialize your display
     * -----------------------*/
    disp_init();

    /*-----------------------------
     * Create a buffer for drawing
     *----------------------------*/

    /**
     * LVGL requires a buffer where it internally draws the widgets.
     * Later this buffer will passed to your display driver's `flush_cb` to copy its content to your display.
     * The buffer has to be greater than 1 display row
     *
     * There are 3 buffering configurations:
     * 1. Create ONE buffer:
     *      LVGL will draw the display's content here and writes it to your display
     *
     * 2. Create TWO buffer:
     *      LVGL will draw the display's content to a buffer and writes it your display.
     *      You should use DMA to write the buffer's content to the display.
     *      It will enable LVGL to draw the next part of the screen to the other buffer while
     *      the data is being sent form the first buffer. It makes rendering and flushing parallel.
     *
     * 3. Double buffering
     *      Set 2 screens sized buffers and set disp_drv.full_refresh = 1.
     *      This way LVGL will always provide the whole rendered screen in `flush_cb`
     *      and you only need to change the frame buffer's address.
     */

#if 1 // 这部分是lgvl port的示例代码
      /* Example for 1) */
      // static lv_disp_draw_buf_t draw_buf_dsc_1;
      // static lv_color_t buf_1[MY_DISP_HOR_RES * 10];                             /*A buffer for 10 rows*/
      // lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, NULL, MY_DISP_HOR_RES * 10); /*Initialize the display buffer*/

    // 选择第一种方法，注释掉后面两种方法
    /* Example for 2) */
    //  static lv_disp_draw_buf_t draw_buf_dsc_1;
    //  static lv_color_t buf_2_1[MY_DISP_HOR_RES * 90];                        /*A buffer for 10 rows*/
    //  static lv_color_t buf_2_2[MY_DISP_HOR_RES * 90];                        /*An other buffer for 10 rows*/
    //  lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_2_1, buf_2_2, MY_DISP_HOR_RES * 90);   /*Initialize the display buffer*/

    // /* Example for 3) also set disp_drv.full_refresh = 1 below*/
    static lv_disp_draw_buf_t draw_buf_dsc_3;
    static lv_color_t *buf_3_1 = (lv_color_t *)(LVGL_MemoryAdd); /*A screen sized buffer*/
    static lv_color_t *buf_3_2 = (lv_color_t *)(LVGL_MemoryAdd + LCD_Width * LCD_Height * BytesPerPixel_0);
    lv_disp_draw_buf_init(&draw_buf_dsc_3, buf_3_1, buf_3_2,
                          LCD_Width * LCD_Height); /*Initialize the display buffer*/
#else                                              // 这部分是使用sdram的代码
    // static lv_disp_draw_buf_t draw_buf_dsc_1;
    // static lv_color_t buf_1[MY_DISP_HOR_RES * 400] __attribute__((at(0xC0000000 + 1024 * 600 * 2))); /*A buffer for 10 rows*/
    // lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, NULL, 320 * 240);                                 // 这里要改成屏幕大小  /*Initialize the display buffer*/

    // /* Example for 2) */
    // static lv_disp_draw_buf_t draw_buf_dsc_2;
    // static lv_color_t buf_2_1[MY_DISP_HOR_RES * 400] __attribute__((at(0xC0000000 + 1024 * 600 * 2))); /*A buffer for 10 rows*/
    // static lv_color_t buf_2_2[MY_DISP_HOR_RES * 400] __attribute__((at(0xC0000000 + 1024 * 600 * 4))); // 因为上一个数组已经占了                         /*An other buffer for 10 rows*/
    // lv_disp_draw_buf_init(&draw_buf_dsc_2, buf_2_1, buf_2_2, 320 * 240);                              // 这里要改成屏幕大小  /*Initialize the display buffer*/

    // /* Example for 3) also set disp_drv.full_refresh = 1 below*/
    // static lv_disp_draw_buf_t draw_buf_dsc_3;
    // static lv_color_t buf_3_1[MY_DISP_HOR_RES * MY_DISP_VER_RES] __attribute__((at(0xC0000000 + 1024 * 600 * 2))); /*A screen sized buffer*/
    // static lv_color_t buf_3_2[MY_DISP_HOR_RES * MY_DISP_VER_RES] __attribute__((at(0xC0000000 + 1024 * 600 * 4))); /*Another screen sized buffer*/
    // lv_disp_draw_buf_init(&draw_buf_dsc_3, buf_3_1, buf_3_2,
    //                       320 * 240); // 这里要改成屏幕大小、

    // /* Example for 1) */
    // static lv_disp_draw_buf_t draw_buf_dsc_1;
    // static lv_color_t buf_1[MY_DISP_HOR_RES * MY_DISP_VER_RES] __attribute__((at(0xC0000000))); /*A buffer for the entire screen*/
    // lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, NULL, MY_DISP_HOR_RES * MY_DISP_VER_RES);     /*Initialize the display buffer*/

    /* Example for 2) */
    static lv_disp_draw_buf_t draw_buf_dsc_2;
    static lv_color_t buf_2_1[MY_DISP_HOR_RES * MY_DISP_VER_RES] __attribute__((at(0xC0000000 + MY_DISP_HOR_RES * MY_DISP_VER_RES * sizeof(lv_color_t))));     /*A buffer for the entire screen*/
    static lv_color_t buf_2_2[MY_DISP_HOR_RES * MY_DISP_VER_RES] __attribute__((at(0xC0000000 + 2 * MY_DISP_HOR_RES * MY_DISP_VER_RES * sizeof(lv_color_t)))); /*Another buffer for the entire screen*/
    lv_disp_draw_buf_init(&draw_buf_dsc_2, buf_2_1, buf_2_2, MY_DISP_HOR_RES * MY_DISP_VER_RES);                                                               /*Initialize the display buffer*/

    /* Example for 3) also set disp_drv.full_refresh = 1 below*/
    // static lv_disp_draw_buf_t draw_buf_dsc_3;
    // static lv_color_t buf_3_1[MY_DISP_HOR_RES * MY_DISP_VER_RES] __attribute__((at(0xC0000000)));                                                          /*A screen sized buffer*/
    // static lv_color_t buf_3_2[MY_DISP_HOR_RES * MY_DISP_VER_RES] __attribute__((at(0xC0000000 + MY_DISP_HOR_RES * MY_DISP_VER_RES * sizeof(lv_color_t)))); /*Another screen sized buffer*/
    // lv_disp_draw_buf_init(&draw_buf_dsc_3, buf_3_1, buf_3_2, MY_DISP_HOR_RES * MY_DISP_VER_RES);                                                           /*Initialize the display buffer*/

#endif

    /*-----------------------------------
     * Register the display in LVGL
     *----------------------------------*/

    static lv_disp_drv_t disp_drv; /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv); /*Basic initialization*/

    /*Set up the functions to access to your display*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = LCD_Width;
    disp_drv.ver_res = LCD_Height;

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = disp_flush;

    /*Set a display buffer*/
    disp_drv.draw_buf = &draw_buf_dsc_3;

    /*Required for Example 3)*/
    disp_drv.full_refresh = 1;

    /* Fill a memory array with a color if you have GPU.
     * Note that, in lv_conf.h you can enable GPUs that has built-in support in LVGL.
     * But if you have a different GPU you can use with this callback.*/
    // disp_drv.gpu_fill_cb = gpu_fill;

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*Initialize your display and the required peripherals.*/
static void disp_init(void)
{
    /*You code here*/
}

volatile bool disp_flush_enabled = true;

/* Enable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_enable_update(void)
{
    disp_flush_enabled = true;
}

/* Disable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_disable_update(void)
{
    disp_flush_enabled = false;
}

//LTDC中断回调函数
/**
  * @brief  Line Event callback.
  * @param  hltdc: pointer to a LTDC_HandleTypeDef structure that contains
  *                the configuration information for the specified LTDC.
  * @retval None
  */
void HAL_LTDC_LineEvenCallback(LTDC_HandleTypeDef *hltdc)
{   
	// 重新载入参数，新显存地址生效，此时显示才会更新
	// 每次进入中断才会更新显示，这样能有效避免撕裂现象	
	__HAL_LTDC_RELOAD_CONFIG(hltdc);						
	HAL_LTDC_ProgramLineEvent(hltdc, 0);
    
}


/*Flush the content of the internal buffer the specific area on the display
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_disp_flush_ready()' has to be called when finished.*/
static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    if (disp_flush_enabled)
    {
        /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/
#if 1
       LTDC_Layer1->CFBAR = (uint32_t )color_p;			// 切换显存地址
#else
        // 使用 DMA2D 进行图像传输
        DMA2D_Transfer((uint32_t *)color_p, area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1);

        // 等待 DMA2D 传输完成
        while (DMA2D_Transfer_Complete() == false)
            ;

        // 将图像数据通过 SPI 发送到 LCD
        LCD_WriteBuff((uint16_t *)color_p, (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1));

#endif
    }

    /*IMPORTANT!!!
     *Inform the graphics library that you are ready with the flushing*/
    lv_disp_flush_ready(disp_drv);
}

/*OPTIONAL: GPU INTERFACE*/

/*If your MCU has hardware accelerator (GPU) then you can use it to fill a memory with a color*/
// static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
//                     const lv_area_t * fill_area, lv_color_t color)
//{
//     /*It's an example code which should be done by your GPU*/
//     int32_t x, y;
//     dest_buf += dest_width * fill_area->y1; /*Go to the first line*/
//
//     for(y = fill_area->y1; y <= fill_area->y2; y++) {
//         for(x = fill_area->x1; x <= fill_area->x2; x++) {
//             dest_buf[x] = color;
//         }
//         dest_buf+=dest_width;    /*Go to the next line*/
//     }
// }

#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
