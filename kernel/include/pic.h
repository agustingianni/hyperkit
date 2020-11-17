#ifndef PIC_H_
#define PIC_H_

#define PIC_MASTER_CMD 0x20
#define PIC_MASTER_IMR 0x21
#define PIC_SLAVE_CMD 0xa0
#define PIC_SLAVE_IMR 0xa1

void init_pic();
void pic_send_eoi(int line);

#endif /* PIC_H_ */