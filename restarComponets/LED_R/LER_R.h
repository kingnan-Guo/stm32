//
// Created by 90175 on 2024/11/7.
//
#ifndef STM32F103C8T6PROJECT_LER_R_H
#define STM32F103C8T6PROJECT_LER_R_H
extern "C" {
    class LED_R{
    public:
        void  run();
    };
    void LED_R_Init(void);
    void LED_R_RESET();
    void LED_R_SET();
    void LED_R_test();
};

#endif //STM32F103C8T6PROJECT_LER_R_H
