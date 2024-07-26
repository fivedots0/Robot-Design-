# Structure of the ARC
```
.
├── auto_correct.c
├── Core
│   ├── Inc
│   │   ├── camera
│   │   │   ├── camera.h
│   │   │   ├── OV7725_defines.h
│   │   │   ├── OV7725_regs.h
│   │   │   └── sccb.h
│   │   ├── can.h
│   │   ├── dma.h
│   │   ├── gpio.h
│   │   ├── i2c.h
│   │   ├── lcd
│   │   │   ├── lcd_font.h
│   │   │   ├── lcd_graphics.h
│   │   │   ├── lcd.h
│   │   │   ├── lcd_init.h
│   │   │   └── strhelper.h
│   │   ├── main.h
│   │   ├── spi.h
│   │   ├── stm32f4xx_hal_conf.h
│   │   ├── stm32f4xx_it.h
│   │   ├── tim.h
│   │   └── usart.h
│   ├── Src
│   │   ├── arc
│   │   │   ├── arc_camera_main.c
│   │   │   ├── arc_image_processing.c
│   │   │   └── arc_main.c
│   │   ├── camera.c
│   │   ├── can.c
│   │   ├── debug.c
│   │   ├── dma.c
│   │   ├── gpio.c
│   │   ├── i2c.c
│   │   ├── lcd.c
│   │   ├── lcd_graphics.c
│   │   ├── main.c
│   │   ├── sccb.c
│   │   ├── spi.c
│   │   ├── stm32f4xx_hal_msp.c
│   │   ├── stm32f4xx_it.c
│   │   ├── syscalls.c
│   │   ├── sysmem.c
│   │   ├── system_stm32f4xx.c
│   │   ├── tim.c
│   │   ├── tr
│   │   │   └── can_init.c
│   │   ├── tutorial2_hw.c
│   │   ├── tutorial3_pwm.c
│   │   └── usart.c
│   └── Startup
│       └── startup_stm32f405rgtx.s
├── filter.c
└── README.md
```